#include <list>

#include "GlulxMemory.h"

#include "Impl/GlulxError.h"

#include "Impl/Header/GlulxHeader.h"

#include "Util/Endian.h"
#include "Util/MathsUtil.h"

using namespace fiction::endian;

constexpr auto THIRTY_TWO_MEGABYTES = 32u * 1024u * 1024u;
constexpr auto PAGE_SIZE = 256u;

namespace {

template<class T>
auto Resize(std::vector<T>& v, uint32_t size) -> bool {
    // Note: it would be better to catch a specific exception.
    // However, while resize has a strong exception guarantee,
    // I don't think the exception type is mandated by the standard
    try {
        v.resize(size);
        return true;
    } catch (...) {
    }
    return false;
}
}

namespace fiction::glulx {
namespace {

auto ComputeHeapExtension(auto heapSize, auto length) {
    const auto extension = std::max(
        { length, PAGE_SIZE, std::min(heapSize, THIRTY_TWO_MEGABYTES) }
    );
    return extension + ComputeAlignmentPadding(extension, PAGE_SIZE);
}

struct HeapBlock {
    HeapBlock(uint32_t address, uint32_t length, bool free) :
        address(address), length(length), free(free) {}

    uint32_t address;
    uint32_t length;
    bool free;
};

auto GetMemorySafe(std::vector<uint8_t> memory, const GlulxHeader& header) -> std::vector<uint8_t> {
    if (IsHeaderValid(header)) {
        return memory;
    }
    return {};
}

auto GetRamStartSafe(const GlulxHeader& header) {
    return IsHeaderValid(header) ? header.ramstart : 0u;
}

auto GetExtStartSafe(const GlulxHeader& header) {
    return IsHeaderValid(header) ? header.extstart : 0u;
}

auto GetEndMemSafe(const GlulxHeader& header) {
    return IsHeaderValid(header) ? header.endmem : 0u;
}

}

struct GlulxMemory::Data {
    explicit Data(std::vector<uint8_t> memory_in, const GlulxHeader& header_in) :
        memory(GetMemorySafe(std::move(memory_in), header_in)),
        protectAddress(0u),
        protectLength(0u),
        blocks(),
        heapStart(0),
        allocations(0),
        ramstart(GetRamStartSafe(header_in)),
        extstart(GetExtStartSafe(header_in)),
        endmem(GetEndMemSafe(header_in)),
        header(header_in),
        originalRam(memory.begin() + ramstart, memory.begin() + extstart) {}

    std::vector<uint8_t> memory;

    uint32_t protectAddress;
    uint32_t protectLength;

    std::list<HeapBlock> blocks;
    uint32_t heapStart;
    uint32_t allocations;

    const uint32_t ramstart;
    const uint32_t extstart;
    const uint32_t endmem;

    const GlulxHeader header;

