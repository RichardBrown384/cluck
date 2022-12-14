#pragma once

#include <cstdint>
#include <vector>

namespace fiction::zork {

struct ZorkHeader;

class ZorkMemory {
public:
    ZorkMemory(std::vector<uint8_t>, const ZorkHeader&);
    ZorkMemory(const ZorkMemory&) = delete;
    ZorkMemory(ZorkMemory&&) = delete;
    ZorkMemory& operator=(const ZorkMemory&) = delete;
    ZorkMemory& operator=(ZorkMemory&&) = delete;
    ~ZorkMemory();

    auto Restart() -> void;

    [[nodiscard]] auto Read8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto Read16(uint32_t) const -> uint32_t;

    auto Write8(uint32_t, uint32_t) -> void;
    auto Write16(uint32_t, uint32_t) -> void;

private:
    const ZorkHeader& header;
    std::vector<uint8_t> memory;
    const std::vector<uint8_t> original;

};
}