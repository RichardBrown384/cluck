#include "GlkGlulxClient.h"

#include "Glulx.h"

using namespace fiction::glulx;

GlkGlulxClient::GlkGlulxClient(Glulx& glulx_in) : glulx(glulx_in) {}

auto GlkGlulxClient::StackPush32(uint32_t v) -> void { glulx.StackPush32(v); }
auto GlkGlulxClient::StackPop32() -> uint32_t { return glulx.StackPop32(); }
auto GlkGlulxClient::MemoryRead8(uint32_t address) -> uint32_t { return glulx.MemoryRead8(address); }
auto GlkGlulxClient::MemoryRead32(uint32_t address) -> uint32_t { return glulx.MemoryRead32(address); }
auto GlkGlulxClient::MemoryWrite8(uint32_t address, uint32_t v) -> void { glulx.MemoryWrite8(address, v); }
auto GlkGlulxClient::MemoryWrite32(uint32_t address, uint32_t v) -> void { glulx.MemoryWrite32(address, v); }
