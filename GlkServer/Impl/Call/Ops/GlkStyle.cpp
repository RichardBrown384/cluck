#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallSetStyle(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [style] = TakeFirst<1>(arguments);
    glk_set_style(style);
    return 0u;
}

auto GlkServerImpl::CallSetStyleStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, style] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_set_style_stream(stream, style);
    return 0u;
}

auto GlkServerImpl::CallStylehintSet(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [winType, style, hint, value] = TakeFirst<4>(arguments);
    glk_stylehint_set(winType, style, hint, static_cast<int32_t>(value));
    return 0u;
}

auto GlkServerImpl::CallStylehintClear(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [winType, style, hint] = TakeFirst<3>(arguments);
    glk_stylehint_clear(winType, style, hint);
    return 0u;
}

auto GlkServerImpl::CallStyleDistinguish(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, style1, style2] = TakeFirst<3>(arguments);
    auto window = GetObject<winid_t>(windowId);
    return glk_style_distinguish(window, style1, style2);
}

auto GlkServerImpl::CallStyleMeasure(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, style, hint, resultRef] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto result { 0u };
    auto measure = glk_style_measure(window, style, hint, &result);
    WriteReference(resultRef, result);
    return measure;
}

}