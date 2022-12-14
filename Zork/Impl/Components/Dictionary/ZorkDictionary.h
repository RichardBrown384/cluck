#pragma once

#include <cstdint>
#include <tuple>

namespace fiction::zork {

struct ZorkHeader;
class ZorkMemory;

class ZorkDictionary {
public:
    ZorkDictionary(ZorkMemory&, const ZorkHeader&);
    ZorkDictionary(const ZorkDictionary&) = delete;
    ZorkDictionary(ZorkDictionary&&) = delete;
    ZorkDictionary& operator=(const ZorkDictionary&) = delete;
    ZorkDictionary& operator=(ZorkDictionary&&) = delete;
    ~ZorkDictionary();
public:
    [[nodiscard]] auto ReadDictionaryWordSeparators() const -> std::vector<uint32_t>;
    [[nodiscard]] auto FindDictionaryEntry(const std::vector<uint32_t>&) const -> uint32_t;

private:
    [[nodiscard]] auto GetDictionaryWordSeparatorCountAddress() const -> uint32_t;
    [[nodiscard]] auto GetDictionaryWordSeparatorsAddress() const -> uint32_t;
    [[nodiscard]] auto GetDictionaryEntryLengthAddress() const -> uint32_t;
    [[nodiscard]] auto GetDictionaryNumberOfEntriesAddress() const -> uint32_t;
    [[nodiscard]] auto GetDictionaryEntriesAddress() const -> uint32_t;

    [[nodiscard]] auto ReadDictionaryWordSeparatorCount() const -> uint32_t;

    [[nodiscard]] auto ReadDictionaryEntryLength() const -> uint32_t;
    [[nodiscard]] auto ReadDictionaryNumberOfEntries() const -> uint32_t;

    [[nodiscard]] auto TestDictionaryEntry(uint32_t, const std::vector<uint32_t>&) const -> bool;
private:
    ZorkMemory& memory;
    const ZorkHeader& header;
};

}