#include <unordered_map>

#include "GlulxInputOutputSystems.h"
#include "GlulxNullInputOutputSystem.h"
#include "GlulxFilterInputOutputSystem.h"
#include "GlulxInputOutputSystem.h"

namespace fiction::glulx {

using NullSystem = GlulxNullInputOutputSystem;
using FilterSystem = GlulxFilterInputOutputSystem;

struct GlulxInputOutputSystems::Data {
    Data();
    uint32_t system;
    uint32_t rock;
    NullSystem null;
    FilterSystem filter;
    std::unordered_map<uint32_t, GlulxInputOutputSystem&> systems;
};

GlulxInputOutputSystems::Data::Data() :
    system(0u),
    rock(0u),
    null(),
    filter(),
    systems({
        { IO_SYSTEM_NULL, null },
        { IO_SYSTEM_FILTER , filter }
    }) {}

GlulxInputOutputSystems::GlulxInputOutputSystems() : data(std::make_unique<Data>()) {}

GlulxInputOutputSystems::~GlulxInputOutputSystems() = default;

auto GlulxInputOutputSystems::GetInputOutputSystem() const -> uint32_t {
    return data->system;
}

auto GlulxInputOutputSystems::GetInputOutputRock() const -> uint32_t {
    return data->rock;
}

auto GlulxInputOutputSystems::IsInputOutputSystemSupported(uint32_t system) const -> bool {
    return data->systems.contains(system);
}

auto GlulxInputOutputSystems::GetActiveInputOutputSystem() -> GlulxInputOutputSystem& {
    return data->systems.at(data->system);
}

auto GlulxInputOutputSystems::SetInputOutputSystemAndRock(uint32_t system, uint32_t rock) -> void {
    data->system = system;
    data->rock = rock;
}

auto GlulxInputOutputSystems::SetInputOutputSystem(uint32_t id, GlulxInputOutputSystem& system) -> void {
    data->systems.emplace(id, system);
}

}