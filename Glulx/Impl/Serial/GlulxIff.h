#pragma once

#include <cstdint>

#include "Util/Endian.h"

namespace fiction::glulx {

class GlulxReadBuffer;

constexpr auto FORM = endian::PackBigEndian32('F', 'O', 'R', 'M');
constexpr auto IFZS = endian::PackBigEndian32('I', 'F', 'Z', 'S');
constexpr auto IFHD = endian::PackBigEndian32('I', 'F', 'h', 'd');
constexpr auto CMEM = endian::PackBigEndian32('C', 'M', 'e', 'm');
constexpr auto MALL = endian::PackBigEndian32('M', 'A', 'l', 'l');
constexpr auto STKS = endian::PackBigEndian32('S', 't', 'k', 's');

constexpr auto IFRS = endian::PackBigEndian32('I', 'F', 'R', 'S');
constexpr auto GLUL = endian::PackBigEndian32('G', 'L', 'U', 'L');

auto IsFormHeaderValid(GlulxReadBuffer&, uint32_t) -> bool;

auto LocateChunk(GlulxReadBuffer&, uint32_t) -> std::pair<uint32_t, uint32_t>;

auto ExtractChunk(GlulxReadBuffer&, uint32_t) -> std::vector<uint8_t>;

}