#include "Impl/GlulxImpl.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Execute/ControlFlow/GlulxCallReturn.h"

namespace fiction::glulx {
namespace {
auto Call(GlulxImpl& glulx,
          const GlulxOperand& l1,
          const GlulxOperand& s1,
          const std::vector<uint32_t>& arguments) {
    Call(glulx, l1.value, s1.desttype, s1.value, arguments);
}
}

auto ExecuteCall(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    Call(glulx, l1, s1, glulx.StackPopArguments(l2.value));
}

auto ExecuteCallf(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    Call(glulx, l1, s1, {});
}

auto ExecuteCallfi(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    Call(glulx, l1, s1, { l2.value });
}

auto ExecuteCallfii(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3, s1] = OperandsReadLLLS(glulx);
    Call(glulx, l1, s1, { l2.value, l3.value });
}

auto ExecuteCallfiii(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3, l4, s1] = OperandsReadLLLLS(glulx);
    Call(glulx, l1, s1, { l2.value, l3.value, l4.value });
}

auto ExecuteReturn(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    Return(glulx, l1.value);
}

auto ExecuteTailcall(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    TailCall(glulx, l1.value, glulx.StackPopArguments(l2.value));
}

}