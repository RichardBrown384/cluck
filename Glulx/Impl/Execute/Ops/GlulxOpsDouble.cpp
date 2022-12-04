#include <cmath>

#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Util/FloatingPoint.h"

namespace fiction::glulx {
namespace {
auto ExecuteDoubleBinaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, l2, l3, l4, s1, s2] = OperandsReadLLLLSS(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    const auto d2 = BitCastDouble(l3.value, l4.value);
    StoreOperandWriteDouble(glulx, s1, s2, operation(d1, d2));
}

auto ExecuteDoubleUnaryOp(GlulxImpl& glulx, const auto& operation) {
    const auto& [l1, l2, s1, s2] = OperandsReadLLSS(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    StoreOperandWriteDouble(glulx, s1, s2, operation(d1));
}
}

auto ExecuteNumtod(GlulxImpl& glulx) -> void {
    const auto& [l1, s1, s2] = OperandsReadLSS(glulx);
    StoreOperandWriteDouble(glulx, s1, s2, StaticCastT<double>(l1.value));
}

auto ExecuteDtonumz(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    StoreOperandWrite32(glulx, s1, Truncate(d1));
}

auto ExecuteDtonumn(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    StoreOperandWrite32(glulx, s1, Round(d1));
}

auto ExecuteFtod(GlulxImpl& glulx) -> void {
    const auto& [l1, s1, s2] = OperandsReadLSS(glulx);
    const auto f1 = BitCastFloat(l1.value);
    StoreOperandWriteDouble(glulx, s1, s2, f1);
}

auto ExecuteDtof(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, s1] = OperandsReadLLS(glulx);
    const auto d1 = BitCastDouble(l1.value, l2.value);
    StoreOperandWriteFloat(glulx, s1, static_cast<float>(d1));
}

auto ExecuteDceil(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::ceil(x); });
}

auto ExecuteDfloor(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::floor(x); });
}

auto ExecuteDadd(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return x + y; });
}

auto ExecuteDsub(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return x - y; });
}

auto ExecuteDmul(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return x * y; });
}

auto ExecuteDdiv(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return x / y; });
}

auto ExecuteDmodr(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, Remainder<double>);
}

auto ExecuteDmodq(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, Quotient<double>);
}

auto ExecuteDsqrt(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::sqrt(x); });
}

auto ExecuteDexp(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::exp(x); });
}

auto ExecuteDlog(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::log(x); });
}

auto ExecuteDpow(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return std::pow(x, y); });
}

auto ExecuteDsin(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::sin(x); });
}

auto ExecuteDcos(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::cos(x); });
}

auto ExecuteDtan(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::tan(x); });
}

auto ExecuteDasin(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::asin(x); });
}

auto ExecuteDacos(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::acos(x); });
}

auto ExecuteDatan(GlulxImpl& glulx) -> void {
    ExecuteDoubleUnaryOp(glulx, [](double x) { return std::atan(x); });
}

auto ExecuteDatan2(GlulxImpl& glulx) -> void {
    ExecuteDoubleBinaryOp(glulx, [](double x, double y) { return std::atan2(x, y); });
}

}