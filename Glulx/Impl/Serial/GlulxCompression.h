#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

auto CompressBuffer(const std::vector<uint8_t>&) -> std::vector<uint8_t>;

auto DecompressBuffer(const std::vector<uint8_t>&, uint32_t) -> std::vector<uint8_t>;

}