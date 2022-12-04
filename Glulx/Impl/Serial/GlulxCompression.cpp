#include "GlulxCompression.h"

namespace fiction::glulx {

auto CompressBuffer(const std::vector<uint8_t>& buffer) -> std::vector<uint8_t> {
    std::vector<uint8_t> compressed;
    compressed.reserve(buffer.size());
    for (auto i = 0u, count = 0u; i < buffer.size(); ++i) {
        const auto c = buffer[i];
        if (c == 0u) {
            ++count;
        } else {
            while (count > 0x100u) {
                compressed.push_back(0u);
                compressed.push_back(0xFFu);
                count -= 0x100u;
            }
            if (count > 0u) {
                compressed.push_back(0u);
                compressed.push_back(count - 1u);
                count = 0u;
            }
            compressed.push_back(c);
        }
    }
    compressed.shrink_to_fit();
    return compressed;
}

auto DecompressBuffer(const std::vector<uint8_t>& buffer, uint32_t size) -> std::vector<uint8_t> {
    std::vector<uint8_t> decompressed(size);
    for (auto i = 0u, j = 0u; i < buffer.size();) {
        const auto c = buffer[i++];
        if (c == 0u) {
            const auto count = buffer[i++] + 1u;
            j += count;
        } else {
            decompressed[j++] = c;
        }
    }
    return decompressed;
}

}