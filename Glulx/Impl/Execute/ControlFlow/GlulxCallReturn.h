#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxImpl;

auto Call(GlulxImpl&, uint32_t, uint32_t, uint32_t, const std::vector<uint32_t>&) -> void;

auto TailCall(GlulxImpl&, uint32_t, const std::vector<uint32_t>&) -> void;

auto Return(GlulxImpl&, uint32_t) -> void;

auto Jump(GlulxImpl&, uint32_t) -> void;

}