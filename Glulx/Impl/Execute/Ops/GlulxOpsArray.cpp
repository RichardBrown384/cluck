#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Common/MathsUtil.h"

namespace fiction::glulx {

auto ExecuteAload(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto v = glulx.MemoryRead32(l1.value + 4u * l2.value);
    StoreOperandWrite32(glulx, s1, v);
}

auto ExecuteAloads(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto v = glulx.MemoryRead16(l1.value + 2u * l2.value);
    StoreOperandWrite32(glulx, s1, v);
}

auto ExecuteAloadb(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto v = glulx.MemoryRead8(l1.value + l2.value);
    StoreOperandWrite32(glulx, s1, v);
}

auto ExecuteAloadbit(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto bit = l2.value & 0x7u;
    const auto mask = LogicalShiftLeft(1u, bit);
    const auto offset = ArithmeticShiftRight(l2.value, 3u);
    const auto v = glulx.MemoryRead8(l1.value + offset);
    StoreOperandWrite32(glulx, s1, (v & mask) ? 1u : 0u);
}

auto ExecuteAstore(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    glulx.MemoryWrite32(l1.value + 4u * l2.value, l3.value);
}

auto ExecuteAstores(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    glulx.MemoryWrite16(l1.value + 2u * l2.value, l3.value);
}

auto ExecuteAstoreb(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    glulx.MemoryWrite8(l1.value + l2.value, l3.value);
}

auto ExecuteAstorebit(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    const auto bit = l2.value & 0x7u;
    const auto mask = LogicalShiftLeft(1u, bit);
    const auto offset = ArithmeticShiftRight(l2.value, 3u);
    const auto v = glulx.MemoryRead8(l1.value + offset);
    const auto s = (l3.value != 0) ? v | mask : v & ~mask;
    glulx.MemoryWrite8(l1.value + offset, s);
}

}