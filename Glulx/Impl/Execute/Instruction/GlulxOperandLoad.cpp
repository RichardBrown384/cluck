#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxInstruction.h"

#include "Impl/Execute/GlulxExecuteConstants.h"

#include "Common/MathsUtil.h"

namespace fiction::glulx {
namespace {

constexpr auto LoadOperandMemory(auto& glulx, auto address, auto size) {
    switch (size) {
        case OPERAND_SIZE_8: return glulx.MemoryRead8(address);
        case OPERAND_SIZE_16: return glulx.MemoryRead16(address);
        case OPERAND_SIZE_32: return glulx.MemoryRead32(address);
        default:
            Error("Unsupported read operand size");
    }
}

constexpr auto LoadOperandRam(auto& glulx, auto address, auto size) {
    return LoadOperandMemory(glulx, glulx.GetRamStart() + address, size);
}

constexpr auto LoadOperandLocals(auto& glulx, auto address, auto size) {
    switch (size) {
        case OPERAND_SIZE_8: return  glulx.StackReadLocal8(address);
        case OPERAND_SIZE_16: return glulx.StackReadLocal16(address);
        case OPERAND_SIZE_32: return glulx.StackReadLocal32(address);
        default:
            Error("Unsupported load operand size");
    }
}

constexpr auto LoadOperandReadValue(auto& glulx, auto mode, auto size) -> uint32_t {
    switch (mode) {
        case MODE_CONSTANT_ZERO: return 0u;
        case MODE_CONSTANT_SIGNED_8: return SignExtend8(OpcodeStreamRead8(glulx));
        case MODE_CONSTANT_SIGNED_16: return SignExtend16(OpcodeStreamRead16(glulx));
        case MODE_CONSTANT_SIGNED_32: return OpcodeStreamRead32(glulx);
        case MODE_ADDRESS_8: return LoadOperandMemory(glulx, OpcodeStreamRead8(glulx), size);
        case MODE_ADDRESS_16: return LoadOperandMemory(glulx, OpcodeStreamRead16(glulx), size);
        case MODE_ADDRESS_32: return LoadOperandMemory(glulx, OpcodeStreamRead32(glulx), size);
        case MODE_STACK: return glulx.StackPop32();
        case MODE_CALL_FRAME_8: return LoadOperandLocals(glulx, OpcodeStreamRead8(glulx), size);
        case MODE_CALL_FRAME_16: return LoadOperandLocals(glulx, OpcodeStreamRead16(glulx), size);
        case MODE_CALL_FRAME_32: return LoadOperandLocals(glulx, OpcodeStreamRead32(glulx), size);
        case MODE_RAM_8: return LoadOperandRam(glulx, OpcodeStreamRead8(glulx), size);
        case MODE_RAM_16: return LoadOperandRam(glulx, OpcodeStreamRead16(glulx), size);
        case MODE_RAM_32: return LoadOperandRam(glulx, OpcodeStreamRead32(glulx), size);
        default: Error("Unsupported operand load address mode");
    }
}
}

auto LoadOperandRead(GlulxImpl& glulx, uint32_t mode, uint32_t size) -> GlulxOperand {
    return { DESTINATION_NONE, LoadOperandReadValue(glulx, mode, size) };
}
}