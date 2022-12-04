#pragma once

#include "GlkClient.h"

namespace fiction::glulx {
class Glulx;
}

class GlkGlulxClient : public fiction::glk::GlkClient {
public:
    explicit GlkGlulxClient(fiction::glulx::Glulx&);
    ~GlkGlulxClient() override = default;
    auto StackPush32(uint32_t) -> void override;
    auto StackPop32() -> uint32_t override;
    auto MemoryRead8(uint32_t) -> uint32_t override;
    auto MemoryRead32(uint32_t) -> uint32_t override;
    auto MemoryWrite8(uint32_t, uint32_t) -> void override;
    auto MemoryWrite32(uint32_t, uint32_t) -> void override;
private:
    fiction::glulx::Glulx& glulx;
};