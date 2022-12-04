#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace fiction::glulx {

class GlulxImpl;
class GlulxInputOutputSystem;

class Glulx {
public:
    explicit Glulx(std::vector<uint8_t>);
    Glulx(const Glulx&) = delete;
    Glulx(Glulx&&) = delete;
    Glulx& operator=(const Glulx&) = delete;
    Glulx& operator=(Glulx&&) = delete;
    ~Glulx();

    [[nodiscard]] auto IsFileValid() const -> bool;

    auto SetInputOutputSystem(uint32_t, GlulxInputOutputSystem&) -> void;
    auto SetServerFunction(std::function<uint32_t(uint32_t, const std::vector<uint32_t>&)>) -> void;

    [[nodiscard]] auto IsRunning() const -> bool;
    auto Restart() -> void;
    auto Execute() -> void;

    auto StackPush32(uint32_t) -> void;
    auto StackPop32() -> uint32_t;

    [[nodiscard]] auto MemoryRead8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto MemoryRead32(uint32_t) const -> uint32_t;

    auto MemoryWrite8(uint32_t, uint32_t) -> void;
    auto MemoryWrite32(uint32_t, uint32_t) -> void;

private:
    std::unique_ptr<GlulxImpl> impl;
};

}