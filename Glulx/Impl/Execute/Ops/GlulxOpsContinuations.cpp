#include "Impl/GlulxImpl.h"

#include "Impl/Execute/ControlFlow/GlulxCallStub.h"
#include "Impl/Execute/ControlFlow/GlulxCallReturn.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteCatch(GlulxImpl& glulx) -> void {
    const auto& [s1, l1] = OperandsReadSL(glulx);
    PushCallStub(glulx, s1.desttype, s1.value);
    StoreOperandWrite32(glulx, s1, glulx.GetStackPointer());
    Jump(glulx, l1.value);
}

auto ExecuteThrow(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.SetStackPointer(l2.value);
    PopCallStub(glulx, l1.value);
}

}