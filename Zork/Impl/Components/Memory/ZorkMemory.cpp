#include "ZorkMemory.h"

#include "Impl/Header/ZorkHeader.h"
#include "Impl/ZorkError.h"

#include "Common/Endian.h"

using namespace fiction::zork;
using namespace fiction::endian;

ZorkMemory::ZorkMemory(std::vector<uint8_t> memory_in, const ZorkHeader& header_in) :
    header(header_in),
    memory(std::move(memory_in)),
    original(memory.begin(), memory.begin() + header.staticMemory) {}

ZorkMemory::~ZorkMemory() = default;

auto ZorkMemory::Restart() -> void {
    // TODO preserve lower bits of flags 2 in the header area
    for (auto i = 0u; i < original.size(); ++i) {
        memory[i] = original[i];
    }
}

auto ZorkMemory::Read8(uint32_t address) const -> uint32_t {
    if (address < memory.size()) {
        return ::Read8(memory, address);
    }
    Error("Attempted to read byte off the end of memory");
}

auto ZorkMemory::Read16(uint32_t address) const -> uint32_t {
    if (address + 1u < memory.size()) {
        return ::Read16(memory, address);
    }
    Error("Attempted to read word off the end of memory");
}

auto ZorkMemory::Write8(uint32_t address, uint32_t value) -> void {
    if (address < header.staticMemory) {
        ::Write8(memory, address, value);
        return;
    }
    Error("Attempted to write byte into static memory");
}

auto ZorkMemory::Write16(uint32_t address, uint32_t value) -> void {
    if (address + 1u < header.staticMemory) {
        ::Write16(memory, address, value);
        return;
    }
    Error("Attempted to write word into static memory");
}