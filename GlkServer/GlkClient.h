#pragma once

#include <cstdint>

namespace fiction::glk {

class GlkClient {
public:
    virtual ~GlkClient() = default;
    virtual auto StackPush32(uint32_t) -> void = 0;
    virtual auto StackPop32() -> uint32_t = 0;
    virtual auto MemoryRead8(uint32_t) -> uint32_t = 0;
    virtual auto MemoryRead32(uint32_t) -> uint32_t = 0;
    virtual auto MemoryWrite8(uint32_t, uint32_t) -> void = 0;
    virtual auto MemoryWrite32(uint32_t, uint32_t) -> void = 0;
};
}