    const std::vector<uint8_t> originalRam;
};

GlulxMemory::GlulxMemory(std::vector<uint8_t> memory, const GlulxHeader& header) :
    data(std::make_unique<Data>(std::move(memory), header)) {}

GlulxMemory::~GlulxMemory() = default;

auto GlulxMemory::GetSize() const -> uint32_t { return data->memory.size(); }
auto GlulxMemory::GetRamStart() const -> uint32_t { return data->ramstart; }
auto GlulxMemory::GetHeapStart() const -> uint32_t { return data->heapStart; }
auto GlulxMemory::GetExtStart() const -> uint32_t { return data->extstart; }
auto GlulxMemory::GetEndMem() const -> uint32_t { return data->endmem; }

auto GlulxMemory::SetProtect(uint32_t address, uint32_t length) -> void {
    // Note: this is different from what the spec says
    // We do what the reference implementation does and cancel when length is zero
    data->protectAddress = length == 0u ? 0 : address;
    data->protectLength = length;
}

auto GlulxMemory::HeapClear() -> void {
    this->data->blocks.clear();
    this->data->heapStart = 0u;
    this->data->allocations = 0u;
}

auto GlulxMemory::HeapActive() -> bool {
    return this->data->allocations != 0u;
}

auto GlulxMemory::SetSize(uint32_t size) -> bool {
    if (HeapActive()) {
        return false;
    }
    if (size >= GetEndMem() && ComputeAlignmentPadding(size, PAGE_SIZE) == 0u) {
        return ::Resize(data->memory, size);
    }
    return false;
}

auto GlulxMemory::CheckReadableAddress(uint32_t address, uint32_t size) const -> void {
    if (address + size - 1u >= GetSize()) {
        Error("Attempted to read off the end of memory");
    }
}

auto GlulxMemory::CheckWriteableAddress(uint32_t address, uint32_t size) const -> void {
    if (address < GetRamStart()) {
        Error("Attempted to write to read only memory");
    }
    if (address + size - 1u >= GetSize()) {
        Error("Attempted to write off the end of memory");
    }
}

auto GlulxMemory::Read8(uint32_t address) const -> uint32_t {
    CheckReadableAddress(address, 1u);
    return ::Read8(data->memory, address);
}

auto GlulxMemory::Read16(uint32_t address) const -> uint32_t {
    CheckReadableAddress(address, 2u);
    return ::Read16(data->memory, address);
}

auto GlulxMemory::Read32(uint32_t address) const -> uint32_t {
    CheckReadableAddress(address, 4u);
    return ::Read32(data->memory, address);
}

auto GlulxMemory::Write8(uint32_t address, uint32_t v) -> void {
    CheckWriteableAddress(address, 1u);
    ::Write8(data->memory, address, v);
}

auto GlulxMemory::Write8Protected(uint32_t address, uint32_t v) -> void {
    const auto start = data->protectAddress;
    const auto end = start + data->protectLength;
    if (address >= start && address < end) {
        return;
    }
    Write8(address, v);
}

auto GlulxMemory::Write16(uint32_t address, uint32_t v) -> void {
    CheckWriteableAddress(address, 2u);
    ::Write16(data->memory, address, v);
}

auto GlulxMemory::Write32(uint32_t address, uint32_t v) -> void {
    CheckWriteableAddress(address, 4u);
    ::Write32(data->memory, address, v);
}

// Note: The algorithm used in Malloc works as follows
// 1. Try to find a big enough free block
// 2. If a big enough block is found:
//    If the fit is exact, use the entire block.
//    Otherwise, split the block in two.
//    If the following block is free then merge the free area into that
//    otherwise insert a new free block after the one being allocated
// 3. Otherwise, allocate a new block at the end. The size of the new block is
//    max(heap size*, 256, length). NB in our case heap size* is
//    max(heap size, 32Mb) to prevent exponential growth.

auto GlulxMemory::Malloc(uint32_t length) -> uint32_t {

    if (length == 0u) {
        Error("Requested malloc of length zero");
    }

    const auto& predicate = [&length](const auto& block) {
        return block.free && block.length >= length;
    };

    auto& blocks = data->blocks;

    auto block = std::find_if(std::begin(blocks), std::end(blocks), predicate);

    if (block != std::end(blocks)) {
        const auto nextAddress = block->address + length;
        const auto nextLength = block->length - length;
        block->length = length;
        block->free = false;
        data->allocations++;
        if (nextLength != 0) {
            auto next = std::next(block);
            if (next == std::end(blocks) || !next->free) {
                blocks.emplace(next, nextAddress, nextLength, true);
            } else {
                next->address = nextAddress;
                next->length += nextLength;
            }
        }
        return block->address;
    }

    const auto address = GetSize();
    const auto heapSize = HeapActive() ? address - data->heapStart : 0u;
    const auto extension = ComputeHeapExtension(heapSize, length);
    if (!Resize(data->memory, address + extension)) {
        return 0u;
    }
    if (data->heapStart == 0u) {
        data->heapStart = address;
    }
    data->allocations++;
    if (extension > length) {
        blocks.emplace_back(address, length, false);
        blocks.emplace_back(address + length, extension - length, true);
    } else {
        blocks.emplace_back(address, length, false);
    }

    return address;
}

// Note: The algorithm used in Free works as follows
// 1. Free the existing block
// 2. Look at the block's predecessor
//      If that's free, merge with that
// 3. Look at the block's successor
//      If that's free, merge with that
//
// This, combined with the merging behaviour in Malloc, should
// guarantee that there are never two adjacent free blocks in
// the block list.

auto GlulxMemory::Free(uint32_t address) -> void {

    const auto& predicate = [&address](const auto& block) {
        return block.address == address && !block.free;
    };

    auto& blocks = data->blocks;

    auto block = std::find_if(std::begin(blocks), std::end(blocks), predicate);

    if (block == std::end(blocks)) {
        Error("Attempted to free a non extant block");
    }

    if (--data->allocations == 0u) {
        Resize(data->memory, data->heapStart);
        HeapClear();
        return;
    }

    block->free = true;

    auto prev = std::prev(block);
    if (prev != std::end(blocks) && prev->free) {
        block->address = prev->address;
        block->length += prev->length;
        block = blocks.erase(prev);
    }

    auto next = std::next(block);
    if (next != std::end(blocks) && next->free) {
        block->length += next->length;
        blocks.erase(next);
    }
}

auto GlulxMemory::GetRomSummary() const -> std::vector<uint8_t> {
    return { data->memory.begin(), data->memory.begin() + 128u };
}

auto GlulxMemory::SetRomSummary(const std::vector<uint8_t>& summary) const -> bool {
    for (auto i = 0u; i < summary.size(); ++i) {
        if (summary[i] != Read8(i)) {
            return false;
        }
    }
    return true;
}

auto GlulxMemory::GetRamSummary() const -> std::vector<uint8_t> {

    std::vector<uint8_t> summary(GetSize() - GetRamStart());

    auto j { 0u };

    for (auto i = GetRamStart(); i < GetExtStart(); ++i, ++j) {
        summary[j] = data->originalRam[j] ^ Read8(i);
    }

    for (auto i = GetExtStart(); i < GetSize(); ++i, ++j) {
        summary[j] = Read8(i);
    }

    return summary;
}

auto GlulxMemory::SetRamSummary(uint32_t size, const std::vector<uint8_t>& summary) -> void {

    HeapClear();

    Resize(data->memory, size);

    auto j { 0u };

    for (auto i = GetRamStart(); i < GetExtStart(); ++i, ++j) {
        Write8Protected(i, data->originalRam[j] ^ summary[j]);
    }

    for (auto i = GetExtStart(); i < GetSize(); ++i, ++j) {
        Write8Protected(i, summary[j]);
    }
}

auto GlulxMemory::GetHeapSummary() const -> std::vector<std::pair<uint32_t, uint32_t>> {
    std::vector<std::pair<uint32_t,uint32_t>> summary;
    summary.reserve(data->allocations);
    for (const auto& [address, length, free] : data->blocks) {
        if (!free) {
            summary.emplace_back(address, length);
        }
    }
    return summary;
}

auto GlulxMemory::SetHeapSummary(uint32_t heapStart, const std::vector<std::pair<uint32_t, uint32_t>>& summary) -> void {

    HeapClear();

    if (heapStart == 0u) {
        return;
    }

    data->heapStart = heapStart;
    data->allocations = summary.size();

    auto& blocks = data->blocks;
    auto prevAddress = heapStart;

    for (const auto& [address, length] : summary) {
        const auto prevLength = address - prevAddress;
        if (prevLength != 0u) {
            blocks.emplace_back(prevAddress, prevLength, true);
        }
        blocks.emplace_back(address, length, false);
        prevAddress = address + length;
    }

    const auto lastLength = GetSize() - prevAddress;
    if (lastLength != 0u) {
        blocks.emplace_back(prevAddress, lastLength, true);
    }
}

auto GlulxMemory::Restart() -> void {
    if (!IsHeaderValid(data->header)) {
        return;
    }
    std::vector<uint8_t> zero(GetEndMem() - GetRamStart());
    SetRamSummary(GetEndMem(), zero);
}

}