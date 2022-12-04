#include "Impl/GlulxImpl.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteGetmemsize(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    StoreOperandWrite32(glulx, s1, glulx.GetMemSize());
}

auto ExecuteSetmemsize(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto result = glulx.SetMemSize(l1.value);
    StoreOperandWrite32(glulx, s1, result ? GLULX_SUCCESS : GLULX_FAILURE);
}

}