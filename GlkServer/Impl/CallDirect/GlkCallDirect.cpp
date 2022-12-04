#include "Impl/GlkServerImpl.h"

extern "C" {
#include "glk.h"
}

namespace fiction::glk {

auto GlkServerImpl::Tick() -> void {
    glk_tick();
}

auto GlkServerImpl::PutChar8(uint32_t c) -> void {
    glk_put_char(c & 0xFFu);
}

auto GlkServerImpl::PutChar32(uint32_t c) -> void {
    glk_put_char_uni(c);
}

auto GlkServerImpl::PutBuffer8(uint32_t streamId, const std::vector<uint8_t>& buffer) -> void {
    auto stream = GetObject<strid_t>(streamId);
    auto data = reinterpret_cast<char*>(const_cast<uint8_t*>(buffer.data()));
    glk_put_buffer_stream(stream, data, buffer.size());
}

auto GlkServerImpl::GetBuffer8(uint32_t streamId) -> std::vector<uint8_t> {
    auto stream = GetObject<strid_t>(streamId);
    std::vector<uint8_t> buffer(4096);
    std::vector<uint8_t> result;
    for (;;) {
        auto count = glk_get_buffer_stream(
            stream,
            reinterpret_cast<char*>(buffer.data()),
            buffer.size()
        );
        if (count <= 0u) {
            break;
        }
        result.insert(result.end(), buffer.begin(), buffer.begin() + count);
    }
    return result;
}

}