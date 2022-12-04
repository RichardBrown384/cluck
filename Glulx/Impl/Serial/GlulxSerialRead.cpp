#include "GlulxSerial.h"

#include "GlulxReadBuffer.h"
#include "GlulxCompression.h"
#include "GlulxIff.h"

#include "Impl/Components/Memory/GlulxMemory.h"
#include "Impl/Components/Stack/GlulxStack.h"

namespace fiction::glulx {
namespace {

auto ReadIfhd(GlulxReadBuffer& buffer, GlulxMemory& memory, auto length) {
    const auto& summary = buffer.ReadBuffer(length);
    return memory.SetRomSummary(summary);
}

auto ReadCmem(GlulxReadBuffer& buffer, GlulxMemory& memory, auto length) {
    const auto memSize = buffer.Read32();
    const auto& data = buffer.ReadBuffer(length - 4u);
    const auto& summary = DecompressBuffer(data, memSize);
    memory.SetRamSummary(memSize, summary);
}

auto ReadMall(GlulxReadBuffer& buffer, GlulxMemory& memory, auto length) {
    if (length == 0u) {
        memory.SetHeapSummary(0u, {});
        return;
    }
    const auto heapStart = buffer.Read32();
    const auto blockCount = buffer.Read32();
    std::vector<std::pair<uint32_t, uint32_t>> summary;
    for (auto i = 0u; i < blockCount; ++i) {
        const auto blockAddress = buffer.Read32();
        const auto blockLength = buffer.Read32();
        summary.emplace_back(blockAddress, blockLength);
    }
    std::sort(summary.begin(), summary.end());
    memory.SetHeapSummary(heapStart, summary);
}

auto ReadStks(GlulxReadBuffer& buffer, GlulxStack& stack, auto length) {
    const auto& summary = buffer.ReadBuffer(length);
    stack.SetSummary(summary);
}

bool ReadIff(GlulxReadBuffer& buffer, GlulxMemory& memory, GlulxStack& stack) {
    while (buffer.GetPosition() < buffer.GetSize()) {
        const auto type = buffer.Read32();
        const auto length = buffer.Read32();
        switch (type) {
            case IFHD:
                if (!ReadIfhd(buffer, memory, length)) {
                    return false;
                }
                break;
            case CMEM:
                ReadCmem(buffer, memory, length);
                break;
            case MALL:
                ReadMall(buffer, memory, length);
                break;
            case STKS:
                ReadStks(buffer, stack, length);
                break;
            default:
                buffer.Skip(length);
                break;
        }
        buffer.Align16();
    }

    return true;
}

}

auto ReadIff(
    const std::vector<uint8_t>& data,
    GlulxMemory& memory,
    GlulxStack& stack) -> bool {

    GlulxReadBuffer buffer(data);

    const auto form = buffer.Read32();
    const auto length = buffer.Read32();
    const auto ifzs = buffer.Read32();

    if (form != FORM) {
        return false;
    }
    if (length != buffer.GetSize() - 8u) {
        return false;
    }
    if (ifzs != IFZS) {
        return false;
    }

    return ReadIff(buffer, memory, stack);

}

}