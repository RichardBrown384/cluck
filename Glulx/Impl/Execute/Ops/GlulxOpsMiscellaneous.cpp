#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Components/Gestalt/GlulxGestalt.h"

namespace fiction::glulx {

auto ExecuteNop(GlulxImpl&) -> void {}

auto ExecuteGestalt(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    StoreOperandWrite32(glulx, s1, Gestalt(glulx, l1.value, l2.value));
}

auto ExecuteDebugtrap(GlulxImpl&) -> void { Error("Debug trap"); }

auto ExecuteGlk(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto arguments = glulx.StackPopArguments(l2.value);
    const auto result = glulx.CallServer(l1.value, arguments);
    StoreOperandWrite32(glulx, s1, result);
}

}