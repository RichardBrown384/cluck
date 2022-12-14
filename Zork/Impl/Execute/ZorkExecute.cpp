#include "Impl/ZorkImpl.h"
#include "Impl/ZorkError.h"

#include "Common/MathsUtil.h"
#include "Common/TakeFirst.h"

#include "Util/Integer.h"

using namespace fiction::zork;

namespace {

constexpr auto Z_MACHINE_FALSE = 0u;
constexpr auto Z_MACHINE_TRUE = 1u;

constexpr auto VARIABLE_STACK = 0u;

auto ReadOperandsByType(ZorkImpl& impl) {
    const auto types = impl.ReadInstruction8();
    return impl.ReadInstructionOperands(types);
}

template<uint32_t n>
auto ReadOperandsByTypeN(ZorkImpl& impl) {
    const auto& operands = ReadOperandsByType(impl);
    return TakeFirst<n>(operands);
}

auto ReadOperands1(ZorkImpl& impl) { return ReadOperandsByTypeN<1>(impl); }
auto ReadOperands2(ZorkImpl& impl) { return ReadOperandsByTypeN<2>(impl); }
auto ReadOperands3(ZorkImpl& impl) { return ReadOperandsByTypeN<3>(impl); }

auto ReadSmallSmall(ZorkImpl& impl) -> std::tuple<uint32_t, uint32_t> {
    const auto first = impl.ReadInstructionSmallConstant();
    const auto second = impl.ReadInstructionSmallConstant();
    return { first, second };
}

auto ReadSmallVariable(ZorkImpl& impl) -> std::tuple<uint32_t, uint32_t> {
    const auto first = impl.ReadInstructionSmallConstant();
    const auto second = impl.ReadInstructionVariable();
    return { first, second };
}

auto ReadVariableSmall(ZorkImpl& impl) -> std::tuple<uint32_t, uint32_t> {
    const auto first = impl.ReadInstructionVariable();
    const auto second = impl.ReadInstructionSmallConstant();
    return { first, second };
}

auto ReadVariableVariable(ZorkImpl& impl) -> std::tuple<uint32_t, uint32_t> {
    const auto first = impl.ReadInstructionVariable();
    const auto second = impl.ReadInstructionVariable();
    return { first, second };
}

auto ReadLarge(ZorkImpl& impl) -> std::tuple<uint32_t> { return { impl.ReadInstructionLargeConstant() }; }
auto ReadSmall(ZorkImpl& impl) -> std::tuple<uint32_t> { return { impl.ReadInstructionSmallConstant() }; }
auto ReadVariable(ZorkImpl& impl) -> std::tuple<uint32_t> { return { impl.ReadInstructionVariable() }; }

auto Illegal(ZorkImpl&) -> void {
    Error("Illegal");
}

auto Unimplemented(const char* feature) {
    ErrorFormat("Unimplemented feature: %s", feature);
}

// 2OPS
template<auto ReadOperands>
auto Je(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, op1 == op2);
}

auto JeVariable(ZorkImpl& impl) -> void {
    const auto operands = ReadOperandsByType(impl);
    const auto& branch = impl.ReadInstructionBranch();
    const auto result = std::find(operands.cbegin() + 1u, operands.cend(), operands[0]);
    impl.DoBranch(branch, result != std::cend(operands));
}

template<auto ReadOperands>
auto Jl(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, LessThan(op1, op2));
}

template<auto ReadOperands>
auto Jg(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, GreaterThan(op1, op2));
}

template<auto ReadOperands>
auto DecChk(ZorkImpl& impl) -> void {
    const auto&[variable, b] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    const auto a = (impl.ReadVariable(variable) - 1u) & 0xFFFFu;
    impl.WriteVariable(variable, a);
    impl.DoBranch(branch, LessThan(a, b));
}

template<auto ReadOperands>
auto IncChk(ZorkImpl& impl) -> void {
    const auto&[variable, b] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    const auto a = (impl.ReadVariable(variable) + 1u) & 0xFFFFu;
    impl.WriteVariable(variable, a);
    impl.DoBranch(branch, GreaterThan(a, b));
}

