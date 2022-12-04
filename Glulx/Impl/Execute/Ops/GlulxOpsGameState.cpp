#include "Impl/GlulxImpl.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "Impl/Execute/ControlFlow/GlulxCallStub.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteQuit(GlulxImpl& glulx) -> void {
    glulx.SetRunning(false);
}

auto ExecuteVerify(GlulxImpl& glulx) -> void {
    // Note: we validate on startup
    const auto& [s1] = OperandsReadS(glulx);
    StoreOperandWrite32(glulx, s1, GLULX_SUCCESS);
}

auto ExecuteRestart(GlulxImpl& glulx) -> void {
    glulx.Restart();
}

auto ExecuteSave(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    PushCallStub(glulx, s1.desttype, s1.value);
    const auto result = glulx.GameStateSave(l1.value);
    PopCallStub(glulx, result ? GLULX_SUCCESS : GLULX_FAILURE);
}

auto ExecuteRestore(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    if (glulx.GameStateRestore(l1.value)) {
        PopCallStub(glulx, GLULX_RESTORED);
        return;
    }
    StoreOperandWrite32(glulx, s1, GLULX_FAILURE);
}

auto ExecuteSaveundo(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    PushCallStub(glulx, s1.desttype, s1.value);
    const auto result = glulx.GameStateSaveUndo();
    PopCallStub(glulx, result ? GLULX_SUCCESS : GLULX_FAILURE);
}

auto ExecuteRestoreundo(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    if (glulx.GameStateRestoreUndo()) {
        PopCallStub(glulx, GLULX_RESTORED);
        return;
    }
    StoreOperandWrite32(glulx, s1, GLULX_FAILURE);
}

auto ExecuteProtect(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.SetProtect(l1.value, l2. value);
}

auto ExecuteHasundo(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    const auto result = glulx.GameStateHasUndo();
    StoreOperandWrite32(glulx, s1, result ? GLULX_SUCCESS : GLULX_FAILURE);
}

auto ExecuteDiscardundo(GlulxImpl& glulx) -> void {
    glulx.GameStateDiscardUndo();
}

}