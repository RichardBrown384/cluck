#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteRandom(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    const auto modulus = static_cast<int32_t>(l1.value);
    const auto random = glulx.RandomNext(modulus);
    StoreOperandWrite32(glulx, s1, random);
}


auto ExecuteSetrandom(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    glulx.RandomSeed(l1.value);
}

}