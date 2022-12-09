#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace fiction::glk {

class GlkServerImpl;
class GlkClient;

class GlkServer {
public:
    GlkServer(GlkClient&, const std::vector<uint8_t>&);
    GlkServer(const GlkServer&) = delete;
    GlkServer(GlkServer&&) = delete;
    GlkServer& operator=(GlkServer&) = delete;
    GlkServer& operator=(const GlkServer&&) = delete;
    ~GlkServer();

    auto Tick() -> void;
    auto PutChar8(uint32_t) -> void;
    auto PutChar32(uint32_t) -> void;
    auto PutBuffer8(uint32_t, const std::vector<uint8_t>&) -> void;
    auto GetBuffer8(uint32_t) -> std::vector<uint8_t>;

    auto Call(uint32_t, const std::vector<uint32_t>&) -> uint32_t;

private:
    auto& GetImpl() { return *impl; }

    std::unique_ptr<GlkServerImpl> impl;
};

}