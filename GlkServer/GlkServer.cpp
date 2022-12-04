#include "GlkServer.h"

#include "Impl/GlkServerImpl.h"
#include "GlkClient.h"
#include "Object/GlkObjectRegistry.h"
#include "Array/GlkArrayRegistry.h"

namespace fiction::glk {

GlkServer::GlkServer(GlkClient& client)
    : impl(std::make_unique<GlkServerImpl>(
        GlkObjectRegistry::GetInstance(),
        GlkArrayRegistry::GetInstance(),
        client)) {}

GlkServer::~GlkServer() = default;

auto GlkServer::Tick() -> void { GetImpl().Tick(); }

auto GlkServer::PutChar8(uint32_t c) -> void { GetImpl().PutChar8(c); }
auto GlkServer::PutChar32(uint32_t c) -> void { GetImpl().PutChar32(c); }

auto GlkServer::PutBuffer8(uint32_t streamId, const std::vector<uint8_t>& buffer) -> void {
    GetImpl().PutBuffer8(streamId, buffer);
}

auto GlkServer::GetBuffer8(uint32_t streamId) -> std::vector<uint8_t> {
    return GetImpl().GetBuffer8(streamId);
}

auto GlkServer::Call(uint32_t function, const std::vector<uint32_t>& arguments) -> uint32_t {
    return GetImpl().Call(function, arguments);
}

}
