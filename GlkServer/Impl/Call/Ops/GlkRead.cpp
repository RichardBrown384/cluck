#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallGetCharStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId] = TakeFirst<1>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    return glk_get_char_stream(stream);
}

auto GlkServerImpl::CallGetCharStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId] = TakeFirst<1>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    return glk_get_char_stream_uni(stream);
}

auto GlkServerImpl::CallGetLineStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, len] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    std::vector<char> buffer(len);
    auto result = glk_get_line_stream(stream, buffer.data(), len);
    WriteArray8(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallGetLineStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, len] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    std::vector<uint32_t> buffer(len);
    auto result = glk_get_line_stream_uni(stream, buffer.data(), len);
    WriteArray32(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallGetBufferStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, len] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    std::vector<char> buffer(len);
    auto result = glk_get_buffer_stream(stream, buffer.data(), len);
    WriteArray8(buffer.data(), len, address);
    return result;

}

auto GlkServerImpl::CallGetBufferStreamUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, address, len] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    std::vector<uint32_t> buffer(len);
    auto result = glk_get_buffer_stream_uni(stream, buffer.data(), len);
    WriteArray32(buffer.data(), len, address);
    return result;
}

}