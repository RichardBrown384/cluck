#include "ZorkTokenizer.h"

#include "Impl/Components/Dictionary/ZorkDictionary.h"
#include "Impl/Header/ZorkHeader.h"
#include "Impl/ZorkImplString.h"

using namespace fiction::zork;

namespace {

struct Word {
    Word(std::vector<char> w, uint32_t l, uint32_t s) : word(std::move(w)), length(l), start(s) {}
    std::vector<char> word;
    uint32_t length;
    uint32_t start;
};

auto SplitString(const std::string& input, const std::vector<uint32_t>& separators) -> std::vector<Word> {
    std::vector<Word> words;

    auto PushWord = [&words](std::vector<char> word, uint32_t start) {
        if (!word.empty()) {
            words.emplace_back(std::move(word), word.size(), start);
        }
    };

    std::vector<char> word;
    auto start = 0u;
    for (auto i = 0u; i < input.size(); ++i) {
        const auto c = input[i];
        if (c == ' ' || c == '\0') {
            PushWord(word, start);
            word.clear();
            start = i + 1;
        } else if (std::find(std::begin(separators), std::end(separators), c) != std::end(separators)) {
            PushWord(word, start);
            PushWord({c}, i);
            word.clear();
            start = i + 1;
        } else {
            word.push_back(c);
        }
    }

    return words;
}

auto EncodeCharacter(char c, const std::string& symbolAlphabet) -> std::vector<uint32_t> {
    std::vector<uint32_t> encoded;
    if (c == ' ') {
        encoded.push_back(ESCAPE_SPACE);
    } else if (c >= 'a' && c <= 'z') {
        encoded.push_back(6 + c - 'a');
    } else if (c >= 'A' && c <= 'Z') {
        encoded.push_back(ESCAPE_SHIFT_LOCK_UP);
        encoded.push_back(6 + c - 'A');
    } else {
        encoded.push_back(ESCAPE_SHIFT_LOCK_DOWN);
        const auto index = symbolAlphabet.find(c);
        if (index != std::string::npos) {
            encoded.push_back(6 + index);
        } else {
            encoded.push_back(6 + PUNCTUATION_ESCAPE);
            encoded.push_back((c >> 5) & 0x1F);
            encoded.push_back(c & 0x1F);
        }
    }
    return encoded;
}

auto EncodeString(const std::vector<char>& text, uint32_t length, const char* symbolAlphabet) -> std::vector<uint32_t>{
    std::vector<uint32_t> encoded;
    for (auto i = 0u; i < length; ++i) {
        if (i < text.size()) {
            const auto& character = EncodeCharacter(text[i], symbolAlphabet);
            encoded.insert(encoded.end(), character.begin(), character.end());
        } else {
            encoded.push_back(ESCAPE_SHIFT_LOCK_DOWN);
        }
    }
    encoded.resize(length);
    return encoded;
}

auto PackString(const std::vector<uint32_t>& encodedString) {
    std::vector<uint32_t> packed;
    for (auto i = 0u; i < encodedString.size(); i += 3) {
        packed.push_back((encodedString[i] << 10)
                         + (encodedString[i + 1] << 5)
                         + (encodedString[i + 2]));
    }
    packed[packed.size() - 1u] |= 0x8000u;
    return packed;
}
}

ZorkTokenizer::ZorkTokenizer(ZorkDictionary& dictionary_in, const ZorkHeader& header_in) :
    dictionary(dictionary_in),
    header(header_in) {}

ZorkTokenizer::~ZorkTokenizer() = default;

auto ZorkTokenizer::TokenizeString(const std::string& text) const -> std::vector<ZorkToken> {
    const auto alphabetTable = (header.version < 2) ? ALPHABET_TABLE_V1 : ALPHABET_TABLE_V2;
    const auto symbolAlphabet = alphabetTable[ALPHABET_PUNCTUATION];

    const auto& separators = dictionary.ReadDictionaryWordSeparators();
    const auto& words = SplitString(text, separators);

    std::vector<ZorkToken> result;
    for (const auto& [word, length, start] : words) {
        const auto encoded = EncodeString(word, 6, symbolAlphabet);
        const auto packed = PackString(encoded);
        const auto entry = dictionary.FindDictionaryEntry(packed);
        result.emplace_back(entry, length, 1u + start);
    }
    return result;
}