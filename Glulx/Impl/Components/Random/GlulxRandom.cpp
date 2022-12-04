#include <random>
#include <cstdint>

#include "GlulxRandom.h"

namespace fiction::glulx {

using Limits = std::numeric_limits<int32_t>;

struct GlulxRandom::Data {
    Data();
    std::random_device randomdevice;
    std::mt19937 engine;
    std::uniform_int_distribution<int32_t> distribution;
};

GlulxRandom::Data::Data() :
    randomdevice(),
    engine(randomdevice()),
    distribution(Limits::min(), Limits::max()) {}

GlulxRandom::GlulxRandom() :
    data(std::make_unique<Data>()) {}

GlulxRandom::~GlulxRandom() = default;

auto GlulxRandom::SetSeed(uint32_t seed) -> void {
    data->engine.seed(seed == 0u ? data->randomdevice() : seed);
}

auto GlulxRandom::GetNext(int32_t modulus) const -> int32_t {
    const auto next = data->distribution(data->engine);
    if (modulus == 0) {
        return next;
    }
    // Note: C++11 says that sign(a % b) = sign(a)
    // However we want the result to have the same sign as b (the modulus)
    const auto absolute = next < 0 ? -next : next;
    const auto ranged = absolute % modulus;
    return modulus < 0 ? -ranged : ranged;
}

}