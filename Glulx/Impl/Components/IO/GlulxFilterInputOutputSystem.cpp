#include "Impl/GlulxImpl.h"

#include "GlulxFilterInputOutputSystem.h"
#include "Impl/Execute/ControlFlow/GlulxFunction.h"

namespace fiction::glulx {

auto GlulxFilterInputOutputSystem::PutChar8(GlulxImpl& glulx, uint32_t c) -> void {
    EnterFunction(glulx, glulx.GetInputOutputRock(), { c & 0xFFu });
}

auto GlulxFilterInputOutputSystem::PutChar32(GlulxImpl& glulx, uint32_t c) -> void {
    EnterFunction(glulx, glulx.GetInputOutputRock(), { c });
}

}

