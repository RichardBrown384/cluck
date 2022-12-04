#include "Impl/GlulxImpl.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteMalloc(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto result = glulx.Malloc(l1.value);
    StoreOperandWrite32(glulx, s1, result);
}

auto ExecuteMfree(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    glulx.Free(l1.value);
}

}