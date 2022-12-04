#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxSearch.h"

namespace fiction::glulx {
namespace {

constexpr auto SEARCH_OPTION_KEY_INDIRECT = 1u;
constexpr auto SEARCH_OPTION_KEY_ZERO_TERMINATES = 2u;
constexpr auto SEARCH_OPTION_RETURN_INDEX = 4u;

constexpr auto KEY_SIZE_8 = 1u;
constexpr auto KEY_SIZE_16 = 2u;
constexpr auto KEY_SIZE_32 = 4u;

constexpr auto Compare(uint32_t a, uint32_t b) -> int {
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    }
    return 0;
}

auto CompareIndirectN(GlulxImpl& glulx, uint32_t a, uint32_t b, uint32_t n) -> int {
    for (auto i = 0u; i < n; ++i) {
        const auto p = glulx.MemoryRead8(a + i);
        const auto q = glulx.MemoryRead8(b + i);
        const auto compare = Compare(p, q);
        if (compare) {
            return compare;
        }
    }
    return 0u;
}

auto CompareDirect8(GlulxImpl& glulx, uint32_t key, uint32_t address, uint32_t) {
    const auto a = key & 0xFFu;
    const auto b = glulx.MemoryRead8(address);
    return Compare(a, b);
}

auto CompareDirect16(GlulxImpl& glulx, uint32_t key, uint32_t address, uint32_t) {
    const auto a = key & 0xFFFFu;
    const auto b = glulx.MemoryRead16(address);
    return Compare(a, b);
}

auto CompareDirect32(GlulxImpl& glulx, uint32_t key, uint32_t address, uint32_t) {
    const auto a = key;
    const auto b = glulx.MemoryRead32(address);
    return Compare(a, b);
}

auto GetComparator(auto keySize, auto keyIndirect) {
    if (keyIndirect) {
        return CompareIndirectN;
    } else if (keySize == KEY_SIZE_8) {
        return CompareDirect8;
    } else if (keySize == KEY_SIZE_16) {
        return CompareDirect16;
    } else if (keySize == KEY_SIZE_32) {
        return CompareDirect32;
    }
    Error("Unsupported key size");
}

auto IsKeyZero(GlulxImpl& glulx, auto keyAddress, auto keySize) {
    for (auto i = 0u; i < keySize; ++i) {
        if (glulx.MemoryRead8(keyAddress + i) != 0u) {
            return false;
        }
    }
    return true;
}
}

auto LinearSearch(GlulxImpl& glulx, const GlulxSearchParameters& parameters) -> uint32_t {
    const auto [
        key, keySize, start, structSize, numStructs, keyOffset, options
    ] = parameters;

    const auto keyIndirect = options & SEARCH_OPTION_KEY_INDIRECT;
    const auto zeroKeyTerminates = options & SEARCH_OPTION_KEY_ZERO_TERMINATES;
    const auto returnIndex = options & SEARCH_OPTION_RETURN_INDEX;

    const auto comparator = GetComparator(keySize, keyIndirect);

    for (auto searchIndex = 0u; searchIndex < numStructs; ++searchIndex) {
        const auto structAddress = start + searchIndex * structSize;
        const auto keyAddress = structAddress + keyOffset;
        const auto compare = comparator(glulx, key, keyAddress, keySize);
        if (compare == 0) {
            return returnIndex ? searchIndex : structAddress;
        }
        if (zeroKeyTerminates && IsKeyZero(glulx, keyAddress, keySize)) {
            break;
        }
    }
    return returnIndex ? 0xFFFFFFFFu : 0u;
}

auto BinarySearch(GlulxImpl& glulx, const GlulxSearchParameters& parameters) -> uint32_t {
    const auto [
        key, keySize, start, structSize, numStructs, keyOffset, options
    ] = parameters;

    const auto keyIndirect = options & SEARCH_OPTION_KEY_INDIRECT;
    const auto returnIndex = options & SEARCH_OPTION_RETURN_INDEX;

    const auto comparator = GetComparator(keySize, keyIndirect);

    auto startIndex = 0u;
    auto endIndex = numStructs;
    while (startIndex < endIndex) {
        const auto searchIndex = (startIndex + endIndex) / 2;
        const auto structAddress = start + searchIndex * structSize;
        const auto keyAddress = structAddress + keyOffset;
        const auto compare = comparator(glulx, key, keyAddress, keySize);
        if (compare == 0) {
            return returnIndex ? searchIndex : structAddress;
        } else if (compare < 0) {
            endIndex = searchIndex;
        } else {
            startIndex = searchIndex + 1u;
        }
    }
    return returnIndex ? 0xFFFFFFFFu : 0u;
}

auto LinkedSearch(GlulxImpl& glulx, const GlulxLinkedSearchParameters& parameters) -> uint32_t {
    const auto [
        key, keySize, start, keyOffset, nextOffset, options
    ] = parameters;

    const auto keyIndirect = options & SEARCH_OPTION_KEY_INDIRECT;
    const auto zeroKeyTerminates = options & SEARCH_OPTION_KEY_ZERO_TERMINATES;

    const auto comparator = GetComparator(keySize, keyIndirect);

    auto structAddress = start;
    while (structAddress != 0u) {
        const auto keyAddress = structAddress + keyOffset;
        const auto nextAddress = structAddress + nextOffset;
        const auto compare = comparator(glulx, key, keyAddress, keySize);
        if (compare == 0) {
            return structAddress;
        }
        if (zeroKeyTerminates && IsKeyZero(glulx, keyAddress, keySize)) {
            break;
        }
        structAddress = glulx.MemoryRead32(nextAddress);
    }
    return 0u;
}

}