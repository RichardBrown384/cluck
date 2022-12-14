#pragma once

#include <memory>

namespace fiction::zork {

class ZorkRandom {
public:
    ZorkRandom();
    ZorkRandom(const ZorkRandom&) = delete;
    ZorkRandom(ZorkRandom&&) = delete;
    ZorkRandom& operator=(const ZorkRandom&) = delete;
    ZorkRandom& operator=(ZorkRandom&&) = delete;
    ~ZorkRandom();

    [[nodiscard]] auto GetNext(int32_t) -> int32_t;

    struct Data;

private:
    auto SetSeed(uint32_t) -> void;
    auto GetRandomDevice() -> uint32_t;
    auto GetNext() -> int32_t;

    std::unique_ptr<Data> data;
};

}