#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxImpl;

auto EnterFunction(GlulxImpl&, uint32_t, const std::vector<uint32_t>&) -> void;
auto LeaveFunction(GlulxImpl&) -> void;

}