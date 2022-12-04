#include "GlulxImpl.h"

#include "GlulxInputOutputSystem.h"

#include "Impl/Execute/ControlFlow/GlulxFunction.h"
#include "Impl/Execute/GlulxExecute.h"

#include "Impl/Serial/GlulxSerial.h"

namespace fiction::glulx {

GlulxImpl::GlulxImpl(std::vector<uint8_t> file) :
    header(ReadHeader(file)),
    running(IsHeaderValid(header)),
    programCounter(0u),
    stringTable(0u),
    memory(std::move(file), header),
    stack(header),
    iosystems(),
    serverFunction([](uint32_t,const std::vector<uint32_t>&) { return 0u; }),
    random(),
    acceleration(),
    undo() {}

GlulxImpl::~GlulxImpl() = default;

auto GlulxImpl::IsFileValid() const -> bool { return IsHeaderValid(header); }

auto GlulxImpl::SetInputOutputSystem(uint32_t id, GlulxInputOutputSystem& system) -> void {
    iosystems.SetInputOutputSystem(id, system);
}

auto GlulxImpl::SetServerFunction(GlulxServerFunction function) -> void { serverFunction.swap(function); }

auto GlulxImpl::Restart() -> void {
    SetRunning(IsFileValid());
    SetProgramCounter(0u);
    SetStringTable(GetRestartStringTable());
    stack.Restart();
    memory.Restart();
    SetInputOutputSystemAndRock(IO_SYSTEM_NULL, 0u);
    RandomSeed(0u);
    EnterFunction(*this, GetRestartFunction(), {});
}

auto GlulxImpl::Execute() -> void {
    if (GetRunning()) {
        fiction::glulx::Execute(*this);
    }
}

// Restart

auto GlulxImpl::GetRestartStringTable() const -> uint32_t { return header.decodingtable; }
auto GlulxImpl::GetRestartFunction() const -> uint32_t { return header.startfunc; }

// Running

auto GlulxImpl::GetRunning() const -> bool { return running; }
auto GlulxImpl::SetRunning(bool v) -> void { running = v; }

// Program Counter

auto GlulxImpl::GetProgramCounter() const -> uint32_t { return programCounter; }
auto GlulxImpl::SetProgramCounter(uint32_t v) -> uint32_t { programCounter = v; return v; }

// String table

auto GlulxImpl::GetStringTable() const -> uint32_t { return stringTable; }
auto GlulxImpl::SetStringTable(uint32_t v) -> uint32_t { stringTable = v; return v; }

// Stack

auto GlulxImpl::GetStackPointer() const -> uint32_t { return stack.GetStackPointer(); }
auto GlulxImpl::GetFramePointer() const -> uint32_t { return stack.GetFramePointer(); }

auto GlulxImpl::SetStackPointer(uint32_t v) -> uint32_t { return stack.SetStackPointer(v); }
auto GlulxImpl::SetFramePointer(uint32_t v) -> uint32_t { return stack.SetFramePointer(v); }

auto GlulxImpl::StackReadLocal8(uint32_t address) const -> uint32_t { return stack.ReadLocal8(address); }
auto GlulxImpl::StackReadLocal16(uint32_t address) const -> uint32_t { return stack.ReadLocal16(address); }
auto GlulxImpl::StackReadLocal32(uint32_t address) const -> uint32_t { return stack.ReadLocal32(address); }

auto GlulxImpl::StackWriteLocal8(uint32_t address, uint32_t v) -> void { stack.WriteLocal8(address, v); }
auto GlulxImpl::StackWriteLocal16(uint32_t address, uint32_t v) -> void { stack.WriteLocal16(address, v); }
auto GlulxImpl::StackWriteLocal32(uint32_t address, uint32_t v) -> void { stack.WriteLocal32(address, v); }

auto GlulxImpl::StackPush8(uint32_t v) -> void { stack.Push8(v); }
auto GlulxImpl::StackPush16(uint32_t v) -> void { stack.Push16(v); }
auto GlulxImpl::StackPush32(uint32_t v) -> void { stack.Push32(v); }

auto GlulxImpl::StackPop32() -> uint32_t { return stack.Pop32(); }

auto GlulxImpl::StackPopArguments(uint32_t n) -> std::vector<uint32_t> {
    return stack.PopArguments(n);
}

auto GlulxImpl::StackGetCount() const -> uint32_t { return stack.GetCount(); }
auto GlulxImpl::StackPeek(uint32_t n) const -> uint32_t { return stack.Peek(n); }
auto GlulxImpl::StackRotate(uint32_t length, uint32_t n) -> void { stack.Rotate(length, n); }
auto GlulxImpl::StackCopy(uint32_t n) -> void { stack.Copy(n); }

// Memory

auto GlulxImpl::GetMemSize() const -> uint32_t { return memory.GetSize(); }
auto GlulxImpl::GetRamStart() const -> uint32_t { return memory.GetRamStart(); }
auto GlulxImpl::GetHeapStart() const -> uint32_t { return memory.GetHeapStart(); }

auto GlulxImpl::SetMemSize(uint32_t size) -> bool { return memory.SetSize(size); }
auto GlulxImpl::SetProtect(uint32_t address, uint32_t length) -> void { memory.SetProtect(address, length); }

auto GlulxImpl::Malloc(uint32_t length) -> uint32_t { return memory.Malloc(length); }
auto GlulxImpl::Free(uint32_t address) -> void { memory.Free(address); }

auto GlulxImpl::MemoryRead8(uint32_t address) const -> uint32_t { return memory.Read8(address); }
auto GlulxImpl::MemoryRead16(uint32_t address) const -> uint32_t { return memory.Read16(address); }
auto GlulxImpl::MemoryRead32(uint32_t address) const -> uint32_t { return memory.Read32(address); }

auto GlulxImpl::MemoryWrite8(uint32_t address, uint32_t v) -> void { memory.Write8(address, v); }
auto GlulxImpl::MemoryWrite16(uint32_t address, uint32_t v) -> void { memory.Write16(address, v); }
auto GlulxImpl::MemoryWrite32(uint32_t address, uint32_t v) -> void { memory.Write32(address, v); }

// Input and Output

auto GlulxImpl::GetInputOutputSystem() const -> uint32_t { return iosystems.GetInputOutputSystem(); }
auto GlulxImpl::GetInputOutputRock() const -> uint32_t { return iosystems.GetInputOutputRock(); }

auto GlulxImpl::IsInputOutputSystemSupported(uint32_t v) -> bool {
    return iosystems.IsInputOutputSystemSupported(v);
}

auto GlulxImpl::GetActiveInputOutputSystem() -> GlulxInputOutputSystem& {
    return iosystems.GetActiveInputOutputSystem();
}

auto GlulxImpl::SetInputOutputSystemAndRock(uint32_t system, uint32_t rock) -> void {
    iosystems.SetInputOutputSystemAndRock(system, rock);
}

// Random

auto GlulxImpl::RandomSeed(uint32_t seed) -> void { random.SetSeed(seed); }
auto GlulxImpl::RandomNext(int32_t modulus) const -> int32_t { return random.GetNext(modulus); }

// Acceleration

auto GlulxImpl::GetAccelerationParam(uint32_t param) const -> uint32_t {
    return acceleration.GetParam(param);
}

auto GlulxImpl::SetAccelerationParam(uint32_t param, uint32_t value) -> void {
    acceleration.SetParam(param, value);
}

auto GlulxImpl::IsAccelerationFunctionSupported(uint32_t function) const -> bool {
    return acceleration.IsFunctionSupported(function);
}

auto GlulxImpl::SetAccelerationFunction(uint32_t function, uint32_t address) -> void {
    acceleration.SetFunction(*this, function, address);
}

auto GlulxImpl::CallAccelerationFunction(uint32_t address, const std::vector<uint32_t>& arguments, uint32_t* result) -> bool {
    return acceleration.CallFunction(*this, address, arguments, result);
}

// Server (Glk)

auto GlulxImpl::CallServer(uint32_t function, const std::vector<uint32_t>& arguments) -> uint32_t {
    return serverFunction(function, arguments);
}

// Game glulx
auto GlulxImpl::GameStateSave(uint32_t streamId) -> bool {
    auto buffer = WriteIff(memory, stack);
    auto& ios = GetActiveInputOutputSystem();
    ios.PutBuffer8(*this, streamId, buffer);
    return true;
}

auto GlulxImpl::GameStateRestore(uint32_t streamId) -> bool {
    auto& ios = GetActiveInputOutputSystem();
    const auto& buffer = ios.GetBuffer8(*this, streamId);
    return ReadIff(buffer, memory, stack);
}

auto GlulxImpl::GameStateSaveUndo() -> bool {
    auto buffer = WriteIff(memory, stack);
    undo.Push(std::move(buffer));
    return true;
}

auto GlulxImpl::GameStateRestoreUndo() -> bool {
    if (undo.Empty()) {
        return false;
    }
    const auto& buffer = undo.Peek();
    auto result = ReadIff(buffer, memory, stack);
    if (result) {
        undo.Pop();
    }
    return result;
}

auto GlulxImpl::GameStateHasUndo() const -> bool {
    return !undo.Empty();
}

auto GlulxImpl::GameStateDiscardUndo() -> void {
    if (!undo.Empty()) {
        undo.Pop();
    }
}

}