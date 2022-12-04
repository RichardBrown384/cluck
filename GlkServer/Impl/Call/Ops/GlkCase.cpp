#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallCharToLower(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [ch] = TakeFirst<1>(arguments);
    return glk_char_to_lower(ch);
}

auto GlkServerImpl::CallCharToUpper(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [ch] = TakeFirst<1>(arguments);
    return glk_char_to_upper(ch);
}

auto GlkServerImpl::CallBufferToLowerCaseUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, len, numChars] = TakeFirst<3>(arguments);
    auto buffer = ReadArray32(address, len);
    auto result = glk_buffer_to_lower_case_uni(buffer.data(), len, numChars);
    WriteArray32(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallBufferToUpperCaseUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, len, numChars] = TakeFirst<3>(arguments);
    auto buffer = ReadArray32(address, len);
    auto result = glk_buffer_to_upper_case_uni(buffer.data(), len, numChars);
    WriteArray32(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallBufferToTitleCaseUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, len, numChars, lowerRest] = TakeFirst<4>(arguments);
    auto buffer = ReadArray32(address, len);
    auto result = glk_buffer_to_title_case_uni(buffer.data(), len, numChars, lowerRest);
    WriteArray32(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallBufferCanonDecomposeUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, len, numChars] = TakeFirst<3>(arguments);
    auto buffer = ReadArray32(address, len);
    auto result = glk_buffer_canon_decompose_uni(buffer.data(), len, numChars);
    WriteArray32(buffer.data(), len, address);
    return result;
}

auto GlkServerImpl::CallBufferCanonNormalizeUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [address, len, numChars] = TakeFirst<3>(arguments);
    auto buffer = ReadArray32(address, len);
    auto result = glk_buffer_canon_normalize_uni(buffer.data(), len, numChars);
    WriteArray32(buffer.data(), len, address);
    return result;
}

}