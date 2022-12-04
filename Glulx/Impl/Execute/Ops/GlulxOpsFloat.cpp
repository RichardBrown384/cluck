#include <cmath>

#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Util/FloatingPoint.h"

namespace fiction::glulx {
namespace {
auto ExecuteFloatBinaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    const auto f2 = BitCastFloat(l2.value);
    StoreOperandWriteFloat(glulx, s1, operation(f1, f2));
}

auto ExecuteFloatUnaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    StoreOperandWriteFloat(glulx, s1, operation(f1));
}
}

auto ExecuteNumtof(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWriteFloat(glulx, s1, StaticCastT<float>(l1.value));
}

auto ExecuteFtonumz(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    StoreOperandWrite32(glulx, s1, Truncate(f1));
}

auto ExecuteFtonumn(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    StoreOperandWrite32(glulx, s1, Round(f1));
}

auto ExecuteCeil(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::ceilf(x); });
}

auto ExecuteFloor(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::floorf(x); });
}

auto ExecuteFadd(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return x + y; });
}

auto ExecuteFsub(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return x - y; });
}

auto ExecuteFmul(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return x * y; });
}

auto ExecuteFdiv(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return x / y; });
}

auto ExecuteFmod(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1, s2] = OperandsReadLLSS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    const auto f2 = BitCastFloat(l2.value);
    const auto remainder = Remainder(f1, f2);
    const auto quotient = Quotient(f1, f2);
    StoreOperandWriteFloat(glulx, s1, remainder);
    StoreOperandWriteFloat(glulx, s2, quotient);
}

auto ExecuteSqrt(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::sqrtf(x); });
}

auto ExecuteExp(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::expf(x); });
}

auto ExecuteLog(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::logf(x); });
}

auto ExecutePow(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return std::powf(x, y); });
}

auto ExecuteSin(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::sinf(x); });
}

auto ExecuteCos(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::cosf(x); });
}

auto ExecuteTan(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::tanf(x); });
}

auto ExecuteAsin(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::asin(x); });
}

auto ExecuteAcos(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::acos(x); });
}

auto ExecuteAtan(GlulxImpl& glulx) -> void {
    ExecuteFloatUnaryOp(glulx, [](float x) { return std::atan(x); });
}

auto ExecuteAtan2(GlulxImpl& glulx) -> void {
    ExecuteFloatBinaryOp(glulx, [](float x, float y) { return std::atan2f(x, y); });
}

}