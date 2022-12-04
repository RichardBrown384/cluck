#include "Impl/GlulxImpl.h"

namespace {
constexpr auto GESTALT_SELECTOR_VERSION_GLULX = 0u;
constexpr auto GESTALT_SELECTOR_VERSION_INTERPRETER = 1u;
constexpr auto GESTALT_SELECTOR_RESIZE_MEM = 2u;
constexpr auto GESTALT_SELECTOR_UNDO = 3u;
constexpr auto GESTALT_SELECTOR_IO_SYSTEM = 4u;
constexpr auto GESTALT_SELECTOR_UNICODE = 5u;
constexpr auto GESTALT_SELECTOR_MEM_COPY = 6u;
constexpr auto GESTALT_SELECTOR_MALLOC = 7u;
constexpr auto GESTALT_SELECTOR_MALLOC_HEAP = 8u;
constexpr auto GESTALT_SELECTOR_ACCELERATION = 9u;
constexpr auto GESTALT_SELECTOR_ACCEL_FUNC = 10u;
constexpr auto GESTALT_SELECTOR_FLOAT = 11u;
constexpr auto GESTALT_SELECTOR_UNDO_EXT = 12u;
constexpr auto GESTALT_SELECTOR_DOUBLE = 13u;
}

namespace fiction::glulx {

constexpr auto MakeVersion(auto major, auto minor, auto revision) {
    return (major << 16u) + ((minor & 0xFFu) << 8u) + (revision & 0xFFu);
}

auto Gestalt(GlulxImpl& glulx, uint32_t selector, uint32_t argument) -> uint32_t {
    switch (selector) {
        case GESTALT_SELECTOR_VERSION_GLULX:
            return MakeVersion(3, 1, 3);
        case GESTALT_SELECTOR_VERSION_INTERPRETER:
            return MakeVersion(0, 0, 1);
        case GESTALT_SELECTOR_IO_SYSTEM:
            return glulx.IsInputOutputSystemSupported(argument);
        case GESTALT_SELECTOR_MALLOC_HEAP:
            return glulx.GetHeapStart();
        case GESTALT_SELECTOR_ACCEL_FUNC:
            return glulx.IsAccelerationFunctionSupported(argument);
        case GESTALT_SELECTOR_RESIZE_MEM:
        case GESTALT_SELECTOR_UNDO:
        case GESTALT_SELECTOR_UNICODE:
        case GESTALT_SELECTOR_MEM_COPY:
        case GESTALT_SELECTOR_MALLOC:
        case GESTALT_SELECTOR_ACCELERATION:
        case GESTALT_SELECTOR_FLOAT:
        case GESTALT_SELECTOR_UNDO_EXT:
        case GESTALT_SELECTOR_DOUBLE:
            return 1u;
        default: return 0u;
    }
}

}