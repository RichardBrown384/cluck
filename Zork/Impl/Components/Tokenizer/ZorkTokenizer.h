#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace fiction::zork {

class ZorkDictionary;
struct ZorkHeader;

struct ZorkToken {
    ZorkToken(uint32_t entry_in, uint32_t length_in, uint32_t start_in) :
        entry(entry_in),
        length(length_in),
        start(start_in) {}
    uint32_t entry;
    uint32_t length;
    uint32_t start;
};

class ZorkTokenizer {
public:
    ZorkTokenizer(ZorkDictionary&, const ZorkHeader&);
    ZorkTokenizer(const ZorkTokenizer&) = delete;
    ZorkTokenizer(ZorkTokenizer&&) = delete;
    ZorkTokenizer& operator=(const ZorkTokenizer&) = delete;
    ZorkTokenizer& operator=(ZorkTokenizer&&) = delete;
    ~ZorkTokenizer();

    [[nodiscard]] auto TokenizeString(const std::string& text) const -> std::vector<ZorkToken>;

private:
    ZorkDictionary& dictionary;
    const ZorkHeader& header;
};
}