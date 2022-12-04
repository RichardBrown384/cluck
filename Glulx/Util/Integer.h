#pragma once

#include <cstdint>

namespace {

constexpr auto CompareEqual(uint32_t x, uint32_t y) -> bool { return x == y; }
constexpr auto CompareNotEqual(uint32_t x, uint32_t y) -> bool { return x != y; }

constexpr auto CompareLessSigned(uint32_t x, uint32_t y) -> bool {
    return static_cast<int32_t>(x) < static_cast<int32_t>(y);
}

constexpr auto CompareGreaterEqualSigned(uint32_t x, uint32_t y) -> bool {
    return !CompareLessSigned(x, y);
}

constexpr auto CompareLessEqualSigned(uint32_t x, uint32_t y) -> bool {
    return static_cast<int32_t>(x) <= static_cast<int32_t>(y);
}

constexpr auto CompareGreaterSigned(uint32_t x, uint32_t y) -> bool {
    return !CompareLessEqualSigned(x, y);
}

constexpr auto CompareLessUnsigned(uint32_t x, uint32_t y) -> bool { return x < y; }
constexpr auto CompareGreaterEqualUnsigned(uint32_t x, uint32_t y) -> bool { return x >= y; }
constexpr auto CompareLessEqualUnsigned(uint32_t x, uint32_t y) -> bool { return x <= y; }
constexpr auto CompareGreaterUnsigned(uint32_t x, uint32_t y) -> bool { return x > y; }

constexpr auto DivideSigned(uint32_t x, uint32_t y) -> uint32_t {
    return static_cast<int32_t>(x) / static_cast<int32_t>(y);
}

constexpr auto ModuloSigned(uint32_t x, uint32_t y) -> uint32_t {
    return static_cast<int32_t>(x) % static_cast<int32_t>(y);
}

}