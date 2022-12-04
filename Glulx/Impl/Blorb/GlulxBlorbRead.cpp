#include "Impl/Serial/GlulxSerial.h"

#include "Impl/Serial/GlulxReadBuffer.h"
#include "Impl/Serial/GlulxIff.h"

namespace fiction::glulx {

auto ExtractGlulExecutable(const std::vector<uint8_t>& data) -> std::vector<uint8_t> {

    GlulxReadBuffer buffer(data);

    if (!IsFormHeaderValid(buffer, IFRS)) {
        return {};
    }

    return ExtractChunk(buffer, GLUL);
}

}