#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxInstruction.h"
#include "Impl/Execute/GlulxExecuteConstants.h"

#include "Util/MathsUtil.h"

namespace fiction::glulx {
namespace {

constexpr auto StoreOperandDiscard() -> GlulxOperand {
    return { DESTINATION_NONE, 0u };
}

constexpr auto StoreOperandStack() -> GlulxOperand {
    return { DESTINATION_STACK, 0u };
}

constexpr auto StoreOperandMemory(auto address) -> GlulxOperand {
    return { DESTINATION_MEMORY, address };
}

constexpr auto StoreOperandRam(auto& glulx, auto address) -> GlulxOperand {
    return StoreOperandMemory(glulx.GetRamStart() + address);
}

constexpr auto StoreOperandLocals(auto address) -> GlulxOperand {
    return { DESTINATION_LOCAL, address};
}
}

auto StoreOperandRead(GlulxImpl& glulx, uint32_t mode) -> GlulxOperand {
    switch (mode) {
        case MODE_CONSTANT_ZERO: return StoreOperandDiscard();
        case MODE_ADDRESS_8: return StoreOperandMemory(OpcodeStreamRead8(glulx));
        case MODE_ADDRESS_16: return StoreOperandMemory(OpcodeStreamRead16(glulx));
        case MODE_ADDRESS_32: return StoreOperandMemory(OpcodeStreamRead32(glulx));
        case MODE_STACK: return StoreOperandStack();
        case MODE_CALL_FRAME_8:  return StoreOperandLocals(OpcodeStreamRead8(glulx));
        case MODE_CALL_FRAME_16: return StoreOperandLocals(OpcodeStreamRead16(glulx));
        case MODE_CALL_FRAME_32: return StoreOperandLocals(OpcodeStreamRead32(glulx));
        case MODE_RAM_8: return StoreOperandRam(glulx, OpcodeStreamRead8(glulx));
        case MODE_RAM_16: return StoreOperandRam(glulx, OpcodeStreamRead16(glulx));
        case MODE_RAM_32: return StoreOperandRam(glulx, OpcodeStreamRead32(glulx));
        default: Error("Unsupported store operand address mode");
    }
}
}