template<auto ReadOperands>
auto Jin(ZorkImpl& impl) -> void {
    const auto&[object, parent] = ReadOperands(impl);
    if (object == 0u) {
        return;
    }
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, impl.ReadObjectParent(object) == parent);
}

template<auto ReadOperands>
auto Test(ZorkImpl& impl) -> void {
    const auto&[bitmap, flags] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, (bitmap & flags) == flags);
}

template<auto ReadOperands>
auto Or(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, op1 | op2);
}

template<auto ReadOperands>
auto And(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, op1 & op2);
}

template<auto ReadOperands>
auto TestAttr(ZorkImpl& impl) -> void {
    const auto&[object, attribute] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    const auto value = impl.ReadObjectAttribute(object, attribute);
    impl.DoBranch(branch, value == Z_MACHINE_TRUE);
}

template<auto ReadOperands>
auto SetAttr(ZorkImpl& impl) -> void {
    const auto&[object, attribute] = ReadOperands(impl);
    impl.WriteObjectAttribute(object, attribute, Z_MACHINE_TRUE);
}

template<auto ReadOperands>
auto ClearAttr(ZorkImpl& impl) -> void {
    const auto&[object, attribute] = ReadOperands(impl);
    impl.WriteObjectAttribute(object, attribute, Z_MACHINE_FALSE);
}

template<auto ReadOperands>
auto Store(ZorkImpl& impl) -> void {
    const auto&[variable, value] = ReadOperands(impl);
    impl.WriteVariable(variable, value);
}

template<auto ReadOperands>
auto InsertObj(ZorkImpl& impl) -> void {
    const auto&[object, destination] = ReadOperands(impl);
    impl.InsertObject(object, destination);
}

template<auto ReadOperands>
auto LoadW(ZorkImpl& impl) -> void {
    const auto&[arrayAddress, element] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto value = impl.MemoryRead16(arrayAddress + 2u * element);
    impl.WriteVariable(variable, value);
}

template<auto ReadOperands>
auto LoadB(ZorkImpl& impl) -> void {
    const auto&[arrayAddress, element] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto value = impl.MemoryRead8(arrayAddress + element);
    impl.WriteVariable(variable, value);
}

template<auto ReadOperands>
auto GetProp(ZorkImpl& impl) -> void {
    const auto&[object, property] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto prop = impl.ReadObjectProperty(object, property);
    impl.WriteVariable(variable, prop);
}

template<auto ReadOperands>
auto GetPropAddr(ZorkImpl& impl) -> void {
    const auto&[object, property] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto&[size, address] = impl.FindObjectPropertySizeAndAddress(object, property);
    impl.WriteVariable(variable, address);
}

template<auto ReadOperands>
auto GetNextProp(ZorkImpl& impl) -> void {
    const auto&[object, property] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto nextProperty = impl.FindObjectNextPropertyNumber(object, property);
    impl.WriteVariable(variable, nextProperty);
}

template<auto ReadOperands>
auto Add(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, op1 + op2);
}

template<auto ReadOperands>
auto Sub(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, op1 - op2);
}

template<auto ReadOperands>
auto Mul(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, SignExtend16(op1) * SignExtend16(op2));
}

template<auto ReadOperands>
auto Div(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    if (op2 == 0u) {
        Error("Divide by zero");
        return;
    }
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, SignExtend16(op1) / SignExtend16(op2));
}

template<auto ReadOperands>
auto Mod(ZorkImpl& impl) -> void {
    const auto&[op1, op2] = ReadOperands(impl);
    if (op2 == 0u) {
        Error("Divide by zero");
        return;
    }
    const auto variable = impl.ReadInstruction8();
    impl.WriteVariable(variable, SignExtend16(op1) % SignExtend16(op2));
}

// 1OPS
template<auto ReadOperands>
auto Jz(ZorkImpl& impl) -> void {
    const auto& [op1] = ReadOperands(impl);
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, op1 == 0u);
}

template<auto ReadOperands>
auto GetSibling(ZorkImpl& impl) -> void {
    const auto& [object] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto& branch = impl.ReadInstructionBranch();
    const auto sibling = impl.ReadObjectSibling(object);
    impl.WriteVariable(variable, sibling);
    impl.DoBranch(branch, sibling != 0u);
}

