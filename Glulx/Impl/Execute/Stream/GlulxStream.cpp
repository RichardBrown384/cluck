#include <string>

#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxInputOutputSystem.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "Impl/Execute/ControlFlow/GlulxCallStub.h"
#include "Impl/Execute/ControlFlow/GlulxFunction.h"
#include "GlulxStream.h"

namespace {
constexpr auto HUFFMAN_NODE_INTERNAL = 0x0u;
constexpr auto HUFFMAN_NODE_TERMINATOR = 0x1u;
constexpr auto HUFFMAN_NODE_CHARACTER = 0x2u;
constexpr auto HUFFMAN_NODE_STRING_C = 0x3u;
constexpr auto HUFFMAN_NODE_CHARACTER_UNICODE = 0x4u;
constexpr auto HUFFMAN_NODE_STRING_UNICODE = 0x5u;
constexpr auto HUFFMAN_NODE_REFERENCE = 0x8u;
constexpr auto HUFFMAN_NODE_REFERENCE_INDIRECT = 0x9u;
constexpr auto HUFFMAN_NODE_REFERENCE_ARGUMENTS = 0xAu;
constexpr auto HUFFMAN_NODE_REFERENCE_INDIRECT_ARGUMENTS = 0xBu;
}

namespace fiction::glulx {
namespace {

struct BitStream {
    BitStream(GlulxImpl& glulx, uint32_t address, uint32_t bit) : glulx(glulx), address(address), bit(bit) {}

    BitStream(const BitStream&) = delete;
    BitStream(BitStream&&) = delete;
    BitStream& operator=(const BitStream&) = delete;
    BitStream& operator=(BitStream&&) = delete;
    ~BitStream() = default;

    auto Next() -> uint32_t;

