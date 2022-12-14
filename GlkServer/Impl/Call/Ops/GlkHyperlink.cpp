#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallSetHyperlink(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto&[linkVal] = TakeFirst<1>(arguments);
    glk_set_hyperlink(linkVal);
    return 0u;
}

auto GlkServerImpl::CallSetHyperlinkStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto&[streamId, linkVal] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_set_hyperlink_stream(stream, linkVal);
    return 0u;
}

}