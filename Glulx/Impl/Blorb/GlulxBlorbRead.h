#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

auto ExtractGlulExecutable(const std::vector<uint8_t>&) -> std::vector<uint8_t>;

}