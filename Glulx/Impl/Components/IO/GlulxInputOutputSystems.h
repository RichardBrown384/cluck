#pragma once

#include <memory>

namespace fiction::glulx {

class GlulxInputOutputSystem;

class GlulxInputOutputSystems {
public:
    GlulxInputOutputSystems();
    GlulxInputOutputSystems(const GlulxInputOutputSystems&) = delete;
    GlulxInputOutputSystems(GlulxInputOutputSystems&&) = delete;
    GlulxInputOutputSystems& operator=(const GlulxInputOutputSystems&) = delete;
    GlulxInputOutputSystems& operator=(GlulxInputOutputSystems&&) = delete;
    ~GlulxInputOutputSystems();

    [[nodiscard]] auto GetInputOutputSystem() const -> uint32_t;
    [[nodiscard]] auto GetInputOutputRock() const -> uint32_t;
    [[nodiscard]] auto IsInputOutputSystemSupported(uint32_t) const -> bool;

    auto GetActiveInputOutputSystem() -> GlulxInputOutputSystem&;

    auto SetInputOutputSystemAndRock(uint32_t, uint32_t) -> void;
    auto SetInputOutputSystem(uint32_t, GlulxInputOutputSystem&) -> void;
    
    struct Data;

private:
    std::unique_ptr<Data> data;
};
}