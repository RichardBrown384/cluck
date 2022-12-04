#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Execute/Stream/GlulxStream.h"

namespace fiction::glulx {

auto ExecuteStreamchar(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    StreamChar(glulx, l1.value);
}

auto ExecuteStreamnum(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    StreamInteger(glulx, l1.value);
}

auto ExecuteStreamstr(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    StreamString(glulx, l1.value);
}

auto ExecuteStreamunichar(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    StreamUnicodeChar(glulx, l1.value);
}

auto ExecuteGetstringtbl(GlulxImpl& glulx) -> void {
    const auto& [s1] = OperandsReadS(glulx);
    StoreOperandWrite32(glulx, s1, glulx.GetStringTable());
}

auto ExecuteSetstringtbl(GlulxImpl& glulx) -> void {
    const auto& [l1] = OperandsReadL(glulx);
    glulx.SetStringTable(l1.value);
}

auto ExecuteGetiosys(GlulxImpl& glulx) -> void {
    const auto& [s1, s2] = OperandsReadSS(glulx);
    StoreOperandWrite32(glulx, s1, glulx.GetInputOutputSystem());
    StoreOperandWrite32(glulx, s2, glulx.GetInputOutputRock());
}

auto ExecuteSetiosys(GlulxImpl& glulx) -> void {
    const auto& [l1, l2] = OperandsReadLL(glulx);
    glulx.SetInputOutputSystemAndRock(l1.value, l2.value);
}

}