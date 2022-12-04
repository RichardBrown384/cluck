#pragma once

#include <cmath>

#include "BitCast.h"

namespace {

// Note: These constants are taken from the reference interpreter
// The lower-bound in might be problematic in some cases since truncating -2147483648.15
// should fit within the range of a two's complement signed integer
constexpr auto FLOAT_NEAREST_INTEGER_MIN = -2147483647.0;
constexpr auto FLOAT_NEAREST_INTEGER_MAX = 2147483647.0;

constexpr auto MIN_INT = 0x80000000u;
constexpr auto MAX_INT = 0x7FFFFFFFu;

inline auto BitCastFloat(uint32_t x) { return BitCast<float>(x); }

inline auto BitCastDouble(uint32_t x, uint32_t y) {
    return BitCast<double>((static_cast<uint64_t>(x) << 32u) + y);
}

template<typename T>
constexpr auto StaticCastT(uint32_t x) -> T {
    return static_cast<T>(static_cast<int32_t>(x));
}

template<typename T>
auto Equal(T x, T y, T epsilon) {
    if (std::isnan(epsilon)) {
        return false;
    }
    if (std::isinf(x) && std::isinf(y)) {
        return std::signbit(x) == std::signbit(y);
    }
    return std::fabs(x - y) <= std::abs(epsilon);
}

template<typename T>
auto NotEqual(T x, T y, T epsilon) {
    return !Equal(x, y, epsilon);
}

template<typename T>
auto OutsideIntRange(T t) {
    return std::isnan(t) || std::isinf(t) || t < FLOAT_NEAREST_INTEGER_MIN || t > FLOAT_NEAREST_INTEGER_MAX;
}

template<typename T>
auto OutsideRangeSignal(T t) { return std::signbit(t) ? MIN_INT : MAX_INT; }

template<typename T>
auto Truncate(T t) -> uint32_t {
    return OutsideIntRange(t) ? OutsideRangeSignal(t) : static_cast<int32_t>(std::trunc(t));
}

template<typename T>
auto Round(T t) -> uint32_t {
    return OutsideIntRange(t) ? OutsideRangeSignal(t) : static_cast<int32_t>(std::round(t));
}

template<typename T>
auto Quotient(T x, T y) {
    const auto remainder = std::fmod(x, y);
    const auto quotient = (x - remainder) / y;
    if (std::fpclassify(quotient) == FP_ZERO) {
        const auto abs = std::fabs(quotient);
        return std::signbit(x) ^ std::signbit(y) ? -abs : abs;
    }
    return quotient;
}

template<typename T>
auto Remainder(T x, T y) {
    return std::fmod(x, y);
}

}