#pragma once

#include <array>

namespace {
constexpr auto ALPHABET_LOWER = 0u;
constexpr auto ALPHABET_UPPER = 1u;
constexpr auto ALPHABET_PUNCTUATION = 2u;

constexpr std::array<const char*, 3> ALPHABET_TABLE_V1 = {
    "abcdefghijklmnopqrstuvwxyz",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    R"( 0123456789.,!?_#'"/\<-:())"
};

constexpr std::array<const char*, 3> ALPHABET_TABLE_V2 = {
    "abcdefghijklmnopqrstuvwxyz",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    " \n0123456789.,!?_#\'\"/\\-:()"
};

constexpr auto ESCAPE_SPACE = 0u;
constexpr auto ESCAPE_NEW_LINE = 1u;
constexpr auto ESCAPE_SHIFT_UP = 2u;
constexpr auto ESCAPE_SHIFT_DOWN = 3u;
constexpr auto ESCAPE_SHIFT_LOCK_UP = 4u;
constexpr auto ESCAPE_SHIFT_LOCK_DOWN = 5u;

constexpr auto PUNCTUATION_ESCAPE = 6u;

constexpr auto CODE_SPACE = ' ';
constexpr auto CODE_NEWLINE = '\n';
}