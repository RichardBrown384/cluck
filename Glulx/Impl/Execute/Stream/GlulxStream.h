#pragma once

#include <cstdint>

namespace fiction::glulx {

class GlulxImpl;

auto StreamCharNested(GlulxImpl&, uint32_t c) -> void;
auto StreamChar(GlulxImpl&, uint32_t c) -> void;

auto StreamUnicodeCharNested(GlulxImpl&, uint32_t c) -> void;
auto StreamUnicodeChar(GlulxImpl&, uint32_t c) -> void;

auto StreamInteger(GlulxImpl&, uint32_t, uint32_t) -> void;
auto StreamInteger(GlulxImpl&, uint32_t) -> void;

auto StreamCString(GlulxImpl&, uint32_t) -> void;
auto StreamUnicodeString(GlulxImpl&, uint32_t) -> void;
auto StreamCompressedString(GlulxImpl&, uint32_t, uint32_t) -> void;

auto StreamStringNested(GlulxImpl&, uint32_t) -> void;
auto StreamString(GlulxImpl&, uint32_t) -> void;

}
