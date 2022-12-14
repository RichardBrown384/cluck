#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Common/MathsUtil.h"

namespace fiction::glulx {

auto ExecuteCopy(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWrite32(glulx, s1, l1.value);
}

auto ExecuteCopys(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadL16S(glulx);
    StoreOperandWrite16(glulx, s1, l1.value);
}

auto ExecuteCopyb(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadL8S(glulx);
    StoreOperandWrite8(glulx, s1, l1.value);
}

auto ExecuteSexs(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWrite32(glulx, s1, SignExtend16(l1.value));
}

auto ExecuteSexb(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWrite32(glulx, s1, SignExtend8(l1.value));
}

}