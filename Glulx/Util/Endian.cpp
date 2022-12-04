#include "Endian.h"

namespace fiction::endian {

auto PackBigEndian16(uint32_t a, uint32_t b) -> uint32_t {
    return (a << 8) + b;
}

auto PackBigEndian32(uint32_t a, uint32_t b, uint32_t c, uint32_t d) -> uint32_t {
    return (a << 24) + (b << 16) + (c << 8) + d;
}

auto Read8(const std::vector<uint8_t>& array, const uint32_t address) -> uint32_t {
    return array[address];
}

auto Read16(const std::vector<uint8_t>& array, const uint32_t address) -> uint32_t {
    const auto a = Read8(array, address);
    const auto b = Read8(array, address + 1u);
    return PackBigEndian16(a, b);
}

auto Read32(const std::vector<uint8_t>& array, const uint32_t address) -> uint32_t {
    const auto a = Read8(array, address);
    const auto b = Read8(array, address + 1u);
    const auto c = Read8(array, address + 2u);
    const auto d = Read8(array, address + 3u);
    return PackBigEndian32(a, b, c, d);
}

auto Write8(std::vector<uint8_t>& array, const uint32_t address, const uint32_t value) -> void {
    array[address] = value;
}

auto Write16(std::vector<uint8_t>& array, const uint32_t address, const uint32_t value) -> void {
    Write8(array, address, value >> 8);
    Write8(array, address + 1, value);
}

auto Write32(std::vector<uint8_t>& array, const uint32_t address, const uint32_t value) -> void {
    Write8(array, address, value >> 24);
    Write8(array, address + 1, value >> 16);
    Write8(array, address + 2, value >> 8);
    Write8(array, address + 3, value);
}

}