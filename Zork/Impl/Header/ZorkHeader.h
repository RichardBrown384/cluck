#pragma once

#include <cstdint>
#include <vector>

namespace fiction::zork {

struct ZorkHeader {
    uint32_t version;
    uint32_t highMemory;
    uint32_t programCounter;
    uint32_t dictionary;
    uint32_t objectTable;
    uint32_t globalVariables;
    uint32_t staticMemory;
    uint32_t abbreviationTable;
    uint32_t headerFileSize;
    uint32_t headerChecksum;
    uint32_t fileSize;
    uint32_t checksum;
};

auto ReadHeader(const std::vector<uint8_t>&) -> ZorkHeader;
auto IsHeaderValid(const ZorkHeader&) -> bool;

}