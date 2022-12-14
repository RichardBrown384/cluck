#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace fiction::zork {

class ZorkImpl;

class Zork {
public:
    explicit Zork(const std::vector<uint8_t>&);
    Zork(const Zork&) = delete;
    Zork(Zork&&) = delete;
    Zork& operator=(const Zork&) = delete;
    Zork& operator=(Zork&&) = delete;
    ~Zork();

    [[nodiscard]] auto IsRunning() const -> bool;

    auto Restart() -> void;
    auto Execute() -> void;

private:
    std::unique_ptr<ZorkImpl> impl;
};

}