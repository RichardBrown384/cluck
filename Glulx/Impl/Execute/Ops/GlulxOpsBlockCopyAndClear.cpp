#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteMzero(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    const auto count = l1.value;
    const auto address = l2.value;
    for (auto i = 0u; i < count; ++i) {
        glulx.MemoryWrite8(address + i, 0u);
    }

}

auto ExecuteMcopy(GlulxImpl& glulx) -> void {
    const auto& [l1, l2, l3] = OperandsReadLLL(glulx);
    const auto count = l1.value;
    const auto source = l2.value;
    const auto target = l3.value;
    if (target > source) {
        for (auto i = 0u; i < count; ++i) {
            const auto j = count - 1u - i;
            glulx.MemoryWrite8(target + j, glulx.MemoryRead8(source + j));
        }
    } else {
        for (auto i = 0u; i < count; ++i) {
            glulx.MemoryWrite8(target + i, glulx.MemoryRead8(source + i));
        }
    }

}

}