#include "ZorkStack.h"

#include "Common/Endian.h"

using namespace fiction::zork;
using namespace fiction::endian;

ZorkStack::ZorkStack() : framePointers(), stack(4096), stackPointer(0u), framePointer(0u) {}

ZorkStack::~ZorkStack() = default;

auto ZorkStack::Restart() -> void {
    framePointers.clear();
    SetStackPointer(0u);
    SetFramePointer(0u);
}

auto ZorkStack::GetStackPointer() const -> uint32_t { return stackPointer; }
auto ZorkStack::SetStackPointer(uint32_t v) -> void { stackPointer = v; }

auto ZorkStack::Push8(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    SetStackPointer(sp + 1u);
    ::Write8(stack, sp, v);
}

auto ZorkStack::Push16(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    SetStackPointer(sp + 2u);
    ::Write16(stack, sp, v);
}

auto ZorkStack::Push24(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    SetStackPointer(sp + 3u);
    ::Write24(stack, sp, v);
}

auto ZorkStack::Pop8() -> uint32_t {
    const auto sp = GetStackPointer() - 1u;
    SetStackPointer(sp);
    return ::Read8(stack, sp);
}

auto ZorkStack::Pop16() -> uint32_t {
    const auto sp = GetStackPointer() - 2u;
    SetStackPointer(sp);
    return ::Read16(stack, sp);
}

auto ZorkStack::Pop24() -> uint32_t {
    const auto sp = GetStackPointer() - 3u;
    SetStackPointer(sp);
    return ::Read24(stack, sp);
}

auto ZorkStack::GetFramePointer() const -> uint32_t { return framePointer; }
auto ZorkStack::SetFramePointer(uint32_t v) -> void { framePointer = v; }

auto ZorkStack::PushFramePointer() -> void { framePointers.push_back(framePointer); }
auto ZorkStack::PopFramePointer() -> void { framePointer = framePointers.back(); framePointers.pop_back(); }

// Note: We use the Quetzal frame format here
// http://inform-fiction.org/zmachine/standards/quetzal/index.html
// In short
// 3 bytes: return address
// 1 byte: number of locals (we don't support V4 yet)
// 1 byte: the variable to write to on return
// 1 byte: 0gfedcba, the supplied arguments
// 2 bytes: number of evaluation words in the frame
// 2 * number of locals bytes: local variables
//
// This means that we have to store the frame pointers outside the main stack
// while running and recover them on loading

auto ZorkStack::GetSuppliedArgumentsPointer() const -> uint32_t {
    return GetFramePointer() + 5u;
}

auto ZorkStack::GetEvaluationCountPointer() const -> uint32_t {
    return GetFramePointer() + 6u;
}

auto ZorkStack::GetLocalPointer(uint32_t variable) const -> uint32_t {
    return GetFramePointer() + 8u + 2u * (variable - 1u);
}

auto ZorkStack::PushStackFrame(
    uint32_t returnAddress,
    uint32_t variable,
    const std::vector<uint32_t>& arguments,
    const std::vector<uint32_t>& locals) -> void {

    PushFramePointer();
    SetFramePointer(GetStackPointer());

    Push24(returnAddress);
    Push8(locals.size());
    Push8(variable);
    Push8((1 << arguments.size()) - 1u);
    Push16(0u);
    for (auto i = 0u; i < locals.size(); ++i) {
        Push16(i < arguments.size() ? arguments[i] : locals[i]);
    }
}

auto ZorkStack::PopStackFrame() -> std::tuple<uint32_t, uint32_t, uint32_t> {
    SetStackPointer(GetSuppliedArgumentsPointer());
    auto variable = Pop8();
    auto flags = Pop8();
    auto returnAddress = Pop24();
    PopFramePointer();
    return { returnAddress, flags, variable };
}

auto ZorkStack::PushEvaluation(uint32_t value) -> void {
    auto count = ::Read16(stack, GetEvaluationCountPointer());
    Write16(stack, GetEvaluationCountPointer(), count + 1u);
    Push16(value);
}

auto ZorkStack::PopEvaluation() -> uint32_t {
    auto count = ::Read16(stack, GetEvaluationCountPointer());
    Write16(stack, GetEvaluationCountPointer(), count - 1u);
    return Pop16();
}

auto ZorkStack::ReadLocal(uint32_t variable) const -> uint32_t {
    return ::Read16(stack, GetLocalPointer(variable));
}

auto ZorkStack::WriteLocal(uint32_t variable, uint32_t value) -> void {
    ::Write16(stack, GetLocalPointer(variable), value);
}
