#include "ZorkHeader.h"

#include "Common/Endian.h"

constexpr auto HEADER_OFFSET_VERSION = 0u;
constexpr auto HEADER_OFFSET_HIGH_MEMORY = 4u;
constexpr auto HEADER_OFFSET_PC = 6u;
constexpr auto HEADER_OFFSET_DICTIONARY = 8u;
constexpr auto HEADER_OFFSET_OBJECT_TABLE = 10u;
constexpr auto HEADER_OFFSET_GLOBAL_VARIABLES = 12u;
constexpr auto HEADER_OFFSET_STATIC_MEMORY = 14u;
constexpr auto HEADER_OFFSET_ABBREVIATION_TABLE = 24u;
constexpr auto HEADER_OFFSET_FILE_SIZE = 26u;
constexpr auto HEADER_OFFSET_FILE_CHECKSUM = 28u;

constexpr auto HEADER_LENGTH = 30u;

using namespace fiction::endian;

namespace {
auto ComputeChecksum(const std::vector<uint8_t>& file, uint32_t start, uint32_t end) -> uint32_t {
    auto checksum = 0u;
    for (auto address = start; address < end; address += 1) {
        checksum += Read8(file, address);
    }
    return checksum & 0xFFFF;
}
}

namespace fiction::zork {

auto ReadHeader(const std::vector<uint8_t>& file) -> ZorkHeader {
    ZorkHeader header{};
    if (file.size() < HEADER_LENGTH) {
        return header;
    }
    header.version = Read8(file, HEADER_OFFSET_VERSION);
    header.highMemory = Read16(file, HEADER_OFFSET_HIGH_MEMORY);
    header.programCounter = Read16(file, HEADER_OFFSET_PC);
    header.dictionary = Read16(file, HEADER_OFFSET_DICTIONARY);
    header.objectTable = Read16(file, HEADER_OFFSET_OBJECT_TABLE);
    header.globalVariables = Read16(file, HEADER_OFFSET_GLOBAL_VARIABLES);
    header.staticMemory = Read16(file, HEADER_OFFSET_STATIC_MEMORY);
    header.abbreviationTable = Read16(file, HEADER_OFFSET_ABBREVIATION_TABLE);
    header.headerFileSize = Read16(file, HEADER_OFFSET_FILE_SIZE);
    header.headerChecksum = Read16(file, HEADER_OFFSET_FILE_CHECKSUM);
    header.fileSize = file.size();
    header.checksum = ComputeChecksum(file, 0x40u, 2u * header.headerFileSize);
    return header;
}

auto IsHeaderValid(const ZorkHeader& header) -> bool {
    return header.version <= 3;
}

}

