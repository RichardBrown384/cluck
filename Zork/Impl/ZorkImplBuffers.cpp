#include "ZorkImpl.h"

using namespace fiction::zork;

auto ZorkImpl::WriteToTextBuffer(uint32_t textBuffer, const std::string& text) -> void {
    const auto textPosition = textBuffer + 1u;
    for (auto i = 0u; i < text.size(); ++i) {
        MemoryWrite8(textPosition + i, text[i]);
    }
}

auto ZorkImpl::WriteToParseBuffer(uint32_t parseBuffer, const std::vector<ZorkToken>& entries) -> void {
    MemoryWrite8(parseBuffer + 1, entries.size());
    auto parsedPosition = parseBuffer + 2;
    for (const auto& [entry, length, start]: entries) {
        MemoryWrite16(parsedPosition, entry);
        MemoryWrite8(parsedPosition + 2, length);
        MemoryWrite8(parsedPosition + 3, start);
        parsedPosition += 4;
    }
}