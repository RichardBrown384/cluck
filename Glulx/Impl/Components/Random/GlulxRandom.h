#pragma once

#include <memory>

namespace fiction::glulx {

class GlulxRandom {
public:
    GlulxRandom();
    GlulxRandom(const GlulxRandom&) = delete;
    GlulxRandom(GlulxRandom&&) = delete;
    GlulxRandom& operator=(const GlulxRandom&) = delete;
    GlulxRandom& operator=(GlulxRandom&&) = delete;
    ~GlulxRandom();

    auto SetSeed(uint32_t) -> void;
    [[nodiscard]] auto GetNext(int32_t) const -> int32_t;

    struct Data;

private:
    std::unique_ptr<Data> data;
};

}