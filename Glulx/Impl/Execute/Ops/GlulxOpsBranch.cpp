#include "Impl/GlulxImpl.h"

#include "Impl/Execute/ControlFlow/GlulxCallReturn.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

#include "Util/Integer.h"

namespace fiction::glulx {
namespace {
auto ExecuteIntegerJumpBinaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    if (predicate(l1.value, l2.value)) {
        Jump(glulx, l3.value);
    }
}

auto ExecuteIntegerJumpUnaryOp(GlulxImpl& glulx, auto predicate) {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    if (predicate(l1.value)) {
        Jump(glulx, l2.value);
    }
}
}

auto ExecuteJump(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    Jump(glulx, l1.value);
}

auto ExecuteJz(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpUnaryOp(glulx, [](auto x) { return x == 0u; });
}

auto ExecuteJnz(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpUnaryOp(glulx, [](auto x) { return x != 0u; });
}

auto ExecuteJeq(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareEqual);
}

auto ExecuteJne(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareNotEqual);
}

auto ExecuteJlt(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareLessSigned);
}

auto ExecuteJge(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareGreaterEqualSigned);
}

auto ExecuteJgt(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareGreaterSigned);
}

auto ExecuteJle(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareLessEqualSigned);
}

auto ExecuteJltu(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareLessUnsigned);
}

auto ExecuteJgeu(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareGreaterEqualUnsigned);
}

auto ExecuteJgtu(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareGreaterUnsigned);
}

auto ExecuteJleu(GlulxImpl& glulx) -> void {
    ExecuteIntegerJumpBinaryOp(glulx, CompareLessEqualUnsigned);
}

auto ExecuteJumpabs(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    glulx.SetProgramCounter(l1.value);
}

}