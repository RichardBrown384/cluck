#include <cmath>

#include "Impl/GlulxImpl.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Execute/ControlFlow/GlulxCallReturn.h"

#include "Util/FloatingPoint.h"

namespace fiction::glulx {
namespace {
auto ExecuteFloatJumpTernaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3, l4] = OperandsReadLLLL(glulx);
    const auto f1 = BitCastFloat(l1.value);
    const auto f2 = BitCastFloat(l2.value);
    const auto f3 = BitCastFloat(l3.value);
    if (predicate(f1, f2, f3)) {
        Jump(glulx, l4.value);
    }
}

auto ExecuteFloatJumpBinaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    const auto f1 = BitCastFloat(l1.value);
    const auto f2 = BitCastFloat(l2.value);
    if (predicate(f1, f2)) {
        Jump(glulx, l3.value);
    }
}

auto ExecuteFloatJumpUnaryOp(GlulxImpl& glulx, const auto& predicate) {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    const auto f1 = BitCastFloat(l1.value);
    if (predicate(f1)) {
        Jump(glulx, l2.value);
    }
}
}

auto ExecuteJfeq(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpTernaryOp(glulx, Equal<float>);
}

auto ExecuteJfne(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpTernaryOp(glulx, NotEqual<float>);
}

auto ExecuteJflt(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpBinaryOp(glulx, [](float x, float y) { return x < y; });
}

auto ExecuteJfle(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpBinaryOp(glulx, [](float x, float y) { return x <= y; });
}

auto ExecuteJfgt(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpBinaryOp(glulx, [](float x, float y) { return x > y; });
}

auto ExecuteJfge(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpBinaryOp(glulx, [](float x, float y) { return x >= y; });
}

auto ExecuteJisnan(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpUnaryOp(glulx, [](float x) { return std::isnan(x); });
}

auto ExecuteJisinf(GlulxImpl& glulx) -> void {
    ExecuteFloatJumpUnaryOp(glulx, [](float x) { return std::isinf(x); });
}

}