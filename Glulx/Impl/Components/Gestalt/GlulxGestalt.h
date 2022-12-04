#pragma once

#include <cstdint>

namespace fiction::glulx {

class GlulxImpl;

auto Gestalt(GlulxImpl&, uint32_t, uint32_t) -> uint32_t;

}