#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallSelect(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [eventRef] = TakeFirst<1>(arguments);
    event_t event;
    glk_select(&event);
    WriteStruct(eventRef, event);
    return 0u;
}

auto GlkServerImpl::CallSelectPoll(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [eventRef] = TakeFirst<1>(arguments);
    event_t event;
    glk_select_poll(&event);
    WriteStruct(eventRef, event);
    return 0u;
}

}