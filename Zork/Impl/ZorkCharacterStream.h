#pragma once

#include <cstdint>

namespace fiction::zork {

class ZorkImpl;

class ZorkCharacterStream {
public:
    ZorkCharacterStream(ZorkImpl&, uint32_t);
    ZorkCharacterStream(const ZorkCharacterStream&) = delete;
    ZorkCharacterStream(ZorkCharacterStream&&) = delete;
    ZorkCharacterStream& operator=(const ZorkCharacterStream&) = delete;
    ZorkCharacterStream& operator=(ZorkCharacterStream&&) = delete;

    [[nodiscard]] auto GetAddress() const -> uint32_t;
    [[nodiscard]] auto Done() const -> bool;

    auto Next() -> uint32_t;
private:

    ZorkImpl& impl;
    bool done;
    uint32_t address;
    uint32_t buffer;
    uint32_t remaining;
};
}