#include "GlulxHeader.h"

#include "Util/Endian.h"

using namespace fiction::endian;

namespace {

constexpr auto GLULX_MAGIC = 0x476C756Cu;

constexpr auto HEADER_MAGIC_OFFSET = 0u;
constexpr auto HEADER_VERSION_OFFSET = 4u;
constexpr auto HEADER_RAMSTART_OFFSET = 8u;
constexpr auto HEADER_EXTSTART_OFFSET = 12u;
constexpr auto HEADER_ENDMEM_OFFSET = 16u;
constexpr auto HEADER_STACKSIZE_OFFSET = 20u;
constexpr auto HEADER_STARTFUNC_OFFSET = 24u;
constexpr auto HEADER_DECODINGTABLE_OFFSET = 28u;
constexpr auto HEADER_CHECKSUM_OFFSET = 32u;
constexpr auto HEADER_LENGTH = 36u;

constexpr auto MajorVersion(auto version) { return version >> 16u; }
constexpr auto MinorVersion(auto version) { return (version >> 8u) & 0xFFu; }
constexpr auto RevisionVersion(auto version) { return version & 0xFFu; }

auto ComputeChecksum(const std::vector<uint8_t>& file, auto start, auto end) -> uint32_t {
    auto checksum = 0u;
    for (auto address = start; address < end; address += 4) {
        checksum += Read32(file, address);
    }
    return checksum;
}

}

namespace fiction::glulx {

auto ReadHeader(const std::vector<uint8_t>& file) -> GlulxHeader {
    GlulxHeader header {};
    if (file.size() < HEADER_LENGTH) {
        return header;
    }
    header.magic = Read32(file, HEADER_MAGIC_OFFSET);
    header.version = Read32(file, HEADER_VERSION_OFFSET);
    header.ramstart = Read32(file, HEADER_RAMSTART_OFFSET);
    header.extstart = Read32(file, HEADER_EXTSTART_OFFSET);
    header.endmem = Read32(file, HEADER_ENDMEM_OFFSET);
    header.stacksize = Read32(file, HEADER_STACKSIZE_OFFSET);
    header.startfunc = Read32(file, HEADER_STARTFUNC_OFFSET);
    header.decodingtable = Read32(file, HEADER_DECODINGTABLE_OFFSET);
    header.checksum = Read32(file, HEADER_CHECKSUM_OFFSET);
    header.filesize = file.size();
    header.filechecksum = ComputeChecksum(file, 0u, HEADER_CHECKSUM_OFFSET)
        + ComputeChecksum(file, HEADER_CHECKSUM_OFFSET + 4u, header.filesize);
    return header;
}

auto IsHeaderValid(const GlulxHeader& header) -> bool {
    if (header.magic != GLULX_MAGIC) {
        return false;
    }

    auto major = MajorVersion(header.version);
    auto minor = MinorVersion(header.version);
    auto revision = RevisionVersion(header.version);
    if (major < 2 || major > 3) {
        return false;
    } else if (major == 3) {
        if (minor > 1 || revision > 3) {
            return false;
        }
    }

    const auto extstartValid = (header.extstart == header.filesize);
    if (!extstartValid) {
        return false;
    }

    const auto memoryLayoutValid =
        (header.ramstart <= header.extstart) &&
        (header.extstart <= header.endmem);

    if (!memoryLayoutValid) {
        return false;
    }

    const auto memoryBoundariesValid =
        ((header.ramstart & 0xFFu) == 0u) &&
        ((header.extstart & 0xFFu) == 0u) &&
        ((header.endmem & 0xFFu) == 0);

    if (!memoryBoundariesValid) {
        return false;
    }

    const auto stackBoundaryValid = ((header.stacksize & 0xFFu) == 0);
    if (!stackBoundaryValid) {
        return false;
    }

    return header.checksum == header.filechecksum;
}

}