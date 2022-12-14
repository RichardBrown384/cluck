#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallWindowIterate(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, rockRef] = TakeFirst<2>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto rock { 0u };
    const auto next = glk_window_iterate(window, &rock);
    WriteReference(rockRef, rock);
    return GetObjectId(next);
}

auto GlkServerImpl::CallWindowGetRock(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    return glk_window_get_rock(window);
}

auto GlkServerImpl::CallWindowGetRoot(const std::vector<uint32_t>&) -> uint32_t {
    auto window = glk_window_get_root();
    return GetObjectId(window);
}

auto GlkServerImpl::CallWindowOpen(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [splitId, method, size, winType, rock] = TakeFirst<5>(arguments);
    auto split = GetObject<winid_t>(splitId);
    const auto window = glk_window_open(split, method, size, winType, rock);
    return GetObjectId(window);
}

auto GlkServerImpl::CallWindowClose(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, resultRef] = TakeFirst<2>(arguments);
    auto window = GetObject<winid_t>(windowId);
    stream_result_t result {};
    glk_window_close(window, &result);
    WriteStruct(resultRef, result);
    return 0u;
}

auto GlkServerImpl::CallWindowGetSize(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, widthRef, heightRef] = TakeFirst<3>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto width { 0u }, height { 0u };
    glk_window_get_size(window, &width, &height);
    WriteReference(widthRef, width);
    WriteReference(heightRef, height);
    return 0u;
}

auto GlkServerImpl::CallWindowSetArrangement(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, method, size, keyWindowId] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto keyWindow = GetObject<winid_t>(keyWindowId);
    glk_window_set_arrangement(window, method, size, keyWindow);
    return 0u;
}

auto GlkServerImpl::CallWindowGetArrangement(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, methodRef, sizeRef, keyWindowRef] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto method { 0u }, size { 0u };
    winid_t keyWindow { };
    glk_window_get_arrangement(window, &method, &size, &keyWindow);
    WriteReference(methodRef, method);
    WriteReference(sizeRef, size);
    WriteReference(keyWindowRef, GetObjectId(keyWindow));
    return 0u;
}

auto GlkServerImpl::CallWindowGetType(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    return glk_window_get_type(window);
}

auto GlkServerImpl::CallWindowGetParent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto parent = glk_window_get_parent(window);
    return GetObjectId(parent);
}

auto GlkServerImpl::CallWindowClear(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_window_clear(window);
    return 0u;
}

auto GlkServerImpl::CallWindowMoveCursor(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, xpos, ypos] = TakeFirst<3>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_window_move_cursor(window, xpos, ypos);
    return 0u;
}

auto GlkServerImpl::CallWindowGetStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto stream = glk_window_get_stream(window);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallWindowSetEchoStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, streamId] = TakeFirst<2>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto stream = GetObject<strid_t>(streamId);
    glk_window_set_echo_stream(window, stream);
    return 0u;
}

auto GlkServerImpl::CallWindowGetEchoStream(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    auto window = GetObject<winid_t>(windowId);
    auto stream = glk_window_get_echo_stream(window);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallSetWindow(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    const auto window = GetObject<winid_t>(windowId);
    glk_set_window(window);
    return 0u;
}

auto GlkServerImpl::CallWindowGetSibling(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    const auto window = GetObject<winid_t>(windowId);
    auto sibling = glk_window_get_sibling(window);
    return GetObjectId(sibling);
}

auto GlkServerImpl::CallWindowFlowBreak(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId] = TakeFirst<1>(arguments);
    const auto window = GetObject<winid_t>(windowId);
    glk_window_flow_break(window);
    return 0u;
}

auto GlkServerImpl::CallWindowEraseRect(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, left, top, width, height] = TakeFirst<5>(arguments);
    const auto window = GetObject<winid_t>(windowId);
    glk_window_erase_rect(window, static_cast<int32_t>(left), static_cast<int32_t>(top), width, height);
    return 0u;
}

auto GlkServerImpl::CallWindowFillRect(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, colour, left, top, width, height] = TakeFirst<6>(arguments);
    auto window = GetObject<winid_t>(windowId);
    glk_window_fill_rect(window, colour, static_cast<int32_t>(left), static_cast<int32_t>(top), width, height);
    return 0u;
}

auto GlkServerImpl::CallWindowSetBackgroundColor(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, colour] = TakeFirst<2>(arguments);
    const auto window = GetObject<winid_t>(windowId);
    glk_window_set_background_color(window, colour);
    return 0u;
}

}