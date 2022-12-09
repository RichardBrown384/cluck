#pragma once

#include <cstdint>
#include <vector>

namespace fiction::endian {
constexpr auto PackBigEndian16(uint32_t a, uint32_t b) -> uint32_t {
    return (a << 8) + b;
}

constexpr auto PackBigEndian32(uint32_t a, uint32_t b, uint32_t c, uint32_t d) -> uint32_t{
    return (a << 24) + (b << 16) + (c << 8) + d;
}

auto Read8(const std::vector<uint8_t>&, uint32_t) -> uint32_t;
auto Read16(const std::vector<uint8_t>&, uint32_t) -> uint32_t;
auto Read32(const std::vector<uint8_t>&, uint32_t) -> uint32_t;

auto Write8(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
auto Write16(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
auto Write32(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
}