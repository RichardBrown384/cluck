#include "Impl/GlulxError.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"

#include "Util/MathsUtil.h"
#include "Util/Integer.h"

namespace fiction::glulx {
namespace {

auto ExecuteIntegerUnaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWrite32(glulx, s1, operation(l1.value));
}

auto ExecuteIntegerBinaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    StoreOperandWrite32(glulx, s1, operation(l1.value, l2.value));
}

auto ExecuteIntegerDivisionOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    if (l2.value == 0u) {
        Error("Division by zero");
    }
    StoreOperandWrite32(glulx, s1, operation(l1.value, l2.value));
}
}

auto ExecuteAdd(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x + y; });
}

auto ExecuteSub(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x - y; });
}

auto ExecuteMul(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x * y; });
}

auto ExecuteDiv(GlulxImpl& glulx) -> void {
    ExecuteIntegerDivisionOp(glulx, DivideSigned);
}

auto ExecuteMod(GlulxImpl& glulx) -> void {
    ExecuteIntegerDivisionOp(glulx, ModuloSigned);
}

auto ExecuteNeg(GlulxImpl& glulx) -> void {
    ExecuteIntegerUnaryOp(glulx, [](auto x) { return ~x + 1u; });
}

auto ExecuteBitand(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x & y; });
}

auto ExecuteBitor(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x | y; });
}

auto ExecuteBitxor(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) { return x ^ y; });
}

auto ExecuteBitnot(GlulxImpl& glulx) -> void {
    ExecuteIntegerUnaryOp(glulx, [](auto x) { return ~x; });
}

auto ExecuteShiftl(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) {
        return (y < 32) ? LogicalShiftLeft(x, y) : 0u;
    });
}

auto ExecuteSshiftr(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) {
        const auto shift = (y < 32u) ? y : 31u;
        return ArithmeticShiftRight(x, shift);
    });
}

auto ExecuteUshiftr(GlulxImpl& glulx) -> void {
    ExecuteIntegerBinaryOp(glulx, [](auto x, auto y) {
        return (y < 32) ? LogicalShiftRight(x, y) : 0u;
    });
}

}