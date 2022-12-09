#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <functional>

namespace fiction::glk {

template<typename T>
struct GlkArrayData {
    GlkArrayData(std::unique_ptr<T[]>, std::function<void(T*)> callback);
    GlkArrayData(const GlkArrayData&) = delete;
    void operator=(const GlkArrayData&) = delete;
    std::unique_ptr<T[]> array;
    uint32_t id;
    std::function<void(T*)> callback;
};

template<typename T>
GlkArrayData<T>::GlkArrayData(
    std::unique_ptr<T[]> array,
    std::function<void(T*)> callback) :
    array(std::move(array)),
    id(0),
    callback(std::move(callback)){}

class GlkArrayRegistry {
public:
    static GlkArrayRegistry& GetInstance() {
        // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
        static GlkArrayRegistry instance;
        return instance;
    }
    GlkArrayRegistry(const GlkArrayRegistry&) = delete;
    GlkArrayRegistry(const GlkArrayRegistry&&) = delete;
    void operator=(const GlkArrayRegistry&) = delete;
    void operator=(const GlkArrayRegistry&&) = delete;
    auto CreateArray8(uint32_t, const std::function<void(char*)>&) -> char*;
    auto CreateArray8(const std::vector<uint8_t>&, const std::function<void(char*)>&) -> char*;
    auto CreateArray32(uint32_t, const std::function<void(uint32_t*)>&) -> uint32_t*;
    auto Register(void*, uint32_t, char*) -> uint32_t;
    auto Unregister(void*, uint32_t, char*, uint32_t) -> void;
private:
    GlkArrayRegistry() = default;
    std::unordered_map<void*, GlkArrayData<char>> arrays8;
    std::unordered_map<void*, GlkArrayData<uint32_t>> arrays32;
    uint32_t next { 1u };
};

}