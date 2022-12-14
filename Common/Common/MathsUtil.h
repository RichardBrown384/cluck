#pragma once

#include <cstdint>

namespace {

constexpr auto LogicalShiftLeft(uint32_t x, uint32_t n) -> uint32_t { return x << n; }
constexpr auto LogicalShiftRight(uint32_t x, uint32_t n) -> uint32_t { return x >> n; }
constexpr auto ArithmeticShiftRight(uint32_t x, uint32_t n) -> uint32_t {
    return ((x ^ 0x80000000u) >> n) - (0x80000000u >> n);
}

constexpr auto SignExtend(uint32_t x, uint32_t n) -> uint32_t {
    const auto bit = 1u << (n - 1u);
    const auto mask = (1u << n) - 1u;
    return ((x & mask) ^ bit) - bit;
}

constexpr auto SignExtend8(uint32_t x) { return SignExtend(x, 8u); }
constexpr auto SignExtend16(uint32_t x) { return SignExtend(x, 16u); }

constexpr auto ComputeAlignmentPadding(uint32_t x, uint32_t size) {
    // Note: Assumes size is a power of 2 (and non-zero)
    return (size - x) & (size - 1u);
}

}