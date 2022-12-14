#include "ZorkImpl.h"
#include "ZorkError.h"

#include "Impl/Execute/ZorkExecute.h"
#include "Util/Integer.h"

using namespace fiction::zork;

ZorkImpl::ZorkImpl(const std::vector<uint8_t>& file) :
    running(false),
    programCounter(0u),
    header(ReadHeader(file)),
    memory(file, header),
    stack(),
    random(),
    dictionary(memory, header),
    tokenizer(dictionary, header),
    objects(memory, header) {}

ZorkImpl::~ZorkImpl() = default;

auto ZorkImpl::IsChecksumValid() const -> bool {
    return header.headerChecksum == header.checksum;
}

auto ZorkImpl::Restart() -> void {
    SetRunning(true);
    memory.Restart();
    stack.Restart();
    SetProgramCounter(header.programCounter);
    PushStackFrame(0u, 0u, {}, {});
}

auto ZorkImpl::Execute() -> void { if (GetRunning()) { ::Execute(*this); } }

// Running
auto ZorkImpl::GetRunning() const -> bool { return running; }
auto ZorkImpl::SetRunning(bool v) -> void { running = v; }

// Program counter
auto ZorkImpl::GetProgramCounter() const -> uint32_t { return programCounter; }
auto ZorkImpl::SetProgramCounter(uint32_t v) -> uint32_t {
    if (v == 0u) {
        Error("Attempted to write zero to the program counter");
    }
    programCounter = v;
    return v;
}

// Addresses
auto ZorkImpl::GetUnpackedAddress(uint32_t address) const -> uint32_t { return 2u * address; }

auto ZorkImpl::GetGlobalVariableAddress(uint32_t variable) const -> uint32_t {
    return header.globalVariables + 2u * (variable - 0x10u);
}

auto ZorkImpl::GetAbbreviationAddress(uint32_t abbreviation) const -> uint32_t {
    return 2u * MemoryRead16(header.abbreviationTable + 2u * abbreviation);
}

// Memory
auto ZorkImpl::MemoryRead8(uint32_t address) const -> uint32_t { return memory.Read8(address); }
auto ZorkImpl::MemoryRead16(uint32_t address) const -> uint32_t { return memory.Read16(address); }

auto ZorkImpl::MemoryWrite8(uint32_t address, uint32_t v) -> void { memory.Write8(address, v); }
auto ZorkImpl::MemoryWrite16(uint32_t address, uint32_t v) -> void { memory.Write16(address, v); }

// Stack
auto ZorkImpl::PushStackFrame(
    uint32_t returnAddress,
    uint32_t variable,
    const std::vector<uint32_t>& arguments,
    const std::vector<uint32_t>& locals) -> void {
    stack.PushStackFrame(returnAddress, variable, arguments, locals);
}

auto ZorkImpl::PopStackFrame() -> std::tuple<uint32_t, uint32_t, uint32_t> {
    return stack.PopStackFrame();
}

// Variables
auto ZorkImpl::ReadVariable(uint32_t variable) -> uint32_t {
    const auto clampedVariable = variable & 0xFFu;
    if (clampedVariable == 0u) {
        return stack.PopEvaluation();
    }
    if (clampedVariable < 0x10u) {
        return stack.ReadLocal(clampedVariable);
    }
    return MemoryRead16(GetGlobalVariableAddress(clampedVariable));
}

auto ZorkImpl::WriteVariable(uint32_t variable, uint32_t value) -> void {
    const auto clampedVariable = variable & 0xFF;
    const auto clampedValue = value & 0xFFFF;
    if (clampedVariable == 0u) {
        stack.PushEvaluation(clampedValue);
    } else if (clampedVariable < 0x10u) {
        stack.WriteLocal(clampedVariable, clampedValue);
    } else {
        MemoryWrite16(GetGlobalVariableAddress(clampedVariable), clampedValue);
    }
}

// Random numbers
auto ZorkImpl::DoRandom(uint32_t range) -> uint32_t {
    return random.GetNext(Signed16Cast(range));
}

// Objects
auto ZorkImpl::GetObjectNameAddress(uint32_t object) const -> uint32_t {
    return objects.GetObjectNameAddress(object);
}

auto ZorkImpl::ReadObjectAttribute(uint32_t object, uint32_t attribute) const -> uint32_t {
    return objects.ReadObjectAttribute(object, attribute);
}

auto ZorkImpl::ReadObjectParent(uint32_t object) const -> uint32_t {
    return objects.ReadObjectParent(object);
}

auto ZorkImpl::ReadObjectSibling(uint32_t object) const -> uint32_t {
    return objects.ReadObjectSibling(object);
}

auto ZorkImpl::ReadObjectChild(uint32_t object) const -> uint32_t {
    return objects.ReadObjectChild(object);
}

auto ZorkImpl::ReadObjectPropertyNumberAndSize(uint32_t address) const -> std::tuple<uint32_t, uint32_t> {
    return objects.ReadObjectPropertyNumberAndSize(address);
}

auto ZorkImpl::ReadObjectProperty(uint32_t object, uint32_t property) const -> uint32_t {
    return objects.ReadObjectProperty(object, property);
}

auto ZorkImpl::FindObjectPropertySizeAndAddress(uint32_t object, uint32_t property) const -> std::tuple<uint32_t, uint32_t> {
    return objects.FindObjectPropertySizeAndAddress(object, property);
}

auto ZorkImpl::FindObjectNextPropertyNumber(uint32_t object, uint32_t property) const -> uint32_t {
    return objects.FindObjectNextPropertyNumber(object, property);
}

auto ZorkImpl::WriteObjectAttribute(uint32_t object, uint32_t attribute, uint32_t value) -> void {
    objects.WriteObjectAttribute(object, attribute, value);
}

auto ZorkImpl::WriteObjectProperty(uint32_t object, uint32_t property, uint32_t value) -> void {
    objects.WriteObjectProperty(object, property, value);
}

auto ZorkImpl::RemoveObject(uint32_t object) -> void {
    objects.RemoveObject(object);
}

auto ZorkImpl::InsertObject(uint32_t object, uint32_t destination) -> void {
    objects.InsertObject(object, destination);
}

// String tokenization
auto ZorkImpl::TokenizeString(const std::string& string) const -> std::vector<ZorkToken> {
    return tokenizer.TokenizeString(string);
}