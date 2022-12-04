#pragma once

namespace fiction::glulx {

constexpr auto FUNCTION_STACK_ARGUMENT = 0xC0u;
constexpr auto FUNCTION_LOCAL_ARGUMENT = 0xC1u;

constexpr auto STRING_C = 0xE0;
constexpr auto STRING_COMPRESSED = 0x0E1;
constexpr auto STRING_UNICODE = 0xE2;

constexpr auto DESTINATION_NONE = 0u;
constexpr auto DESTINATION_MEMORY = 1u;
constexpr auto DESTINATION_LOCAL = 2u;
constexpr auto DESTINATION_STACK = 3u;
constexpr auto DESTINATION_RESUME_PRINTING_COMPRESSED_STRING = 10u;
constexpr auto DESTINATION_RESUME_EXECUTING = 11u;
constexpr auto DESTINATION_RESUME_PRINTING_INTEGER = 12u;
constexpr auto DESTINATION_RESUME_PRINTING_C_STRING = 13u;
constexpr auto DESTINATION_RESUME_PRINTING_UNICODE_STRING = 14u;

constexpr auto GLULX_SUCCESS = 0u;
constexpr auto GLULX_FAILURE = 1u;
constexpr auto GLULX_RESTORED = 0xFFFFFFFFu;

constexpr auto IsFunctionType(auto type) -> bool {
    return type == FUNCTION_STACK_ARGUMENT || type == FUNCTION_LOCAL_ARGUMENT;
}

constexpr auto IsStringType(auto type) -> bool {
    return type == STRING_C || type == STRING_COMPRESSED || type == STRING_UNICODE;
}

}