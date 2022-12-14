#include "ZorkDictionary.h"

#include "Impl/Components/Memory/ZorkMemory.h"
#include "Impl/Header/ZorkHeader.h"

using namespace fiction::zork;

namespace {
constexpr auto OFFSET_DICTIONARY_WORD_SEPARATOR_COUNT = 0u;
constexpr auto OFFSET_DICTIONARY_WORD_SEPARATORS = 1U;
}

ZorkDictionary::ZorkDictionary(ZorkMemory& memory_in, const ZorkHeader& header_in) :
    memory(memory_in),
    header(header_in) {}

ZorkDictionary::~ZorkDictionary() = default;

auto ZorkDictionary::GetDictionaryWordSeparatorCountAddress() const -> uint32_t {
    return header.dictionary + OFFSET_DICTIONARY_WORD_SEPARATOR_COUNT;
}

auto ZorkDictionary::GetDictionaryWordSeparatorsAddress() const -> uint32_t {
    return header.dictionary + OFFSET_DICTIONARY_WORD_SEPARATORS;
}

auto ZorkDictionary::GetDictionaryEntryLengthAddress() const -> uint32_t {
    const auto address = GetDictionaryWordSeparatorsAddress();
    const auto separatorCount = ReadDictionaryWordSeparatorCount();
    return address + separatorCount;
}

auto ZorkDictionary::GetDictionaryNumberOfEntriesAddress() const -> uint32_t {
    return GetDictionaryEntryLengthAddress() + 1u;
}

auto ZorkDictionary::GetDictionaryEntriesAddress() const -> uint32_t {
    return GetDictionaryNumberOfEntriesAddress() + 2u;
}

auto ZorkDictionary::ReadDictionaryWordSeparatorCount() const -> uint32_t {
    return memory.Read8(GetDictionaryWordSeparatorCountAddress());
}

auto ZorkDictionary::ReadDictionaryWordSeparators() const -> std::vector<uint32_t> {
    const auto count = ReadDictionaryWordSeparatorCount();
    auto address = GetDictionaryWordSeparatorsAddress();
    std::vector<uint32_t> separators;
    for (auto i = 0u; i < count; ++i) {
        const auto code = memory.Read8(address);
        address += 1;
        separators.push_back(code);
    }
    return separators;
}

auto ZorkDictionary::ReadDictionaryEntryLength() const -> uint32_t {
    return memory.Read8(GetDictionaryEntryLengthAddress());
}

auto ZorkDictionary::ReadDictionaryNumberOfEntries() const -> uint32_t {
    return memory.Read16(GetDictionaryNumberOfEntriesAddress());
}

auto ZorkDictionary::TestDictionaryEntry(
    uint32_t entryAddress,
    const std::vector<uint32_t>& words) const -> bool {
    auto address = entryAddress;
    for (uint32_t word : words) {
        if (memory.Read16(address) != word) {
            return false;
        }
        address += 2;
    }
    return true;
}

auto ZorkDictionary::FindDictionaryEntry(const std::vector<uint32_t>& words) const -> uint32_t {
    const auto entryCount = ReadDictionaryNumberOfEntries();
    const auto entryLength = ReadDictionaryEntryLength();
    auto address = GetDictionaryEntriesAddress();
    for (auto i = 0u; i < entryCount; ++i) {
        if (TestDictionaryEntry(address, words)) {
            return address;
        }
        address += entryLength;
    }
    return 0u;
}