template<auto ReadOperands>
auto GetChild(ZorkImpl& impl) -> void {
    const auto& [object] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto& branch = impl.ReadInstructionBranch();
    const auto child = impl.ReadObjectChild(object);
    impl.WriteVariable(variable, child);
    impl.DoBranch(branch, child != 0u);
}

template<auto ReadOperands>
auto GetParent(ZorkImpl& impl) -> void {
    const auto& [object] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    const auto parent = impl.ReadObjectParent(object);
    impl.WriteVariable(variable, parent);
}

template<auto ReadOperands>
auto GetPropLen(ZorkImpl& impl) -> void {
    const auto& [address] = ReadOperands(impl);
    const auto variable = impl.ReadInstruction8();
    if (address == 0u) {
        impl.WriteVariable(variable, 0u);
    } else {
        const auto& [number, size] = impl.ReadObjectPropertyNumberAndSize(address - 1u);
        impl.WriteVariable(variable, size);
    }
}

template<auto ReadOperands>
auto Inc(ZorkImpl& impl) -> void {
    const auto& [variable] = ReadOperands(impl);
    const auto value = impl.ReadVariable(variable) + 1u;
    impl.WriteVariable(variable, value);
}

template<auto ReadOperands>
auto Dec(ZorkImpl& impl) -> void {
    const auto& [variable] = ReadOperands(impl);
    const auto value = impl.ReadVariable(variable) - 1u;
    impl.WriteVariable(variable, value);
}

template<auto ReadOperands>
auto PrintAddr(ZorkImpl& impl) -> void {
    const auto& [address] = ReadOperands(impl);
    impl.PrintString(address);
}

template<auto ReadOperands>
auto RemoveObj(ZorkImpl& impl) -> void {
    const auto& [object] = ReadOperands(impl);
    impl.RemoveObject(object);
}

template<auto ReadOperands>
auto PrintObj(ZorkImpl& impl) -> void {
    const auto& [object] = ReadOperands(impl);
    const auto address = impl.GetObjectNameAddress(object);
    impl.PrintString(address);
}

template<auto ReadOperands>
auto Ret(ZorkImpl& impl) -> void {
    const auto& [op1] = ReadOperands(impl);
    impl.DoReturn(op1);
}

template<auto ReadOperands>
auto Jump(ZorkImpl& impl) -> void {
    const auto& [op1] = ReadOperands(impl);
    const auto target = impl.GetProgramCounter() + SignExtend16(op1) - 2u;
    impl.SetProgramCounter(target);
}

template<auto ReadOperands>
auto PrintPaddr(ZorkImpl& impl) -> void {
    const auto& [address] = ReadOperands(impl);
    impl.PrintString(impl.GetUnpackedAddress(address));
}

template<auto ReadOperands>
auto Load(ZorkImpl& impl) -> void {
    const auto& [variable] = ReadOperands(impl);
    const auto destination = impl.ReadInstruction8();
    const auto value = impl.ReadVariable(variable);
    impl.WriteVariable(destination, value);
}

template<auto ReadOperands>
auto Not(ZorkImpl&) -> void {
    Unimplemented("not");
}

// 0OPS
auto Rtrue(ZorkImpl& impl) -> void {
    impl.DoReturn(Z_MACHINE_TRUE);
}

auto Rfalse(ZorkImpl& impl) -> void {
    impl.DoReturn(Z_MACHINE_FALSE);
}

auto Print(ZorkImpl& impl) -> void {
    const auto pc = impl.GetProgramCounter();
    const auto target = impl.PrintString(pc);
    impl.SetProgramCounter(target);
}

auto PrintRet(ZorkImpl& impl) -> void {
    impl.PrintString(impl.GetProgramCounter());
    impl.PrintCharacter('\n');
    impl.DoReturn(Z_MACHINE_TRUE);
}

auto Nop(ZorkImpl&) -> void {}

auto Save(ZorkImpl&) -> void {
    Unimplemented("save");
}

auto Restore(ZorkImpl&) -> void {
    Unimplemented("restore");
}

