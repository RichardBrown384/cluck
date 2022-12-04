#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallImageGetInfo(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [image, widthRef, heightRef] = TakeFirst<3>(arguments);
    auto width { 0u }, height { 0u} ;
    auto info = glk_image_get_info(image, &width, &height);
    WriteReference(widthRef, width);
    WriteReference(heightRef, height);
    return info;
}

auto GlkServerImpl::CallImageDraw(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, image, val1, val2] = TakeFirst<4>(arguments);
    auto window = GetObject<winid_t>(windowId);
    return glk_image_draw(window, image, static_cast<int32_t>(val1), static_cast<int32_t>(val2));
}

auto GlkServerImpl::CallImageDrawScaled(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [windowId, image, val1, val2, width, height] = TakeFirst<6>(arguments);
    auto window = GetObject<winid_t>(windowId);
    return glk_image_draw_scaled(
        window,
        image,
        static_cast<int32_t>(val1),
        static_cast<int32_t>(val2),
        width,
        height);
}

}