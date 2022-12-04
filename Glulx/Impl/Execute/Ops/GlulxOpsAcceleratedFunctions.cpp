#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteAccelfunc(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.SetAccelerationFunction(l1.value, l2.value);
}

auto ExecuteAccelparam(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.SetAccelerationParam(l1.value, l2.value);
}

}