    GlulxImpl& glulx;
    uint32_t address;
    uint32_t bit;
};

auto BitStream::Next() -> uint32_t {
    const auto next = (glulx.MemoryRead8(address) >> bit) & 1u;
    if (++bit == 8) {
        address += 1;
        bit = 0;
    }
    return next;
}

auto PushCompressedStringCallStub(GlulxImpl& glulx, BitStream& stream) {
    glulx.SetProgramCounter(stream.address);
    PushCallStub(glulx, DESTINATION_RESUME_PRINTING_COMPRESSED_STRING, stream.bit);
}

auto PushIntegerCallStub(GlulxImpl& glulx, uint32_t value, uint32_t position) {
    glulx.SetProgramCounter(value);
    PushCallStub(glulx, DESTINATION_RESUME_PRINTING_INTEGER, position);
}

auto PushCStringCallStub(GlulxImpl& glulx, uint32_t address) {
    glulx.SetProgramCounter(address);
    PushCallStub(glulx, DESTINATION_RESUME_PRINTING_C_STRING, 0u);
}

auto PushUnicodeCallStub(GlulxImpl& glulx, uint32_t address) {
    glulx.SetProgramCounter(address);
    PushCallStub(glulx, DESTINATION_RESUME_PRINTING_UNICODE_STRING, 0u);
}

auto ReadArguments(GlulxImpl& glulx, uint32_t address, uint32_t n) -> std::vector<uint32_t> {
    std::vector<uint32_t> arguments(n);
    for (auto i = 0u; i < n; ++i) {
        arguments[i] = glulx.MemoryRead32(address + 4u * i);
    }
    return arguments;
}

auto StreamCompressedStringReference(
    GlulxImpl& glulx,
    BitStream& stream,
    const uint32_t referenceAddress,
    const std::vector<uint32_t>& arguments) -> void {
    const auto type = glulx.MemoryRead8(referenceAddress);
    if (IsStringType(type)) {
        PushCompressedStringCallStub(glulx, stream);
        StreamStringNested(glulx, referenceAddress);
        return;
    } else if (IsFunctionType(type)) {
        PushCompressedStringCallStub(glulx, stream);
        EnterFunction(glulx, referenceAddress, arguments);
        return;
    }
    Error("Unsupported compressed string reference type");
}

}

auto StreamCharNested(GlulxImpl& glulx, uint32_t c) -> void {
    auto& ios = glulx.GetActiveInputOutputSystem();
    if (ios.IsFiltering()) {
        ios.PutChar8(glulx, c);
        return;
    }
    ios.PutChar8(glulx, c);
    PopCallStub(glulx, 0u);
}

auto StreamChar(GlulxImpl& glulx, uint32_t c) -> void {
    PushCallStub(glulx, DESTINATION_NONE, 0u);
    StreamCharNested(glulx, c);
}

void StreamUnicodeCharNested(GlulxImpl& glulx, uint32_t c) {
    auto& ios = glulx.GetActiveInputOutputSystem();
    if (ios.IsFiltering()) {
        ios.PutChar32(glulx, c);
        return;
    }
    ios.PutChar32(glulx, c);
    PopCallStub(glulx, 0u);
}

auto StreamUnicodeChar(GlulxImpl& glulx, uint32_t c) -> void {
    PushCallStub(glulx, DESTINATION_NONE, 0u);
    StreamUnicodeCharNested(glulx, c);
}

auto StreamInteger(GlulxImpl& glulx, uint32_t value, uint32_t index) -> void {
    auto& ios = glulx.GetActiveInputOutputSystem();
    auto string = std::to_string(static_cast<int32_t>(value));
    for (auto i = index; i < string.length(); ++i) {
        const auto c = string[i];
        if (ios.IsFiltering()) {
            PushIntegerCallStub(glulx, value, index + 1u);
            ios.PutChar8(glulx, c);
            return;
        }
        ios.PutChar8(glulx, c);
    }
    PopCallStub(glulx, 0u);
}

auto StreamInteger(GlulxImpl& glulx, uint32_t value) -> void {
    PushCallStub(glulx, DESTINATION_RESUME_EXECUTING, 0u);
    StreamInteger(glulx, value, 0u);
}

auto StreamCString(GlulxImpl& glulx, uint32_t address) -> void {
    auto& ios = glulx.GetActiveInputOutputSystem();
    auto c = glulx.MemoryRead8(address);
    while (c != 0u) {
        address = address + 1u;
        if (ios.IsFiltering()) {
            PushCStringCallStub(glulx, address);
            ios.PutChar8(glulx, c);
            return;
        }
        ios.PutChar8(glulx, c);
        c = glulx.MemoryRead8(address);
    }
    PopCallStub(glulx, 0u);
}

auto StreamUnicodeString(GlulxImpl& glulx, uint32_t address) -> void {
    auto& ios = glulx.GetActiveInputOutputSystem();
    auto c = glulx.MemoryRead32(address);
    while (c != 0u) {
        address = address + 4u;
        if (ios.IsFiltering()) {
            PushUnicodeCallStub(glulx, address);
            ios.PutChar32(glulx, c);
            return;
        }
        ios.PutChar32(glulx, c);
        c = glulx.MemoryRead32(address);
    }
    PopCallStub(glulx, 0u);
}



auto StreamCompressedString(GlulxImpl& glulx, uint32_t address, uint32_t bit) -> void {

    const auto stringTable = glulx.GetStringTable();
    [[maybe_unused]] const auto tableLength = glulx.MemoryRead32(stringTable);
    [[maybe_unused]] const auto nodeCount = glulx.MemoryRead32(stringTable + 4u);
    const auto rootNode = glulx.MemoryRead32(stringTable + 8u);

    BitStream stream(glulx, address, bit);

    auto nodeAddress = rootNode;
    for (;;) {
        switch (glulx.MemoryRead8(nodeAddress)) {
            case HUFFMAN_NODE_INTERNAL: {
                const auto leftNode = glulx.MemoryRead32(nodeAddress + 1u);
                const auto rightNode = glulx.MemoryRead32(nodeAddress + 5u);
                nodeAddress = (stream.Next() == 0u) ? leftNode : rightNode;
                break;
            }
            case HUFFMAN_NODE_TERMINATOR: {
                PopCallStub(glulx, 0u);
                return;
            }
            case HUFFMAN_NODE_CHARACTER: {
                const auto c = glulx.MemoryRead8(nodeAddress + 1u);
                PushCompressedStringCallStub(glulx, stream);
                StreamCharNested(glulx, c);
                return;
            }
            case HUFFMAN_NODE_STRING_C: {
                PushCompressedStringCallStub(glulx, stream);
                StreamCString(glulx, nodeAddress + 1u);
                return;
            }
            case HUFFMAN_NODE_CHARACTER_UNICODE: {
                const auto c = glulx.MemoryRead32(nodeAddress + 1u);
                PushCompressedStringCallStub(glulx, stream);
                StreamUnicodeCharNested(glulx, c);
                return;
            }
            case HUFFMAN_NODE_STRING_UNICODE: {
                PushCompressedStringCallStub(glulx, stream);
                StreamUnicodeString(glulx, nodeAddress + 1u);
                return;
            }
            case HUFFMAN_NODE_REFERENCE: {
                const auto referenceAddress = glulx.MemoryRead32(nodeAddress + 1u);
                StreamCompressedStringReference(glulx, stream, referenceAddress, {});
                return;
            }
            case HUFFMAN_NODE_REFERENCE_INDIRECT: {
                const auto indirectAddress = glulx.MemoryRead32(nodeAddress + 1u);
                const auto referenceAddress = glulx.MemoryRead32(indirectAddress);
                StreamCompressedStringReference(glulx, stream, referenceAddress, {});
                return;
            }
            case HUFFMAN_NODE_REFERENCE_ARGUMENTS: {
                const auto referenceAddress = glulx.MemoryRead32(nodeAddress + 1u);
                const auto argumentCount = glulx.MemoryRead32(nodeAddress + 5u);
                const auto arguments = ReadArguments(glulx, nodeAddress + 9u, argumentCount);
                StreamCompressedStringReference(glulx, stream, referenceAddress, arguments);
                return;
            }
            case HUFFMAN_NODE_REFERENCE_INDIRECT_ARGUMENTS: {
                const auto indirectAddress = glulx.MemoryRead32(nodeAddress + 1u);
                const auto referenceAddress = glulx.MemoryRead32(indirectAddress);
                const auto argumentCount = glulx.MemoryRead32(nodeAddress + 5u);
                const auto arguments = ReadArguments(glulx, nodeAddress + 9, argumentCount);
                StreamCompressedStringReference(glulx, stream, referenceAddress, arguments);
                return;
            }
            default:Error("Unsupported Huffman node type");
        }
    }
}

auto StreamStringNested(GlulxImpl& glulx, uint32_t address) -> void {
    const auto type = glulx.MemoryRead8(address);
    switch (type) {
        case STRING_C: return StreamCString(glulx, address + 1u);
        case STRING_COMPRESSED: return StreamCompressedString(glulx, address + 1u, 0u);
        case STRING_UNICODE: return StreamUnicodeString(glulx, address + 4u);
        default: Error("Unsupported string type");
    }
}

auto StreamString(GlulxImpl& glulx, uint32_t address) -> void {
    PushCallStub(glulx, DESTINATION_RESUME_EXECUTING, 0u);
    StreamStringNested(glulx, address);
}

}