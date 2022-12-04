#pragma once

#include <memory>

namespace fiction::glulx {

class GlulxImpl;

class GlulxAcceleration {
public:
    GlulxAcceleration();
    GlulxAcceleration(const GlulxAcceleration&) = delete;
    GlulxAcceleration(GlulxAcceleration&&) = delete;
    GlulxAcceleration& operator=(const GlulxAcceleration&) = delete;
    GlulxAcceleration& operator=(GlulxAcceleration&&) = delete;
    ~GlulxAcceleration();

    struct Data;

    auto SetParam(uint32_t, uint32_t) -> void;
    [[nodiscard]] auto GetParam(uint32_t) const -> uint32_t;

    [[nodiscard]] auto IsFunctionSupported(uint32_t) const -> bool;

    auto SetFunction(GlulxImpl&, uint32_t, uint32_t) -> void;
    auto CallFunction(GlulxImpl&, uint32_t, const std::vector<uint32_t>&, uint32_t*) -> bool;

    auto& GetData() { return *data; }
    const auto& GetData() const { return *data; }

private:
    std::unique_ptr<Data> data;
};

}