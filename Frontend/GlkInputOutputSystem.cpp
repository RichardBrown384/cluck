#include "GlkInputOutputSystem.h"

#include "GlkServer.h"

using namespace fiction::glulx;
using namespace fiction::glk;

GlkInputOutputSystem::GlkInputOutputSystem(GlkServer& server) : server(server) {}

auto GlkInputOutputSystem::PutChar8(GlulxImpl&, uint32_t c) -> void {
    server.PutChar8(c & 0xFFu);
}

auto GlkInputOutputSystem::PutChar32(GlulxImpl&, uint32_t c)  -> void {
    server.PutChar32(c);
}

auto GlkInputOutputSystem::PutBuffer8(GlulxImpl&, uint32_t streamId, const std::vector<uint8_t>& buffer) -> void {
    server.PutBuffer8(streamId, buffer);
}

auto GlkInputOutputSystem::GetBuffer8(GlulxImpl&, uint32_t streamId) -> std::vector<uint8_t> {
    return server.GetBuffer8(streamId);
}