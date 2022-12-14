#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <string>

#include "Impl/Header/ZorkHeader.h"
#include "Impl/Components/Dictionary/ZorkDictionary.h"
#include "Impl/Components/Memory/ZorkMemory.h"
#include "Impl/Components/Objects/ZorkObjects.h"
#include "Impl/Components/Random/ZorkRandom.h"
#include "Impl/Components/Stack/ZorkStack.h"
#include "Impl/Components/Tokenizer/ZorkTokenizer.h"

namespace fiction::zork {

class ZorkImpl {
public:
    explicit ZorkImpl(const std::vector<uint8_t>& file);
    ZorkImpl(const ZorkImpl&) = delete;
    ZorkImpl(ZorkImpl&&) = delete;
    ZorkImpl& operator=(const ZorkImpl&) = delete;
    ZorkImpl& operator=(ZorkImpl&&) = delete;
    ~ZorkImpl();

    [[nodiscard]] auto IsChecksumValid() const -> bool;

    auto Restart() -> void;
    auto Execute() -> void;

    // Running
    [[nodiscard]] auto GetRunning() const -> bool;
    auto SetRunning(bool) -> void;

    // Program counter
    [[nodiscard]] auto GetProgramCounter() const -> uint32_t;
    auto SetProgramCounter(uint32_t) -> uint32_t;

    // Addresses
    [[nodiscard]] auto GetUnpackedAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetGlobalVariableAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetAbbreviationAddress(uint32_t) const -> uint32_t;

    // Memory
    [[nodiscard]] auto MemoryRead8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto MemoryRead16(uint32_t) const -> uint32_t;

    auto MemoryWrite8(uint32_t, uint32_t) -> void;
    auto MemoryWrite16(uint32_t, uint32_t) -> void;

    // Stack
    auto PushStackFrame(uint32_t, uint32_t, const std::vector<uint32_t>&, const std::vector<uint32_t>&) -> void;
    auto PopStackFrame() -> std::tuple<uint32_t, uint32_t, uint32_t>;

    // Variables
    auto ReadVariable(uint32_t) -> uint32_t;
    auto WriteVariable(uint32_t, uint32_t) -> void;

    // Instruction stream
    auto ReadInstruction8() -> uint32_t;
    auto ReadInstruction16() -> uint32_t;
    auto ReadInstructionLargeConstant() -> uint32_t;
    auto ReadInstructionSmallConstant() -> uint32_t;
    auto ReadInstructionVariable() -> uint32_t;
    auto ReadInstructionOperands(uint32_t) -> std::vector<uint32_t>;
    auto ReadInstructionBranch() -> std::tuple<bool, uint32_t>;

    // Control flow
    auto DoCall(uint32_t, uint32_t, const std::vector<uint32_t>&) -> void;
    auto DoReturn(uint32_t) -> void;
    auto DoBranch(const std::tuple<bool, uint32_t>&, bool) -> void;

    // Random numbers
    auto DoRandom(uint32_t) -> uint32_t;

    // Objects
    [[nodiscard]] auto GetObjectNameAddress(uint32_t) const -> uint32_t;

    [[nodiscard]] auto ReadObjectAttribute(uint32_t, uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectParent(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectSibling(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectChild(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectPropertyNumberAndSize(uint32_t) const -> std::tuple<uint32_t, uint32_t>;
    [[nodiscard]] auto ReadObjectProperty(uint32_t, uint32_t) const -> uint32_t;

    [[nodiscard]] auto FindObjectPropertySizeAndAddress(uint32_t, uint32_t) const -> std::tuple<uint32_t, uint32_t>;
    [[nodiscard]] auto FindObjectNextPropertyNumber(uint32_t, uint32_t) const -> uint32_t;

    auto WriteObjectAttribute(uint32_t, uint32_t, uint32_t) -> void;
    auto WriteObjectProperty(uint32_t, uint32_t, uint32_t) -> void;

    auto RemoveObject(uint32_t) -> void;
    auto InsertObject(uint32_t, uint32_t) -> void;

    // String printing
    auto PrintCharacter(uint32_t) -> void;
    auto PrintNumber(uint32_t) -> void;
    auto PrintString(uint32_t, bool = false) -> uint32_t;

    // String reading
    auto ReadLine() -> std::string;

    // String tokenization
    [[nodiscard]] auto TokenizeString(const std::string&) const -> std::vector<ZorkToken>;

    // Text buffer
    auto WriteToTextBuffer(uint32_t, const std::string& text) -> void;

    // Parse buffer
    auto WriteToParseBuffer(uint32_t, const std::vector<ZorkToken>&) -> void;

private:
    bool running;
    uint32_t programCounter;
    ZorkHeader header;
    ZorkMemory memory;
    ZorkStack stack;
    ZorkRandom random;
    ZorkDictionary dictionary;
    ZorkTokenizer tokenizer;
    ZorkObjects objects;
};
}