#include "GlkServerImpl.h"

#include "GlkClient.h"
#include "Object/GlkObjectRegistry.h"
#include "Array/GlkArrayRegistry.h"

#include <iostream>

namespace fiction::glk {

constexpr auto STRING_C = 0xE0u;
constexpr auto STRING_UNICODE = 0xE2000000u;

[[noreturn]] auto Error(const char* message) -> void {
    std::cerr << "Glk Error" << std::endl;
    std::cerr << message << std::endl;
    std::exit(1);
}

GlkServerImpl::GlkServerImpl(
    GlkObjectRegistry& objects,
    GlkArrayRegistry& arrays,
    GlkClient& client) :
    objects(objects), arrays(arrays), client(client) {}


auto GlkServerImpl::GetObject(uint32_t id) -> void* {
    return objects.GetObject(id);
}

auto GlkServerImpl::GetObjectId(void* object) -> uint32_t {
    return objects.GetId(object);
}

auto GlkServerImpl::CreateArray8(uint32_t length, const std::function<void(char*)>& function) -> char* {
    return arrays.CreateArray8(length, function);
}

auto GlkServerImpl::CreateArray32(uint32_t length, const std::function<void(uint32_t *)>& function) -> uint32_t* {
    return arrays.CreateArray32(length, function);
}

auto GlkServerImpl::ReadArray8(uint32_t address, uint32_t length) -> std::vector<char> {
    std::vector<char> buffer(length);
    for (auto i = 0u; i < length; ++i) {
        buffer[i] = static_cast<char>(client.MemoryRead8(address));
    }
    return buffer;
}

auto GlkServerImpl::ReadArray32(uint32_t address, uint32_t length) -> std::vector<uint32_t> {
    std::vector<uint32_t> buffer(length);
    for (auto i = 0u; i < length; ++i) {
        buffer[i] = client.MemoryRead32(address + 4u * i);
    }
    return buffer;
}

auto GlkServerImpl::ReadString8(uint32_t address) -> std::vector<char> {
    std::vector<char> buffer;
    auto c = client.MemoryRead8(address);
    if (c != STRING_C) {
        Error("Non C-string being used in string method");
    }
    address = address + 1u;
    c = client.MemoryRead8(address);
    buffer.push_back(static_cast<char>(c));
    while (c != 0u) {
        address = address + 1u;
        c = client.MemoryRead8(address);
        buffer.push_back(static_cast<char>(c));
    }
    return buffer;
}

auto GlkServerImpl::ReadString32(uint32_t address) -> std::vector<uint32_t> {
    std::vector<uint32_t> buffer;
    auto c = client.MemoryRead32(address);
    if (c != STRING_UNICODE) {
        Error("Non-unicode string being used in unicode method");
    }
    address = address + 4u;
    c = client.MemoryRead32(address);
    buffer.push_back(c);
    while (c != 0u) {
        address = address + 4u;
        c = client.MemoryRead32(address);
        buffer.push_back(c);
    }
    return buffer;
}

auto GlkServerImpl::ReadStructField(uint32_t reference, uint32_t index) -> int32_t {
    if (reference == 0xFFFFFFFFu) {
        return static_cast<int32_t>(client.StackPop32());
    }
    return static_cast<int32_t>(client.MemoryRead32(reference + 4u * index));
}

auto GlkServerImpl::ReadStruct(uint32_t reference, glktimeval_t& result) -> void {
    auto& [highSec, lowSec, microSec] = result;
    highSec = ReadStructField(reference, 0);
    lowSec = ReadStructField(reference, 1);
    microSec = ReadStructField(reference, 2);
}

auto GlkServerImpl::ReadStruct(uint32_t reference, glkdate_t& result) -> void {
    auto& [ year, month, day, weekday, hour, minute, second, microSec ] = result;
    year = ReadStructField(reference, 0);
    month = ReadStructField(reference, 1);
    day = ReadStructField(reference, 2);
    weekday = ReadStructField(reference, 3);
    hour = ReadStructField(reference, 4);
    minute = ReadStructField(reference, 5);
    second = ReadStructField(reference, 6);
    microSec = ReadStructField(reference, 7);
}

auto GlkServerImpl::WriteReference(uint32_t reference, uint32_t value) -> void {
    if (reference == 0xFFFFFFFFu) {
        client.StackPush32(value);
        return;
    } else if (reference != 0u) {
        client.MemoryWrite32(reference, value);
    }
}

auto GlkServerImpl::WriteArray8(char* array, uint32_t length, uint32_t address) -> void {
    for (auto i = 0u; i < length; ++i) {
        client.MemoryWrite8(address + i, array[i]);
    }
}

auto GlkServerImpl::WriteArray32(uint32_t* array, uint32_t length, uint32_t address) -> void {
    for (auto i = 0u; i < length; ++i) {
        client.MemoryWrite32(address + i, array[i]);
    }
}

auto GlkServerImpl::WriteStructField(uint32_t reference, uint32_t index, uint32_t value) -> void {
    if (reference == 0xFFFFFFFFu) {
        client.StackPush32(value);
    } else if (reference != 0u) {
        client.MemoryWrite32(reference + 4u * index, value);
    }
}

auto GlkServerImpl::WriteStruct(uint32_t reference, event_t& event) -> void {
    // Note: win prevents us from passing by constant reference
    auto& [type, win, val1, val2] = event;
    WriteStructField(reference, 0, type);
    WriteStructField(reference, 1, GetObjectId(win));
    WriteStructField(reference, 2, val1);
    WriteStructField(reference, 3, val2);
}

auto GlkServerImpl::WriteStruct(uint32_t reference, const stream_result_t& result) -> void {
    auto& [readCount, writeCount] = result;
    WriteStructField(reference, 0, readCount);
    WriteStructField(reference, 1, writeCount);
}

auto GlkServerImpl::WriteStruct(uint32_t reference, const glktimeval_t& result) -> void {
    const auto& [highSec, lowSec, microSec] = result;
    WriteStructField(reference, 0, highSec);
    WriteStructField(reference, 1, lowSec);
    WriteStructField(reference, 2, microSec);
}

auto GlkServerImpl::WriteStruct(uint32_t reference, const glkdate_t& result) -> void {
    const auto& [ year, month, day, weekday, hour, minute, second, microSec ] = result;
    WriteStructField(reference, 0, year);
    WriteStructField(reference, 1, month);
    WriteStructField(reference, 2, day);
    WriteStructField(reference, 3, weekday);
    WriteStructField(reference, 4, hour);
    WriteStructField(reference, 5, minute);
    WriteStructField(reference, 6, second);
    WriteStructField(reference, 7, microSec);
}
    
}
