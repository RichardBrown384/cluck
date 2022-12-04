#include <numeric>

#include "GlulxStack.h"

#include "Impl/Header/GlulxHeader.h"
#include "Util/Endian.h"

using namespace fiction::endian;

namespace fiction::glulx {
namespace {
auto CheckAlignment(uint32_t address, uint32_t size) {
    if (address & (size - 1u)) {
        std::exit(1);
    }
}
}

GlulxStack::GlulxStack(const GlulxHeader& header) :
    stack(header.stacksize),
    stackpointer(0u),
    framepointer(0u) {}

GlulxStack::~GlulxStack() = default;

auto GlulxStack::Read8(uint32_t address) const -> uint32_t { return ::Read8(stack, address); }
auto GlulxStack::Read16(uint32_t address) const -> uint32_t { return ::Read16(stack, address); }
auto GlulxStack::Read32(uint32_t address) const -> uint32_t { return ::Read32(stack, address); }

auto GlulxStack::Write8(uint32_t address, uint32_t v) -> void { ::Write8(stack, address, v); }
auto GlulxStack::Write16(uint32_t address, uint32_t v) -> void { ::Write16(stack, address, v); }
auto GlulxStack::Write32(uint32_t address, uint32_t v) -> void { ::Write32(stack, address, v); }

auto GlulxStack::GetSummary() const -> std::vector<uint8_t> {
    return { stack.begin(), stack.begin() + stackpointer };
}

auto GlulxStack::SetSummary(const std::vector<uint8_t>& summary) -> void {
    stackpointer = summary.size();
    for (auto i = 0u; i < stackpointer; ++i) {
        stack[i] = summary[i];
    }
}

auto GlulxStack::Restart() -> void {
    SetStackPointer(0u);
    SetFramePointer(0u);
}

auto GlulxStack::GetStackPointer() const -> uint32_t { return stackpointer; }
auto GlulxStack::GetFramePointer() const -> uint32_t { return framepointer; }

auto GlulxStack::GetLocalsPointer() const -> uint32_t {
    const auto framePointer = GetFramePointer();
    const auto localsPos = Read32(framePointer + 4u);
    return framePointer + localsPos;
}

auto GlulxStack::GetValuesPointer() const -> uint32_t {
    const auto framePointer = GetFramePointer();
    const auto frameLength = Read32(framePointer);
    return framePointer + frameLength;
}

auto GlulxStack::SetStackPointer(uint32_t v) -> uint32_t { stackpointer = v; return v; }
auto GlulxStack::SetFramePointer(uint32_t v) -> uint32_t { framepointer = v; return v; }

auto GlulxStack::ReadLocal8(uint32_t address) const -> uint32_t { return Read8(GetLocalsPointer() + address); }
auto GlulxStack::ReadLocal16(uint32_t address) const -> uint32_t { return Read16(GetLocalsPointer() + address); }
auto GlulxStack::ReadLocal32(uint32_t address) const -> uint32_t { return Read32(GetLocalsPointer() + address); }

auto GlulxStack::WriteLocal8(uint32_t address, uint32_t value) -> void { Write8(GetLocalsPointer() + address, value); }
auto GlulxStack::WriteLocal16(uint32_t address, uint32_t value) -> void { Write16(GetLocalsPointer() + address, value); }
auto GlulxStack::WriteLocal32(uint32_t address, uint32_t value) -> void { Write32(GetLocalsPointer() + address, value); }

auto GlulxStack::Push8(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    Write8(sp, v);
    SetStackPointer(sp + 1u);
}

auto GlulxStack::Push16(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    CheckAlignment(sp, 2u);
    Write16(sp, v);
    SetStackPointer(sp + 2u);
}

auto GlulxStack::Push32(uint32_t v) -> void {
    const auto sp = GetStackPointer();
    CheckAlignment(sp, 4u);
    Write32(sp, v);
    SetStackPointer(sp + 4u);
}

auto GlulxStack::Pop32() -> uint32_t {
    const auto sp = GetStackPointer();
    CheckAlignment(sp, 4u);
    const auto nsp = SetStackPointer(sp - 4u);
    return Read32(nsp);
}

auto GlulxStack::PopArguments(uint32_t n) -> std::vector<uint32_t> {
    std::vector<uint32_t> array(n);
    for (auto i = 0u; i < n; ++i) {
        array[i] = Pop32();
    }
    return array;
}

auto GlulxStack::GetCount() const -> uint32_t {
    const auto sp = GetStackPointer();
    const auto vp = GetValuesPointer();
    return (sp - vp) / sizeof(uint32_t);
}

auto GlulxStack::Peek(uint32_t i) const -> uint32_t {
    return Read32(GetStackPointer() - 4u * (i + 1u));
}

auto GlulxStack::Rotate(uint32_t length, uint32_t n) -> void {
    if (length == 0u) {
        return;
    }
    const auto amount = (length - n) % length;
    if (amount == 0u) {
        return;
    }
    const auto start = GetStackPointer() - 4u * length;
    const auto gcd = std::gcd(length, amount);
    for (auto i = 0u; i < gcd; ++i) {
        auto j = i;
        const auto temp = Read32(start + 4u * j);
        for (auto k = 0u; k < (length / gcd) - 1u; ++k) {
            const auto next = (j + amount) % length;
            const auto t = Read32(start + 4u * next);
            Write32(start + 4u * j, t);
            j = next;
        }
        Write32(start + 4u * j, temp);
    }
}

auto GlulxStack::Copy(uint32_t n) -> void {
    const auto start = GetStackPointer() - 4u * n;
    for (auto i = 0u; i < n; ++i) {
        Push32(Read32(start + 4u * i));
    }
}

}