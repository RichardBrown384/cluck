#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxCallStub.h"
#include "GlulxFunction.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"

#include "Common/MathsUtil.h"

using namespace fiction::glulx;

namespace fiction::glulx {
namespace {

struct GlulxLocalFormat {
    GlulxLocalFormat(uint32_t s, uint32_t c) : size(s), count(c) {}
    uint32_t size;
    uint32_t count;
};

constexpr auto IsOdd(uint32_t v) { return (v & 1u) != 0u; }

auto ReadFunctionType(GlulxImpl& glulx) { return OpcodeStreamRead8(glulx); }

auto ReadFunctionLocalFormats(GlulxImpl& glulx) {
    std::vector<GlulxLocalFormat> locals;
    for (;;) {
        const auto size = OpcodeStreamRead8(glulx);
        const auto count = OpcodeStreamRead8(glulx);
        locals.emplace_back(size, count);
        if (size == 0) {
            break;
        }
    }
    if (IsOdd(locals.size())) {
        locals.emplace_back(0u, 0u);
    }
    return locals;
}

auto ComputeLocalsSize(const std::vector<GlulxLocalFormat>& formats) {
    auto localsSize = 0u;
    for (const auto& [size, count]: formats) {
        if (size != 0u) {
            localsSize += ComputeAlignmentPadding(localsSize, size);
            localsSize += count * size;
        }
    }
    localsSize += ComputeAlignmentPadding(localsSize, 4u);
    return localsSize;
}

auto PushLocalFormats(GlulxImpl& glulx, const std::vector<GlulxLocalFormat>& formats) {
    for (const auto& format: formats) {
        glulx.StackPush8(format.size);
        glulx.StackPush8(format.count);
    }
}

auto PushLocalZeroes(GlulxImpl& glulx, uint32_t count) {
    for (auto i = 0u; i < count; ++i) {
        glulx.StackPush8(0u);
    }
}

auto PushStackArguments(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) {
    for (auto i = 0u; i < arguments.size(); ++i) {
        glulx.StackPush32(arguments[arguments.size() - 1 - i]);
    }
    glulx.StackPush32(arguments.size());
}

auto PushLocalArgument(GlulxImpl& glulx, const GlulxLocalFormat& format, uint32_t argument) {
    const auto sp = glulx.GetStackPointer();
    const auto padding = ComputeAlignmentPadding(sp, format.size);
    glulx.SetStackPointer(sp + padding);
    switch (format.size) {
        case 0: return;
        case 1: return glulx.StackPush8(argument);
        case 2: return glulx.StackPush16(argument);
        case 4: return glulx.StackPush32(argument);
        default: Error("Unsupported stack local argument size");
    }
}

auto PushLocalArguments(
    GlulxImpl& glulx,
    const std::vector<GlulxLocalFormat>& formats,
    const std::vector<uint32_t>& arguments) {
    auto k = 0ul;
    for (const auto& format: formats) {
        if (format.size != 0u) {
            const auto limit = std::min(k + format.count, arguments.size());
            for (; k < limit; ++k) {
                PushLocalArgument(glulx, format, arguments[k]);
            }
        }
    }
}

}

auto EnterFunction(GlulxImpl& glulx, const uint32_t functionAddress, const std::vector<uint32_t>& arguments) -> void {

    auto accelerationResult { 0u };
    if (glulx.CallAccelerationFunction(functionAddress, arguments, &accelerationResult)) {
        PopCallStub(glulx, accelerationResult);
        return;
    }

    glulx.SetProgramCounter(functionAddress);

    const auto type = ReadFunctionType(glulx);
    if (!IsFunctionType(type)) {
        Error("Unsupported function type");
    }
    const auto formats = ReadFunctionLocalFormats(glulx);

    const auto localsPos = 8u + 2 * formats.size();
    const auto localsSize = ComputeLocalsSize(formats);
    const auto frameLength = localsPos + localsSize;

    glulx.SetFramePointer(glulx.GetStackPointer());
    glulx.StackPush32(frameLength);
    glulx.StackPush32(localsPos);
    PushLocalFormats(glulx, formats);
    PushLocalZeroes(glulx, localsSize);
    if (type == FUNCTION_STACK_ARGUMENT) {
        PushStackArguments(glulx, arguments);
    } else {
        const auto sp = glulx.GetStackPointer();
        glulx.SetStackPointer(sp - localsSize);
        PushLocalArguments(glulx, formats, arguments);
        glulx.SetStackPointer(sp);
    }
}

auto LeaveFunction(GlulxImpl& glulx) -> void {
    const auto framePointer = glulx.GetFramePointer();
    glulx.SetStackPointer(framePointer);
}

}