#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxInstruction.h"

#include "Impl/Execute/GlulxExecuteConstants.h"

#include "Util/BitCast.h"

namespace fiction::glulx {

auto StoreOperandWrite8(GlulxImpl& glulx, const GlulxOperand& operand, uint32_t value) -> void {
    const auto truncated = value & 0xFFu;
    switch (operand.desttype) {
        case DESTINATION_NONE: return;
        case DESTINATION_MEMORY: return glulx.MemoryWrite8(operand.value, truncated);
        case DESTINATION_LOCAL: return glulx.StackWriteLocal8(operand.value, truncated);
        case DESTINATION_STACK: return glulx.StackPush32(truncated);
        default: Error("Unsupported 8-bit store operand destination");
    }
}

auto StoreOperandWrite16(GlulxImpl& glulx, const GlulxOperand& operand, uint32_t value) -> void {
    const auto truncated = value & 0xFFFFu;
    switch (operand.desttype) {
        case DESTINATION_NONE: return;
        case DESTINATION_MEMORY: return glulx.MemoryWrite16(operand.value, truncated);
        case DESTINATION_LOCAL: return glulx.StackWriteLocal16(operand.value, truncated);
        case DESTINATION_STACK: return glulx.StackPush32(truncated);
        default: Error("Unsupported 16-bit store operand destination");
    }
}


auto StoreOperandWrite32(GlulxImpl& glulx, const GlulxOperand& operand, uint32_t value) -> void {
    switch (operand.desttype) {
        case DESTINATION_NONE: return;
        case DESTINATION_MEMORY: return glulx.MemoryWrite32(operand.value, value);
        case DESTINATION_LOCAL: return glulx.StackWriteLocal32(operand.value, value);
        case DESTINATION_STACK: return glulx.StackPush32(value);
        default: Error("Unsupported 32-bit store operand destination");
    }
}

auto StoreOperandWriteFloat(GlulxImpl& glulx, const GlulxOperand& operand, float value) -> void {
    StoreOperandWrite32(glulx, operand, BitCast<uint32_t>(value));
}

auto StoreOperandWriteDouble(GlulxImpl& glulx,
                             const GlulxOperand& operand1,
                             const GlulxOperand& operand2,
                             double value) -> void {
    const auto x = BitCast<uint64_t>(value);
    StoreOperandWrite32(glulx, operand1, x & 0xFFFFFFFFu);
    StoreOperandWrite32(glulx, operand2, x >> 32u);
}

}