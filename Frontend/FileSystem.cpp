#include "FileSystem.h"

#include <cstdint>
#include <vector>
#include <fstream>

namespace fiction::filesystem {
auto LoadFile(const std::filesystem::path& filename) -> std::vector<uint8_t> {
    std::vector<uint8_t> bytes;
    std::ifstream input(filename, std::ios::binary);
    if (input) {
        bytes.insert(bytes.end(),
                     std::istreambuf_iterator<char>(input),
                     std::istreambuf_iterator<char>()
        );
    }
    return bytes;
}
}