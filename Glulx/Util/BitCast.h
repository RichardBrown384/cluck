// Note: This is a trivial replacement for std::bit_cast
// for C++20 compilers that don't support it.
// Missing some important things like checking to and from are the same
// size and are trivially copyable.
// Caveat emptor

#pragma once

#include <cstring>

namespace {
template<typename To, typename From>
auto BitCast(const From& from) {
    To to;
    std::memcpy(&to, &from, sizeof(to));
    return to;
}
}