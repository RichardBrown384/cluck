#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace fiction::filesystem {
auto LoadFile(const std::filesystem::path& filename) -> std::vector<uint8_t>;
}