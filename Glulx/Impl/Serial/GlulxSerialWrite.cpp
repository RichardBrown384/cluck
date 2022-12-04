#include <cstdint>

#include "GlulxWriteBuffer.h"
#include "GlulxCompression.h"
#include "GlulxIff.h"

#include "Impl/Components/Memory/GlulxMemory.h"
#include "Impl/Components/Stack/GlulxStack.h"

namespace fiction::glulx {

namespace {
struct TypeLength {
    TypeLength(GlulxWriteBuffer& buffer_in, uint32_t type) :
        buffer(buffer_in),
        position(buffer_in.GetPosition() + 8u) {
        buffer.Write32(type);
        buffer.Write32(0u);
    }

    ~TypeLength() {
        buffer.Write32At(position - 4u, buffer.GetPosition() - position);
    }

    GlulxWriteBuffer& buffer;
    uint32_t position;
};

auto WriteIfhd(GlulxWriteBuffer& buffer, const GlulxMemory& memory) {
    TypeLength typeLength(buffer, IFHD);
    const auto summary = memory.GetRomSummary();
    buffer.WriteBuffer(summary);
}

auto WriteCmem(GlulxWriteBuffer& buffer, const GlulxMemory& memory) {
    TypeLength typeLength(buffer, CMEM);
    const auto summary = memory.GetRamSummary();
    const auto compressed = CompressBuffer(summary);
    buffer.Write32(memory.GetSize());
    buffer.WriteBuffer(compressed);
}

auto WriteMall(GlulxWriteBuffer& buffer, const GlulxMemory& memory) {
    TypeLength typeLength(buffer, MALL);
    const auto heapStart = memory.GetHeapStart();
    const auto summary = memory.GetHeapSummary();
    if (heapStart == 0u) {
        return;
    }
    buffer.Write32(heapStart);
    buffer.Write32(summary.size());
    for (const auto&[address, length]: summary) {
        buffer.Write32(address);
        buffer.Write32(length);
    }
}

auto WriteStks(GlulxWriteBuffer& buffer, const GlulxStack& stack) {
    TypeLength typeLength(buffer, STKS);
    const auto summary = stack.GetSummary();
    buffer.WriteBuffer(summary);
}

auto WriteIff(GlulxWriteBuffer& buffer, const GlulxMemory& memory, const GlulxStack& stack) {
    TypeLength form(buffer, FORM);

    buffer.Write32(IFZS);

    WriteIfhd(buffer, memory);
    buffer.Align16();

    WriteCmem(buffer, memory);
    buffer.Align16();

    WriteMall(buffer, memory);
    buffer.Align16();

    WriteStks(buffer, stack);
    buffer.Align16();
}
}

auto WriteIff(const GlulxMemory& memory, const GlulxStack& stack) -> std::vector<uint8_t> {
    GlulxWriteBuffer buffer(memory.GetSize() + stack.GetSize());
    WriteIff(buffer, memory, stack);
    return buffer.GetBuffer();
}

}