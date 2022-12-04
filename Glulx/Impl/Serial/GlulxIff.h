#pragma once

#include <cstdint>

namespace {

constexpr auto IffType(uint8_t a, uint8_t b, uint8_t c, uint8_t d) -> uint32_t {
    return (a << 24u) + (b << 16u) + (c << 8u) + d;
}

constexpr auto FORM = IffType('F', 'O', 'R', 'M');
constexpr auto IFZS = IffType('I', 'F', 'Z', 'S');
constexpr auto IFHD = IffType('I', 'F', 'h', 'd');
constexpr auto CMEM = IffType('C', 'M', 'e', 'm');
constexpr auto MALL = IffType('M', 'A', 'l', 'l');
constexpr auto STKS = IffType('S', 't', 'k', 's');


}