auto Restart(ZorkImpl& impl) -> void {
    impl.Restart();
}

auto RetPopped(ZorkImpl& impl) -> void {
    impl.DoReturn(impl.ReadVariable(0u));
}

auto Pop(ZorkImpl&) -> void {
    Unimplemented("pop");
}

auto Quit(ZorkImpl& impl) -> void {
    impl.SetRunning(false);
}

auto NewLine(ZorkImpl& impl) -> void {
    impl.PrintCharacter('\n');
}

auto ShowStatus(ZorkImpl&) -> void {
    Unimplemented("showStatus");
}

auto Verify(ZorkImpl& impl) -> void {
    const auto& branch = impl.ReadInstructionBranch();
    impl.DoBranch(branch, impl.IsChecksumValid());
}

// VAR
auto Call(ZorkImpl& impl) -> void {
    const auto& arguments = ReadOperandsByType(impl);
    const auto variable = impl.ReadInstruction8();
    impl.DoCall(arguments.empty() ? 0u: arguments[0], variable, { arguments.cbegin() + 1u, arguments.cend()});
}

auto StoreW(ZorkImpl& impl) -> void {
    const auto& [arrayAddress, element, value] = ReadOperands3(impl);
    impl.MemoryWrite16(arrayAddress + 2u * element, value);
}

auto StoreB(ZorkImpl& impl) -> void {
    const auto& [arrayAddress, element, value] = ReadOperands3(impl);
    impl.MemoryWrite8(arrayAddress + element, value);
}

auto PutProp(ZorkImpl& impl) -> void {
    const auto& [object, property, value] = ReadOperands3(impl);
    impl.WriteObjectProperty(object, property, value);
}

auto Sread(ZorkImpl& impl) -> void {
    const auto& [textBuffer, parseBuffer] = ReadOperands2(impl);
    const auto text = impl.ReadLine();
    const auto tokens = impl.TokenizeString(text);

    impl.WriteToTextBuffer(textBuffer, text);
    impl.WriteToParseBuffer(parseBuffer, tokens);
}

auto PrintChar(ZorkImpl& impl) -> void {
    const auto& [c] = ReadOperands1(impl);
    impl.PrintCharacter(c);
}

auto PrintNum(ZorkImpl& impl) -> void {
    const auto& [op1] = ReadOperands1(impl);
    impl.PrintNumber(op1);
}

auto Random(ZorkImpl& impl) -> void {
    const auto& [range] = ReadOperands1(impl);
    const auto variable = impl.ReadInstruction8();
    const auto random = impl.DoRandom(range);
    impl.WriteVariable(variable, random);
}

auto Push(ZorkImpl& impl) -> void {
    const auto& [value] = ReadOperands1(impl);
    impl.WriteVariable(VARIABLE_STACK, value);
}

auto Pull(ZorkImpl& impl) -> void {
    const auto& [variable] = ReadOperands1(impl);
    const auto value = impl.ReadVariable(VARIABLE_STACK);
    impl.WriteVariable(variable, value);
}

auto SplitWindow(ZorkImpl&) -> void {
    Unimplemented("splitWindow");
}

auto SetWindow(ZorkImpl&) -> void {
    Unimplemented("setWindow");
}

auto OutputStream(ZorkImpl&) -> void {
    Unimplemented("outputStream");
}

auto InputStream(ZorkImpl&) -> void {
    Unimplemented("inputStream");
}

auto SoundEffect(ZorkImpl&) -> void {
    Unimplemented("soundEffect");
}
}

