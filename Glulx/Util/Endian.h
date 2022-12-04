#pragma once

#include <cstdint>
#include <vector>

namespace fiction::endian {
auto PackBigEndian16(uint32_t, uint32_t) -> uint32_t ;
auto PackBigEndian32(uint32_t, uint32_t, uint32_t, uint32_t) -> uint32_t;

auto Read8(const std::vector<uint8_t>&, uint32_t) -> uint32_t;
auto Read16(const std::vector<uint8_t>&, uint32_t) -> uint32_t;
auto Read32(const std::vector<uint8_t>&, uint32_t) -> uint32_t;

auto Write8(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
auto Write16(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
auto Write32(std::vector<uint8_t>&, uint32_t, uint32_t) -> void;
}