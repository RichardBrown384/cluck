#include "Impl/GlulxImpl.h"

#include "GlulxCallReturn.h"
#include "GlulxCallStub.h"
#include "GlulxFunction.h"

namespace fiction::glulx {

auto Call(GlulxImpl& glulx,
          uint32_t address,
          uint32_t destinationType,
          uint32_t destinationValue,
          const std::vector<uint32_t>& arguments) -> void {
    PushCallStub(glulx, destinationType, destinationValue);
    EnterFunction(glulx, address, arguments);
}

auto TailCall(GlulxImpl& glulx,
              uint32_t address,
              const std::vector<uint32_t>& arguments) -> void {
    LeaveFunction(glulx);
    EnterFunction(glulx, address, arguments);
}

auto Return(GlulxImpl& glulx, uint32_t returnValue) -> void {
    LeaveFunction(glulx);
    if (glulx.GetStackPointer() == 0u) {
        glulx.SetRunning(false);
        return;
    }
    PopCallStub(glulx, returnValue);
}

auto Jump(GlulxImpl& glulx, uint32_t offset) -> void {
    if (offset == 0u || offset == 1u) {
        Return(glulx, offset);
        return;
    }
    const auto pc = glulx.GetProgramCounter();
    const auto target = pc + offset - 2u;
    glulx.SetProgramCounter(target);
}

}