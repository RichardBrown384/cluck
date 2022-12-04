#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

constexpr auto IO_SYSTEM_NULL = 0u;
constexpr auto IO_SYSTEM_FILTER = 1u;
constexpr auto IO_SYSTEM_GLK = 2u;

class GlulxImpl;

class GlulxInputOutputSystem {
public:
    virtual ~GlulxInputOutputSystem() = default;
    [[nodiscard]] virtual auto IsFiltering() const -> bool { return false; }
    virtual auto PutChar8(GlulxImpl&, uint32_t) -> void = 0;
    virtual auto PutChar32(GlulxImpl&, uint32_t) -> void = 0;
    virtual auto PutBuffer8(GlulxImpl&, uint32_t, const std::vector<uint8_t>&) -> void = 0;
    virtual auto GetBuffer8(GlulxImpl&, uint32_t) -> std::vector<uint8_t> = 0;
};
}