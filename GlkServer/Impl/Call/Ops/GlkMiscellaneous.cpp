#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallExit(const std::vector<uint32_t>&) -> uint32_t {
    glk_exit();
}

auto GlkServerImpl::CallSetInterruptHandler(const std::vector<uint32_t>&) -> uint32_t {
    return 0u;
}

auto GlkServerImpl::CallTick(const std::vector<uint32_t>&) -> uint32_t {
    glk_tick();
    return 0u;
}

auto GlkServerImpl::CallGestalt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [selector, argument] = TakeFirst<2>(arguments);
    return glk_gestalt(selector, argument);
}

auto GlkServerImpl::CallGestaltExt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [selector, argument, address, length] = TakeFirst<4>(arguments);
    auto array = ReadArray32(address, length);
    return glk_gestalt_ext(selector, argument, array.data(), length);
}

}