namespace fiction::zork {

auto Execute(ZorkImpl& impl) -> void {
    const auto opcode = impl.ReadInstruction8();
    switch (opcode) {
        case 0x00: return Illegal(impl);
        case 0x01: return Je<ReadSmallSmall>(impl);
        case 0x02: return Jl<ReadSmallSmall>(impl);
        case 0x03: return Jg<ReadSmallSmall>(impl);
        case 0x04: return DecChk<ReadSmallSmall>(impl);
        case 0x05: return IncChk<ReadSmallSmall>(impl);
        case 0x06: return Jin<ReadSmallSmall>(impl);
        case 0x07: return Test<ReadSmallSmall>(impl);
        case 0x08: return Or<ReadSmallSmall>(impl);
        case 0x09: return And<ReadSmallSmall>(impl);
        case 0x0A: return TestAttr<ReadSmallSmall>(impl);
        case 0x0B: return SetAttr<ReadSmallSmall>(impl);
        case 0x0C: return ClearAttr<ReadSmallSmall>(impl);
        case 0x0D: return Store<ReadSmallSmall>(impl);
        case 0x0E: return InsertObj<ReadSmallSmall>(impl);
        case 0x0F: return LoadW<ReadSmallSmall>(impl);
        case 0x10: return LoadB<ReadSmallSmall>(impl);
        case 0x11: return GetProp<ReadSmallSmall>(impl);
        case 0x12: return GetPropAddr<ReadSmallSmall>(impl);
        case 0x13: return GetNextProp<ReadSmallSmall>(impl);
        case 0x14: return Add<ReadSmallSmall>(impl);
        case 0x15: return Sub<ReadSmallSmall>(impl);
        case 0x16: return Mul<ReadSmallSmall>(impl);
        case 0x17: return Div<ReadSmallSmall>(impl);
        case 0x18: return Mod<ReadSmallSmall>(impl);

        case 0x20: return Illegal(impl);
        case 0x21: return Je<ReadSmallVariable>(impl);
        case 0x22: return Jl<ReadSmallVariable>(impl);
        case 0x23: return Jg<ReadSmallVariable>(impl);
        case 0x24: return DecChk<ReadSmallVariable>(impl);
        case 0x25: return IncChk<ReadSmallVariable>(impl);
        case 0x26: return Jin<ReadSmallVariable>(impl);
        case 0x27: return Test<ReadSmallVariable>(impl);
        case 0x28: return Or<ReadSmallVariable>(impl);
        case 0x29: return And<ReadSmallVariable>(impl);
        case 0x2A: return TestAttr<ReadSmallVariable>(impl);
        case 0x2B: return SetAttr<ReadSmallVariable>(impl);
        case 0x2C: return ClearAttr<ReadSmallVariable>(impl);
        case 0x2D: return Store<ReadSmallVariable>(impl);
        case 0x2E: return InsertObj<ReadSmallVariable>(impl);
        case 0x2F: return LoadW<ReadSmallVariable>(impl);
        case 0x30: return LoadB<ReadSmallVariable>(impl);
        case 0x31: return GetProp<ReadSmallVariable>(impl);
        case 0x32: return GetPropAddr<ReadSmallVariable>(impl);
        case 0x33: return GetNextProp<ReadSmallVariable>(impl);
        case 0x34: return Add<ReadSmallVariable>(impl);
        case 0x35: return Sub<ReadSmallVariable>(impl);
        case 0x36: return Mul<ReadSmallVariable>(impl);
        case 0x37: return Div<ReadSmallVariable>(impl);
        case 0x38: return Mod<ReadSmallVariable>(impl);

        case 0x40: return Illegal(impl);
        case 0x41: return Je<ReadVariableSmall>(impl);
        case 0x42: return Jl<ReadVariableSmall>(impl);
        case 0x43: return Jg<ReadVariableSmall>(impl);
        case 0x44: return DecChk<ReadVariableSmall>(impl);
        case 0x45: return IncChk<ReadVariableSmall>(impl);
        case 0x46: return Jin<ReadVariableSmall>(impl);
        case 0x47: return Test<ReadVariableSmall>(impl);
        case 0x48: return Or<ReadVariableSmall>(impl);
        case 0x49: return And<ReadVariableSmall>(impl);
        case 0x4A: return TestAttr<ReadVariableSmall>(impl);
        case 0x4B: return SetAttr<ReadVariableSmall>(impl);
        case 0x4C: return ClearAttr<ReadVariableSmall>(impl);
        case 0x4D: return Store<ReadVariableSmall>(impl);
        case 0x4E: return InsertObj<ReadVariableSmall>(impl);
        case 0x4F: return LoadW<ReadVariableSmall>(impl);
        case 0x50: return LoadB<ReadVariableSmall>(impl);
        case 0x51: return GetProp<ReadVariableSmall>(impl);
        case 0x52: return GetPropAddr<ReadVariableSmall>(impl);
        case 0x53: return GetNextProp<ReadVariableSmall>(impl);
        case 0x54: return Add<ReadVariableSmall>(impl);
        case 0x55: return Sub<ReadVariableSmall>(impl);
        case 0x56: return Mul<ReadVariableSmall>(impl);
        case 0x57: return Div<ReadVariableSmall>(impl);
        case 0x58: return Mod<ReadVariableSmall>(impl);

        case 0x60: return Illegal(impl);
        case 0x61: return Je<ReadVariableVariable>(impl);
        case 0x62: return Jl<ReadVariableVariable>(impl);
        case 0x63: return Jg<ReadVariableVariable>(impl);
        case 0x64: return DecChk<ReadVariableVariable>(impl);
        case 0x65: return IncChk<ReadVariableVariable>(impl);
        case 0x66: return Jin<ReadVariableVariable>(impl);
        case 0x67: return Test<ReadVariableVariable>(impl);
        case 0x68: return Or<ReadVariableVariable>(impl);
        case 0x69: return And<ReadVariableVariable>(impl);
        case 0x6A: return TestAttr<ReadVariableVariable>(impl);
        case 0x6B: return SetAttr<ReadVariableVariable>(impl);
        case 0x6C: return ClearAttr<ReadVariableVariable>(impl);
        case 0x6D: return Store<ReadVariableVariable>(impl);
        case 0x6E: return InsertObj<ReadVariableVariable>(impl);
        case 0x6F: return LoadW<ReadVariableVariable>(impl);
        case 0x70: return LoadB<ReadVariableVariable>(impl);
        case 0x71: return GetProp<ReadVariableVariable>(impl);
        case 0x72: return GetPropAddr<ReadVariableVariable>(impl);
        case 0x73: return GetNextProp<ReadVariableVariable>(impl);
        case 0x74: return Add<ReadVariableVariable>(impl);
        case 0x75: return Sub<ReadVariableVariable>(impl);
        case 0x76: return Mul<ReadVariableVariable>(impl);
        case 0x77: return Div<ReadVariableVariable>(impl);
        case 0x78: return Mod<ReadVariableVariable>(impl);

        case 0x80: return Jz<ReadLarge>(impl);
        case 0x81: return GetSibling<ReadLarge>(impl);
        case 0x82: return GetChild<ReadLarge>(impl);
        case 0x83: return GetParent<ReadLarge>(impl);
        case 0x84: return GetPropLen<ReadLarge>(impl);
        case 0x85: return Inc<ReadLarge>(impl);
        case 0x86: return Dec<ReadLarge>(impl);
        case 0x87: return PrintAddr<ReadLarge>(impl);
        case 0x88: return Illegal(impl);
        case 0x89: return RemoveObj<ReadLarge>(impl);
        case 0x8A: return PrintObj<ReadLarge>(impl);
        case 0x8B: return Ret<ReadLarge>(impl);
        case 0x8C: return Jump<ReadLarge>(impl);
        case 0x8D: return PrintPaddr<ReadLarge>(impl);
        case 0x8E: return Load<ReadLarge>(impl);
        case 0x8F: return Not<ReadLarge>(impl);

        case 0x90: return Jz<ReadSmall>(impl);
        case 0x91: return GetSibling<ReadSmall>(impl);
        case 0x92: return GetChild<ReadSmall>(impl);
        case 0x93: return GetParent<ReadSmall>(impl);
        case 0x94: return GetPropLen<ReadSmall>(impl);
        case 0x95: return Inc<ReadSmall>(impl);
        case 0x96: return Dec<ReadSmall>(impl);
        case 0x97: return PrintAddr<ReadSmall>(impl);
        case 0x98: return Illegal(impl);
        case 0x99: return RemoveObj<ReadSmall>(impl);
        case 0x9A: return PrintObj<ReadSmall>(impl);
        case 0x9B: return Ret<ReadSmall>(impl);
        case 0x9C: return Jump<ReadSmall>(impl);
        case 0x9D: return PrintPaddr<ReadSmall>(impl);
        case 0x9E: return Load<ReadSmall>(impl);
        case 0x9F: return Not<ReadSmall>(impl);

        case 0xA0: return Jz<ReadVariable>(impl);
        case 0xA1: return GetSibling<ReadVariable>(impl);
        case 0xA2: return GetChild<ReadVariable>(impl);
        case 0xA3: return GetParent<ReadVariable>(impl);
        case 0xA4: return GetPropLen<ReadVariable>(impl);
        case 0xA5: return Inc<ReadVariable>(impl);
        case 0xA6: return Dec<ReadVariable>(impl);
        case 0xA7: return PrintAddr<ReadVariable>(impl);
        case 0xA8: return Illegal(impl);
        case 0xA9: return RemoveObj<ReadVariable>(impl);
        case 0xAA: return PrintObj<ReadVariable>(impl);
        case 0xAB: return Ret<ReadVariable>(impl);
        case 0xAC: return Jump<ReadVariable>(impl);
        case 0xAD: return PrintPaddr<ReadVariable>(impl);
        case 0xAE: return Load<ReadVariable>(impl);
        case 0xAF: return Not<ReadVariable>(impl);

        case 0xB0: return Rtrue(impl);
        case 0xB1: return Rfalse(impl);
        case 0xB2: return Print(impl);
        case 0xB3: return PrintRet(impl);
        case 0xB4: return Nop(impl);
        case 0xB5: return Save(impl);
        case 0xB6: return Restore(impl);
        case 0xB7: return Restart(impl);
        case 0xB8: return RetPopped(impl);
        case 0xB9: return Pop(impl);
        case 0xBA: return Quit(impl);
        case 0xBB: return NewLine(impl);
        case 0xBC: return ShowStatus(impl);
        case 0xBD: return Verify(impl);

        case 0xC0: return Illegal(impl);
        case 0xC1: return JeVariable(impl);
        case 0xC2: return Jl<ReadOperands2>(impl);
        case 0xC3: return Jg<ReadOperands2>(impl);
        case 0xC4: return DecChk<ReadOperands2>(impl);
        case 0xC5: return IncChk<ReadOperands2>(impl);
        case 0xC6: return Jin<ReadOperands2>(impl);
        case 0xC7: return Test<ReadOperands2>(impl);
        case 0xC8: return Or<ReadOperands2>(impl);
        case 0xC9: return And<ReadOperands2>(impl);
        case 0xCA: return TestAttr<ReadOperands2>(impl);
        case 0xCB: return SetAttr<ReadOperands2>(impl);
        case 0xCC: return ClearAttr<ReadOperands2>(impl);
        case 0xCD: return Store<ReadOperands2>(impl);
        case 0xCE: return InsertObj<ReadOperands2>(impl);
        case 0xCF: return LoadW<ReadOperands2>(impl);
        case 0xD0: return LoadB<ReadOperands2>(impl);
        case 0xD1: return GetProp<ReadOperands2>(impl);
        case 0xD2: return GetPropAddr<ReadOperands2>(impl);
        case 0xD3: return GetNextProp<ReadOperands2>(impl);
        case 0xD4: return Add<ReadOperands2>(impl);
        case 0xD5: return Sub<ReadOperands2>(impl);
        case 0xD6: return Mul<ReadOperands2>(impl);
        case 0xD7: return Div<ReadOperands2>(impl);
        case 0xD8: return Mod<ReadOperands2>(impl);

        case 0xE0: return Call(impl);
        case 0xE1: return StoreW(impl);
        case 0xE2: return StoreB(impl);
        case 0xE3: return PutProp(impl);
        case 0xE4: return Sread(impl);
        case 0xE5: return PrintChar(impl);
        case 0xE6: return PrintNum(impl);
        case 0xE7: return Random(impl);
        case 0xE8: return Push(impl);
        case 0xE9: return Pull(impl);
        case 0xEA: return SplitWindow(impl);
        case 0xEB: return SetWindow(impl);
        case 0xF3: return OutputStream(impl);
        case 0xF4: return InputStream(impl);
        case 0xF5: return SoundEffect(impl);

        default: return Illegal(impl);

    }
}

}