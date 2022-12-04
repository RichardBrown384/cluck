#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

struct GlulxHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t ramstart;
    uint32_t extstart;
    uint32_t endmem;
    uint32_t stacksize;
    uint32_t startfunc;
    uint32_t decodingtable;
    uint32_t checksum;
    uint32_t filesize;
    uint32_t filechecksum;
};

auto ReadHeader(const std::vector<uint8_t>&) -> GlulxHeader;
auto IsHeaderValid(const GlulxHeader&) -> bool;

}