#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallPutChar(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [c] = TakeFirst<1>(arguments);
    glk_put_char(c);
    return 0u;
}

auto GlkServerImpl::CallPutCharUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [c] = TakeFirst<1>(arguments);
    glk_put_char_uni(c);
    return 0u;
}

auto GlkServerImpl::CallPutCharStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, c] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_put_char_stream(stream, c);
    return 0u;
}

auto GlkServerImpl::CallPutCharStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, c] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_put_char_stream_uni(stream, c);
    return 0u;
}

auto GlkServerImpl::CallPutString(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address] = TakeFirst<1>(arguments);
    auto buffer = ReadString8(address);
    glk_put_string(buffer.data());
    return 0u;
}

auto GlkServerImpl::CallPutStringUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address] = TakeFirst<1>(arguments);
    auto buffer = ReadString32(address);
    glk_put_string_uni(buffer.data());
    return 0u;
}

auto GlkServerImpl::CallPutStringStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    auto buffer = ReadString8(address);
    glk_put_string_stream(stream, buffer.data());
    return 0u;
}

auto GlkServerImpl::CallPutStringStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    auto buffer = ReadString32(address);
    glk_put_string_stream_uni(stream, buffer.data());
    return 0u;
}

auto GlkServerImpl::CallPutBuffer(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, length] = TakeFirst<2>(arguments);
    auto buffer = ReadArray8(address, length);
    glk_put_buffer(buffer.data(), length);
    return 0u;
}

auto GlkServerImpl::CallPutBufferUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, length] = TakeFirst<2>(arguments);
    auto buffer = ReadArray32(address, length);
    glk_put_buffer_uni(buffer.data(), length);
    return 0u;
}

auto GlkServerImpl::CallPutBufferStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, length] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    auto buffer = ReadArray8(address, length);
    glk_put_buffer_stream(stream, buffer.data(), length);
    return 0u;
}

auto GlkServerImpl::CallPutBufferStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, length] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    auto buffer = ReadArray32(address, length);
    glk_put_buffer_stream_uni(stream, buffer.data(), length);
    return 0u;
}

}