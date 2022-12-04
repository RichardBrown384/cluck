#pragma once

#include <cstdint>
#include <unordered_map>

namespace fiction::glk {

class GlkObjectRegistry {
public:
    static GlkObjectRegistry& GetInstance() {
        // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
        static GlkObjectRegistry instance;
        return instance;
    }
    GlkObjectRegistry(const GlkObjectRegistry&) = delete;
    GlkObjectRegistry(const GlkObjectRegistry&&) = delete;
    void operator=(const GlkObjectRegistry&) = delete;
    void operator=(const GlkObjectRegistry&&) = delete;
    auto Register(void*, uint32_t) -> uint32_t;
    auto Unregister(void*, uint32_t, uint32_t) -> void;
    [[nodiscard]] auto GetObject(uint32_t) const -> void*;
    auto GetId(void*) const -> uint32_t;
private:
    GlkObjectRegistry() = default;
    std::unordered_map<uint32_t, void*> objects;
    std::unordered_map<void*, uint32_t> ids;
    uint32_t next { 1u };
};

}