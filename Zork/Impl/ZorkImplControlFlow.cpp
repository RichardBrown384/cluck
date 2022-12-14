#include "ZorkImpl.h"

using namespace fiction::zork;

auto ZorkImpl::DoCall(uint32_t target, uint32_t variable, const std::vector<uint32_t>& arguments) -> void {
    if (target == 0) {
        WriteVariable(variable, 0);
        return;
    }
    const auto returnAddress = GetProgramCounter();
    SetProgramCounter(GetUnpackedAddress(target));
    const auto localCount = ReadInstruction8();
    std::vector<uint32_t> locals(localCount);
    for (auto i = 0u; i < localCount; ++i) {
        locals[i] = ReadInstruction16();
    }
    PushStackFrame(returnAddress, variable, arguments, locals);
}

auto ZorkImpl::DoReturn(uint32_t value) -> void {
    const auto& [returnAddress, flags, variable] = PopStackFrame();
    WriteVariable(variable, value);
    SetProgramCounter(returnAddress);
}

auto ZorkImpl::DoBranch(const std::tuple<bool, uint32_t>& branch, bool condition) -> void {
    const auto& [negate, offset] = branch;
    if (condition ^ negate) {
        if (offset == 0u || offset == 1u) {
            DoReturn(offset);
        } else {
            const auto pc = GetProgramCounter();
            SetProgramCounter(pc + offset - 2u);
        }
    }
}