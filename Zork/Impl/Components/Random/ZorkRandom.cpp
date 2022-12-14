#include <random>
#include <cstdint>

#include "ZorkRandom.h"

namespace fiction::zork {

using Limits = std::numeric_limits<int32_t>;

struct ZorkRandom::Data {
    Data();
    std::random_device randomdevice;
    std::mt19937 engine;
    std::uniform_int_distribution<int32_t> distribution;
};

ZorkRandom::Data::Data() :
    randomdevice(),
    engine(randomdevice()),
    distribution(0, Limits::max()) {}

ZorkRandom::ZorkRandom() :
    data(std::make_unique<Data>()) {}

ZorkRandom::~ZorkRandom() = default;

auto ZorkRandom::SetSeed(uint32_t seed) -> void { data->engine.seed(seed); }
auto ZorkRandom::GetRandomDevice() -> uint32_t { return data->randomdevice(); }
auto ZorkRandom::GetNext() -> int32_t { return data->distribution(data->engine); }

auto ZorkRandom::GetNext(int32_t modulus) -> int32_t {
    if (modulus < 0) {
        SetSeed(-modulus);
        return 0;
    } else if (modulus == 0) {
        SetSeed(GetRandomDevice());
        return 0;
    }
    return 1 + (GetNext() % modulus);
}

}