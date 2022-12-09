#include "GlulxIff.h"

#include "GlulxReadBuffer.h"

namespace fiction::glulx {

auto IsFormHeaderValid(GlulxReadBuffer& buffer, uint32_t requiredFormType) -> bool {
    const auto form = buffer.Read32();
    const auto length = buffer.Read32();
    const auto formType = buffer.Read32();
    return form == FORM &&
           length == buffer.GetSize() - 8u &&
           formType == requiredFormType;
}

auto LocateChunk(GlulxReadBuffer& buffer, uint32_t searchType) -> std::pair<uint32_t, uint32_t> {
    while (buffer.GetPosition() < buffer.GetSize()) {
        const auto type = buffer.Read32();
        const auto length = buffer.Read32();
        if (type == searchType) {
            return { buffer.GetPosition(), length };
        }
        buffer.Skip(length);
        buffer.Align16();
    }
    return { 0u, 0u };
}

auto ExtractChunk(GlulxReadBuffer& buffer, uint32_t searchType) -> std::vector<uint8_t> {

    const auto& [position, length] = LocateChunk(buffer, searchType);
    if (length == 0u) {
        return {};
    }

    buffer.SetPosition(position);

    return buffer.ReadBuffer(length);

}

}
