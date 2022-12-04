#include <cmath>

#include "Impl/GlulxImpl.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Execute/ControlFlow/GlulxCallReturn.h"

#include "Util/FloatingPoint.h"

namespace fiction::glulx {
namespace {
auto ExecuteDoubleJumpTernaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3, l4, l5, l6, l7] = OperandsReadLLLLLLL(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    const auto d2 = BitCastDouble(l3.value, l4.value);
    const auto d3 = BitCastDouble(l5.value, l6.value);
    if (predicate(d1, d2, d3)) {
        Jump(glulx, l7.value);
    }
}

auto ExecuteDoubleJumpBinaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3, l4, l5] = OperandsReadLLLLL(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    const auto d2 = BitCastDouble(l3.value, l4.value);
    if (predicate(d1, d2)) {
        Jump(glulx, l5.value);
    }
}

auto ExecuteDoubleJumpUnaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    if (predicate(d1)) {
        Jump(glulx, l3.value);
    }
}
}

auto ExecuteJdeq(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpTernaryOp(glulx, Equal<double>);
}

auto ExecuteJdne(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpTernaryOp(glulx, NotEqual<double>);
}

auto ExecuteJdlt(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpBinaryOp(glulx, [](double x, double y) { return x < y; });
}

auto ExecuteJdle(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpBinaryOp(glulx, [](double x, double y) { return x <= y; });
}

auto ExecuteJdgt(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpBinaryOp(glulx, [](double x, double y) { return x > y; });
}

auto ExecuteJdge(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpBinaryOp(glulx, [](double x, double y) { return x >= y; });
}

auto ExecuteJdisnan(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpUnaryOp(glulx, [](double x) { return std::isnan(x); });
}

auto ExecuteJdisinf(GlulxImpl& glulx) -> void {
    ExecuteDoubleJumpUnaryOp(glulx, [](double x) { return std::isinf(x); });
}

}