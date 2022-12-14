#pragma once

#include <cstdint>

#include "Common/MathsUtil.h"

namespace {

constexpr auto Signed16Cast(uint32_t x) -> int32_t { return static_cast<int32_t>(SignExtend16(x)); }

constexpr auto LessThan(uint32_t x, uint32_t y) -> bool { return Signed16Cast(x) < Signed16Cast(y); }
constexpr auto GreaterThan(uint32_t x, uint32_t y) -> bool { return Signed16Cast(x) > Signed16Cast(y); }

}

