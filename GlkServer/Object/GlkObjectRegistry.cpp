#include "GlkObjectRegistry.h"

using namespace fiction::glk;

auto GlkObjectRegistry::Register(
    void* object,
    [[maybe_unused]] uint32_t objectClass) -> uint32_t {
    const auto id = next;
    objects[id] = object;
    ids[object] = id;
    ++next;
    return id;
}

auto GlkObjectRegistry::Unregister(
    void* object,
    [[maybe_unused]] uint32_t objectClass,
    [[maybe_unused]] uint32_t dispatchRock) -> void {
    const auto id = ids[object];
    ids.erase(object);
    objects.erase(id);
}

auto GlkObjectRegistry::GetObject(uint32_t objectId) const -> void* {
    return (objectId == 0u) ? nullptr : objects.at(objectId);
}

auto GlkObjectRegistry::GetId(void* object) const -> uint32_t {
    return (object == nullptr) ? 0u : ids.at(object);
}