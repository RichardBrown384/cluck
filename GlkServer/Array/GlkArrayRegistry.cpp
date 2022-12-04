#include "GlkArrayRegistry.h"

#include <utility>

namespace fiction::glk {

auto GlkArrayRegistry::CreateArray8(uint32_t length, const std::function<void(char*)>& function) -> char* {
    auto managed = std::make_unique<char[]>(length);
    auto unmanaged = managed.get();
    arrays8.emplace(std::piecewise_construct,
                   std::forward_as_tuple(static_cast<void*>(unmanaged)),
                   std::forward_as_tuple(std::move(managed), function));
    return unmanaged;
}

auto GlkArrayRegistry::CreateArray32(uint32_t length, const std::function<void(uint32_t*)>& function) -> uint32_t* {
    auto managed = std::make_unique<uint32_t[]>(length);
    auto unmanaged = managed.get();
    arrays32.emplace(std::piecewise_construct,
                    std::forward_as_tuple(static_cast<void*>(unmanaged)),
                    std::forward_as_tuple(std::move(managed), function));
    return unmanaged;
}


auto GlkArrayRegistry::Register(
    void* array,
    [[maybe_unused]] uint32_t length,
    [[maybe_unused]] char* typeCode) -> uint32_t {
    const auto id = next;
    if (arrays8.contains(array)) {
        auto& data = arrays8.at(array);
        data.id = id;
    } else {
        auto& data = arrays32.at(array);
        data.id = id;
    }
    ++next;
    return id;
}

auto GlkArrayRegistry::Unregister(
    void* array,
    [[maybe_unused]] uint32_t length,
    [[maybe_unused]] char* typeCode,
    [[maybe_unused]] uint32_t id) -> void {
    if (arrays8.contains(array)) {
        auto& data = arrays8.at(array);
        data.callback(static_cast<char*>(array));
        arrays8.erase(array);
    } else {
        auto& data = arrays32.at(array);
        data.callback(static_cast<uint32_t*>(array));
        arrays32.erase(array);
    }
}

}