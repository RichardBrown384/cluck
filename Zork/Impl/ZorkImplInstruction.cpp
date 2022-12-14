#include "ZorkImpl.h"
#include "Common/MathsUtil.h"

using namespace fiction::zork;

namespace {
constexpr auto TYPE_LARGE_CONSTANT = 0u;
constexpr auto TYPE_SMALL_CONSTANT = 1u;
constexpr auto TYPE_VARIABLE = 2u;
}

auto ZorkImpl::ReadInstruction8() -> uint32_t {
    const auto pc = GetProgramCounter();
    const auto b = MemoryRead8(pc);
    SetProgramCounter(pc + 1u);
    return b;
}

auto ZorkImpl::ReadInstruction16() -> uint32_t  {
    const auto pc = GetProgramCounter();
    const auto w = MemoryRead16(pc);
    SetProgramCounter(pc + 2);
    return w;
}

auto ZorkImpl::ReadInstructionLargeConstant() -> uint32_t {
    return ReadInstruction16();
}

auto ZorkImpl::ReadInstructionSmallConstant() -> uint32_t {
    return ReadInstruction8();
}

auto ZorkImpl::ReadInstructionVariable() -> uint32_t {
    const auto variable = ReadInstruction8();
    return ReadVariable(variable);
}

auto ZorkImpl::ReadInstructionOperands(uint32_t operandTypes) -> std::vector<uint32_t> {
    std::vector<uint32_t> operands;
    for (auto i = 0u; i < 4u; ++i) {
        const auto shift = 6u - 2u * i;
        const auto operandType = (operandTypes >> shift) & 0x3u;
        if (operandType == TYPE_LARGE_CONSTANT) {
            operands.push_back(ReadInstructionLargeConstant());
        } else if (operandType == TYPE_SMALL_CONSTANT) {
            operands.push_back(ReadInstructionSmallConstant());
        } else if (operandType == TYPE_VARIABLE) {
            operands.push_back(ReadInstructionVariable());
        } else {
            break;
        }
    }
    return operands;
}

auto ZorkImpl::ReadInstructionBranch() -> std::tuple<bool, uint32_t> {
    const auto far = ReadInstruction8();
    const auto negate = (far & 0x80u) == 0u;
    if ((far & 0x40u) == 0u) {
        const auto near = ReadInstruction8();
        const auto unsignedOffset = ((far & 0x3Fu) << 8u) + near;
        const auto offset = SignExtend(unsignedOffset, 14u);
        return { negate, offset };
    }
    const auto offset = far & 0x3Fu;
    return { negate, offset };
}