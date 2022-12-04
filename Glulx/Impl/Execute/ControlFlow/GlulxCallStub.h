#pragma once

#include <cstdint>

namespace fiction::glulx {

class GlulxImpl;

auto PushCallStub(GlulxImpl&, uint32_t, uint32_t) -> void;
auto PopCallStub(GlulxImpl&, uint32_t) -> void;

}