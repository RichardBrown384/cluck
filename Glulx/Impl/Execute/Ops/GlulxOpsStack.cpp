#include "Impl/GlulxImpl.h"

#include "Impl/Execute/Instruction/GlulxInstruction.h"

namespace fiction::glulx {

auto ExecuteStkcount(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    StoreOperandWrite32(glulx, s1, glulx.StackGetCount());
}

auto ExecuteStkpeek(GlulxImpl& glulx) -> void {
    const auto& [l1, s1] = OperandsReadLS(glulx);
    StoreOperandWrite32(glulx, s1, glulx.StackPeek(l1.value));
}

auto ExecuteStkswap(GlulxImpl& glulx) -> void {
    glulx.StackRotate(2u, 1u);
}

auto ExecuteStkroll(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.StackRotate(l1.value, l2.value);
}

auto ExecuteStkcopy(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    glulx.StackCopy(l1.value);
}

}