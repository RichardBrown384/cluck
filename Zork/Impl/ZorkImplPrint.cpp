#include "ZorkImpl.h"
#include "ZorkImplString.h"

#include "ZorkCharacterStream.h"
#include "ZorkError.h"

#include <iostream>

using namespace fiction::zork;

auto ZorkImpl::PrintCharacter(uint32_t character) -> void {
    // TODO map from ZSCII
    std::cout << static_cast<char>(character);
}

auto ZorkImpl::PrintNumber(uint32_t number) -> void {
    std::cout << number;
}

auto ZorkImpl::PrintString(uint32_t address, bool abbreviation) -> uint32_t {
    constexpr auto DECODE_STATE_NORMAL = 0u;
    constexpr auto DECODE_STATE_ABBREVIATION = 1u;
    constexpr auto DECODE_STATE_ZSCII_HIGH = 2u;
    constexpr auto DECODE_STATE_ZSCII_LOW = 3u;

    auto ShiftUp = [](auto a) { return (a + 1) % 3; };

    auto ShiftDown = [](auto a) { return (a + 2) % 3; };

    auto IsZsciiEscape = [](uint32_t alphabet, uint32_t character) {
        return alphabet == ALPHABET_PUNCTUATION && character == PUNCTUATION_ESCAPE;
    };

    const auto alphabetTable = (header.version < 2) ? ALPHABET_TABLE_V1 : ALPHABET_TABLE_V2;

    auto alphabetPrevious = ALPHABET_LOWER;
    auto alphabetCurrent = ALPHABET_LOWER;
    auto state = DECODE_STATE_NORMAL;
    auto intermediate = 0u;

    std::function<void(uint32_t)> DecodeCharacterVersion2 = [&](uint32_t c) {
        if (c == ESCAPE_SPACE) {
            PrintCharacter(CODE_SPACE);
        } else if (c == ESCAPE_NEW_LINE) {
            if (header.version < 2u) {
                PrintCharacter(CODE_NEWLINE);
            } else if (abbreviation) {
                Error("Nested abbreviation");
            } else {
                state = DECODE_STATE_ABBREVIATION;
                intermediate = (c - 1u) << 5u;
            }
        } else if (c == ESCAPE_SHIFT_UP) {
            alphabetPrevious = alphabetCurrent;
            alphabetCurrent = ShiftUp(alphabetCurrent);
        } else if (c == ESCAPE_SHIFT_DOWN) {
            alphabetPrevious = alphabetCurrent;
            alphabetCurrent = ShiftDown(alphabetCurrent);
        } else if (c == ESCAPE_SHIFT_LOCK_UP) {
            alphabetPrevious = ShiftUp(alphabetCurrent);
            alphabetCurrent = alphabetPrevious;
        } else if (c == ESCAPE_SHIFT_LOCK_DOWN) {
            alphabetPrevious = ShiftDown(alphabetCurrent);
            alphabetCurrent = alphabetPrevious;
        } else {
            if (IsZsciiEscape(alphabetCurrent, c)) {
                state = DECODE_STATE_ZSCII_HIGH;
            } else {
                const auto character = alphabetTable[alphabetCurrent][c - 6u];
                PrintCharacter(character);
            }
            alphabetCurrent = alphabetPrevious;
        }
    };

    std::function<void(uint32_t)> DecodeCharacterVersion3 = [&](uint32_t c)  {
        if (c == ESCAPE_SPACE) {
            PrintCharacter(CODE_SPACE);
        } else if (c < ESCAPE_SHIFT_LOCK_UP) {
            if (abbreviation) {
                Error("Nested abbreviation");
            }
            state = DECODE_STATE_ABBREVIATION;
            intermediate = (c - 1u) << 5u;
        } else if (c == ESCAPE_SHIFT_LOCK_UP) {
            alphabetCurrent = ALPHABET_UPPER;
        } else if (c == ESCAPE_SHIFT_LOCK_DOWN) {
            alphabetCurrent = ALPHABET_PUNCTUATION;
        } else {
            if (IsZsciiEscape(alphabetCurrent, c)) {
                state = DECODE_STATE_ZSCII_HIGH;
            } else {
                const auto character = alphabetTable[alphabetCurrent][c - 6u];
                PrintCharacter(character);
            }
            alphabetCurrent = ALPHABET_LOWER;
        }
    };

    auto DecodeCharacter = (header.version < 3u) ? DecodeCharacterVersion2 : DecodeCharacterVersion3;

    ZorkCharacterStream stream(*this, address);
    while (!stream.Done()) {
        const auto c = stream.Next();
        if (state == DECODE_STATE_NORMAL) {
            DecodeCharacter(c);
        } else if (state == DECODE_STATE_ABBREVIATION) {
            state = DECODE_STATE_NORMAL;
            const auto abbreviationAddress = GetAbbreviationAddress(intermediate + c);
            PrintString(abbreviationAddress, true);
        } else if (state == DECODE_STATE_ZSCII_HIGH) {
            state = DECODE_STATE_ZSCII_LOW;
            intermediate = (c << 5);
        } else if (state == DECODE_STATE_ZSCII_LOW) {
            state = DECODE_STATE_NORMAL;
            PrintCharacter(intermediate + c);
        }
    }

    if (abbreviation && state != DECODE_STATE_NORMAL) {
        Error("Abbreviation ended in incomplete zscii character");
    }

    return stream.GetAddress();
}

auto ZorkImpl::ReadLine() -> std::string {
    std::string line;
    std::getline(std::cin, line);
    std::transform(line.begin(), line.end(), line.begin(), [](auto c) { return std::tolower(c); });
    line.push_back('\0');
    return line;
}