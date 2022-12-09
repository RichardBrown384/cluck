#pragma once

namespace fiction::glulx {

constexpr auto MakeVersion(auto major, auto minor, auto revision) {
    return (major << 16u) + ((minor & 0xFFu) << 8u) + (revision & 0xFFu);
}

constexpr auto MIN_VERSION = MakeVersion(2u, 0u, 0u);
constexpr auto MAX_VERSION = MakeVersion(3u, 1u, 3u);

constexpr auto INTERPRETER_VERSION = MakeVersion(0, 0, 1);
}