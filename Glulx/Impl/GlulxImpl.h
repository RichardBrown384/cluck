#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <list>

#include "Components/Acceleration/GlulxAcceleration.h"
#include "Components/IO/GlulxInputOutputSystems.h"
#include "Components/Memory/GlulxMemory.h"
#include "Components/Random/GlulxRandom.h"
#include "Components/Stack/GlulxStack.h"
#include "Components/Undo/GlulxUndo.h"

#include "Header/GlulxHeader.h"

namespace fiction::glulx {

using GlulxServerFunction = std::function<uint32_t(uint32_t, const std::vector<uint32_t>&)>;

class GlulxInputOutputSystem;

struct GlulxExtractor {
    explicit GlulxExtractor(const std::vector<uint8_t>&);
    std::vector<uint8_t> file;
};

class GlulxImpl {
private:
    explicit GlulxImpl(const GlulxExtractor&);
public:
    explicit GlulxImpl(const std::vector<uint8_t>&);
    GlulxImpl(const GlulxImpl&) = delete;
    GlulxImpl(GlulxImpl&&) = delete;
    GlulxImpl& operator=(const GlulxImpl&) = delete;
    GlulxImpl& operator=(GlulxImpl&&) = delete;
    ~GlulxImpl();

    [[nodiscard]] auto IsFileValid() const -> bool;

    auto SetInputOutputSystem(uint32_t id, GlulxInputOutputSystem&) -> void;
    auto SetServerFunction(GlulxServerFunction) -> void;

    auto Restart() -> void;
    auto Execute() -> void;

    // Restart
    [[nodiscard]] auto GetRestartStringTable() const -> uint32_t;
    [[nodiscard]] auto GetRestartFunction() const -> uint32_t;

    // Running
    [[nodiscard]] auto GetRunning() const -> bool;
    auto SetRunning(bool) -> void;

    // Program counter
    [[nodiscard]] auto GetProgramCounter() const -> uint32_t;
    auto SetProgramCounter(uint32_t) -> uint32_t;

    // String table
    [[nodiscard]] auto GetStringTable() const -> uint32_t;
    auto SetStringTable(uint32_t) -> uint32_t;

    // Stack
    [[nodiscard]] auto GetStackPointer() const -> uint32_t;
    [[nodiscard]] auto GetFramePointer() const -> uint32_t;

    auto SetStackPointer(uint32_t) -> uint32_t;
    auto SetFramePointer(uint32_t) -> uint32_t;

    [[nodiscard]] auto StackReadLocal8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto StackReadLocal16(uint32_t) const -> uint32_t;
    [[nodiscard]] auto StackReadLocal32(uint32_t) const -> uint32_t;

    auto StackWriteLocal8(uint32_t, uint32_t) -> void;
    auto StackWriteLocal16(uint32_t, uint32_t) -> void;
    auto StackWriteLocal32(uint32_t, uint32_t) -> void;

    auto StackPush8(uint32_t) -> void;
    auto StackPush16(uint32_t) -> void;
    auto StackPush32(uint32_t) -> void;

    auto StackPop32() -> uint32_t;

    auto StackPopArguments(uint32_t) -> std::vector<uint32_t>;

    [[nodiscard]] auto StackGetCount() const -> uint32_t;
    [[nodiscard]] auto StackPeek(uint32_t) const -> uint32_t;
    auto StackRotate(uint32_t, uint32_t) -> void;
    auto StackCopy(uint32_t) -> void;

    // Memory
    [[nodiscard]] auto GetMemSize() const -> uint32_t;
    [[nodiscard]] auto GetRamStart() const -> uint32_t;
    [[nodiscard]] auto GetHeapStart() const -> uint32_t;

    auto SetMemSize(uint32_t size) -> bool;
    auto SetProtect(uint32_t, uint32_t length) -> void;

    auto Malloc(uint32_t) -> uint32_t;
    auto Free(uint32_t) -> void;

    [[nodiscard]] auto MemoryRead8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto MemoryRead16(uint32_t) const -> uint32_t;
    [[nodiscard]] auto MemoryRead32(uint32_t) const -> uint32_t;

    auto MemoryWrite8(uint32_t, uint32_t) -> void;
    auto MemoryWrite16(uint32_t, uint32_t) -> void;
    auto MemoryWrite32(uint32_t, uint32_t) -> void;

    // Input Output
    [[nodiscard]] auto GetInputOutputSystem() const -> uint32_t;
    [[nodiscard]] auto GetInputOutputRock() const -> uint32_t;
    auto IsInputOutputSystemSupported(uint32_t) -> bool;
    auto GetActiveInputOutputSystem() -> GlulxInputOutputSystem&;

    auto SetInputOutputSystemAndRock(uint32_t, uint32_t) -> void;

    // Random
    auto RandomSeed(uint32_t) -> void;
    [[nodiscard]] auto RandomNext(int32_t) const -> int32_t;

    // Acceleration
    [[nodiscard]] auto GetAccelerationParam(uint32_t) const -> uint32_t;
    auto SetAccelerationParam(uint32_t, uint32_t) -> void;
    [[nodiscard]] auto IsAccelerationFunctionSupported(uint32_t) const -> bool;
    auto SetAccelerationFunction(uint32_t, uint32_t) -> void;
    auto CallAccelerationFunction(uint32_t, const std::vector<uint32_t>&, uint32_t*) -> bool;

    // Server (Glk)
    auto CallServer(uint32_t, const std::vector<uint32_t>&) -> uint32_t;

    // Game State
    auto GameStateSave(uint32_t) -> bool;
    auto GameStateSaveUndo() -> bool;

    auto GameStateRestore(uint32_t) -> bool;
    auto GameStateRestoreUndo() -> bool;

    [[nodiscard]] auto GameStateHasUndo() const -> bool;
    auto GameStateDiscardUndo() -> void;

private:
    GlulxHeader header;
    bool running;
    uint32_t programCounter;
    uint32_t stringTable;
    GlulxMemory memory;
    GlulxStack stack;
    GlulxInputOutputSystems iosystems;
    GlulxServerFunction serverFunction;
    GlulxRandom random;
    GlulxAcceleration acceleration;
    GlulxUndo undo;
};

}