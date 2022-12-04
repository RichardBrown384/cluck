#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallRequestLineEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    // Note: the specific lambda captures are to deal with the fact Apple clang version 14.0.0
    // does not support capturing structured bindings, contrary to the C++20 standard
    const auto& [windowId, address, maxlen, initlen] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto buffer = CreateArray8(maxlen, [=, maxlen=maxlen, address=address](char* array) {
        WriteArray8(array, maxlen, address);
    });
    glk_request_line_event(window, buffer, maxlen, initlen);
    return 0u;
}

auto GlkServerImpl::CallRequestLineEventUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    // Note: the specific lambda captures are to deal with the fact Apple clang version 14.0.0
    // does not support capturing structured bindings, contrary to the C++20 standard
    const auto& [windowId, address, maxlen, initlen] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto buffer = CreateArray32(maxlen, [=, maxlen=maxlen, address=address](uint32_t* array) {
        WriteArray32(array, maxlen, address);
    });
    glk_request_line_event_uni(window, buffer, maxlen, initlen);
    return 0u;
}

auto GlkServerImpl::CallCancelLineEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, eventRef] = TakeFirst<2>(arguments);
    auto window = GetObject<winid_t>(windowId);
    event_t event {};
    glk_cancel_line_event(window, &event);
    WriteStruct(eventRef, event);
    return 0u;
}

auto GlkServerImpl::CallRequestCharEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_request_char_event(window);
    return 0u;
}

auto GlkServerImpl::CallRequestCharEventUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_request_char_event_uni(window);
    return 0u;
}

auto GlkServerImpl::CallCancelCharEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_cancel_char_event(window);
    return 0u;
}

auto GlkServerImpl::CallRequestMouseEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_request_mouse_event(window);
    return 0u;
}

auto GlkServerImpl::CallCancelMouseEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_cancel_mouse_event(window);
    return 0u;
}

auto GlkServerImpl::CallRequestTimerEvents(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [millis] = TakeFirst<1>(arguments);
    glk_request_timer_events(millis);
    return 0u;
}

auto GlkServerImpl::CallRequestHyperlinkEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_request_hyperlink_event(window);
    return 0u;
}

auto GlkServerImpl::CallCancelHyperlinkEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_cancel_hyperlink_event(window);
    return 0u;
}

auto GlkServerImpl::CallSetEchoLineEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, value] = TakeFirst<2>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_set_echo_line_event(window, value);
    return 0u;
}

auto GlkServerImpl::CallSetTerminatorsLineEvent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, address, count] = TakeFirst<3>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto keycodes = ReadArray32(address, count);
    glk_set_terminators_line_event(window, keycodes.data(), count);
    return 0u;
}

}