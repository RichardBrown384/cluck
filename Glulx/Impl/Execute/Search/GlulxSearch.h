#pragma once

#include <cstdint>

namespace fiction::glulx {

class GlulxImpl;

struct GlulxSearchParameters {
    [[maybe_unused]] uint32_t key;
    [[maybe_unused]] uint32_t keySize;
    [[maybe_unused]] uint32_t start;
    [[maybe_unused]] uint32_t structSize;
    [[maybe_unused]] uint32_t numStructs;
    [[maybe_unused]] uint32_t keyOffset;
    [[maybe_unused]] uint32_t options;
};

struct GlulxLinkedSearchParameters {
    [[maybe_unused]] uint32_t key;
    [[maybe_unused]] uint32_t keySize;
    [[maybe_unused]] uint32_t start;
    [[maybe_unused]] uint32_t keyOffset;
    [[maybe_unused]] uint32_t nextOffset;
    [[maybe_unused]] uint32_t options;
};

auto LinearSearch(GlulxImpl&, const GlulxSearchParameters&) -> uint32_t;
auto BinarySearch(GlulxImpl&, const GlulxSearchParameters&) -> uint32_t;
auto LinkedSearch(GlulxImpl&, const GlulxLinkedSearchParameters&) -> uint32_t;

}