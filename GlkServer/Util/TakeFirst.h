#pragma once

#include <utility>
#include <tuple>

namespace {

// Note: This is so we can use structured bindings to unpack the arguments
// to the Glk calls. Unpacking them directly by array index was somewhat
// error-prone. This could potentially be replaced by ranges.
// Taken from https://stackoverflow.com/a/28411055

template <auto... Indices>
auto TakeFirstVariadic(const auto& v, std::index_sequence<Indices...>) {
    return std::make_tuple(v.at(Indices)...);
}

template<auto n>
auto TakeFirst(const auto& v) {
    return TakeFirstVariadic(v, std::make_index_sequence<n>());
}

}