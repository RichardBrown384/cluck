#pragma once

#include "GlulxInputOutputSystem.h"

namespace fiction::glulx {

class GlulxFilterInputOutputSystem : public GlulxInputOutputSystem {

public:
    ~GlulxFilterInputOutputSystem() override = default;
    [[nodiscard]] auto IsFiltering() const -> bool override { return true; }
    auto PutChar8(GlulxImpl&, uint32_t) -> void override;
    auto PutChar32(GlulxImpl&, uint32_t) -> void override;
    auto PutBuffer8(GlulxImpl&, uint32_t, const std::vector<uint8_t>&) -> void override { }
    auto GetBuffer8(GlulxImpl&, uint32_t) -> std::vector<uint8_t> override { return {}; }
};

}