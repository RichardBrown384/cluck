#include "Impl/GlulxImpl.h"
#include "GlulxInstruction.h"

#include "Util/Endian.h"

namespace fiction::glulx {

auto OpcodeStreamRead8(GlulxImpl& glulx) -> uint32_t {
    const auto pc = glulx.GetProgramCounter();
    const auto v = glulx.MemoryRead8(pc);
    glulx.SetProgramCounter(pc + 1u);
    return v;
}

auto OpcodeStreamRead16(GlulxImpl& glulx) -> uint32_t {
    const auto pc = glulx.GetProgramCounter();
    const auto v = glulx.MemoryRead16(pc);
    glulx.SetProgramCounter(pc + 2u);
    return v;
}

auto OpcodeStreamRead32(GlulxImpl& glulx) -> uint32_t {
    const auto pc = glulx.GetProgramCounter();
    const auto v = glulx.MemoryRead32(pc);
    glulx.SetProgramCounter(pc + 4u);
    return v;
}

auto OpcodeRead(GlulxImpl& glulx) -> uint32_t {
    using namespace fiction::endian;
    const auto a = OpcodeStreamRead8(glulx);
    const auto size = (a >> 6);
    if (size < 2) {
        return a;
    } else if (size < 3) {
        const auto b = OpcodeStreamRead8(glulx);
        return PackBigEndian16(a & 0x3F, b);
    } else {
        const auto b = OpcodeStreamRead8(glulx);
        const auto c = OpcodeStreamRead8(glulx);
        const auto d = OpcodeStreamRead8(glulx);
        return PackBigEndian32(a & 0x3F, b, c, d);
    }
}

constexpr auto FirstMode(uint32_t mode) -> uint32_t { return mode & 0xFu; }

constexpr auto SecondMode(uint32_t mode) -> uint32_t { return mode >> 4u; }

auto OperandAddressModesRead2(GlulxImpl& glulx) -> std::array<uint32_t, 2> {
    const auto pair1 = OpcodeStreamRead8(glulx);
    return {
        FirstMode(pair1), SecondMode(pair1)
    };
}

auto OperandAddressModesRead4(GlulxImpl& glulx) -> std::array<uint32_t, 4> {
    const auto pair1 = OpcodeStreamRead8(glulx);
    const auto pair2 = OpcodeStreamRead8(glulx);
    return {
        FirstMode(pair1), SecondMode(pair1),
        FirstMode(pair2), SecondMode(pair2),
    };
}

auto OperandAddressModesRead6(GlulxImpl& glulx) -> std::array<uint32_t, 6> {
    const auto pair1 = OpcodeStreamRead8(glulx);
    const auto pair2 = OpcodeStreamRead8(glulx);
    const auto pair3 = OpcodeStreamRead8(glulx);
    return {
        FirstMode(pair1), SecondMode(pair1),
        FirstMode(pair2), SecondMode(pair2),
        FirstMode(pair3), SecondMode(pair3),
    };
}

auto OperandAddressModesRead8(GlulxImpl& glulx) -> std::array<uint32_t, 8> {
    const auto pair1 = OpcodeStreamRead8(glulx);
    const auto pair2 = OpcodeStreamRead8(glulx);
    const auto pair3 = OpcodeStreamRead8(glulx);
    const auto pair4 = OpcodeStreamRead8(glulx);
    return {
        FirstMode(pair1), SecondMode(pair1),
        FirstMode(pair2), SecondMode(pair2),
        FirstMode(pair3), SecondMode(pair3),
        FirstMode(pair4), SecondMode(pair4),
    };
}

auto OperandsReadL(GlulxImpl& glulx) -> std::array<GlulxOperand, 1> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    return { l1 };
}

auto OperandsReadLL(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    return { l1, l2 };
}

auto OperandsReadLLL(GlulxImpl& glulx) -> std::array<GlulxOperand, 3> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    return { l1, l2, l3 };
}

auto OperandsReadLLLL(GlulxImpl& glulx) -> std::array<GlulxOperand, 4> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    return { l1, l2, l3, l4 };
}

auto OperandsReadLLLLL(GlulxImpl& glulx) -> std::array<GlulxOperand, 5> {
    const auto [m1, m2, m3, m4, m5, m6] = OperandAddressModesRead6(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto l5 = LoadOperandRead(glulx, m5);
    return { l1, l2, l3, l4, l5 };
}

auto OperandsReadLLLLLLL(GlulxImpl& glulx) -> std::array<GlulxOperand, 7> {
    const auto [m1, m2, m3, m4, m5, m6, m7, m8] = OperandAddressModesRead8(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto l5 = LoadOperandRead(glulx, m5);
    const auto l6 = LoadOperandRead(glulx, m6);
    const auto l7 = LoadOperandRead(glulx, m7);
    return { l1, l2, l3, l4, l5, l6, l7 };
}

auto OperandsReadS(GlulxImpl& glulx) -> std::array<GlulxOperand, 1> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto s1 = StoreOperandRead(glulx, m1);
    return { s1 };
}

auto OperandsReadLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto s1 = StoreOperandRead(glulx, m2);
    return { l1, s1 };
}

auto OperandsReadL8S(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto l1 = LoadOperandRead(glulx, m1, OPERAND_SIZE_8);
    const auto s1 = StoreOperandRead(glulx, m2);
    return { l1, s1 };
}

auto OperandsReadL16S(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto l1 = LoadOperandRead(glulx, m1, OPERAND_SIZE_16);
    const auto s1 = StoreOperandRead(glulx, m2);
    return { l1, s1 };
}

auto OperandsReadLLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 3> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto s1 = StoreOperandRead(glulx, m3);
    return { l1, l2, s1 };
}

auto OperandsReadLLLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 4> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto s1 = StoreOperandRead(glulx, m4);
    return { l1, l2, l3, s1 };
}

auto OperandsReadLLLLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 5> {
    const auto [m1, m2, m3, m4, m5, m6] = OperandAddressModesRead6(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto s1 = StoreOperandRead(glulx, m5);
    return { l1, l2, l3, l4, s1 };
}

auto OperandsReadLLLLLLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 7> {
    const auto [m1, m2, m3, m4, m5, m6, m7, m8] = OperandAddressModesRead8(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto l5 = LoadOperandRead(glulx, m5);
    const auto l6 = LoadOperandRead(glulx, m6);
    const auto s1 = StoreOperandRead(glulx, m7);
    return { l1, l2, l3, l4, l5, l6, s1 };
}

auto OperandsReadLLLLLLLS(GlulxImpl& glulx) -> std::array<GlulxOperand, 8> {
    const auto [m1, m2, m3, m4, m5, m6, m7, m8] = OperandAddressModesRead8(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto l5 = LoadOperandRead(glulx, m5);
    const auto l6 = LoadOperandRead(glulx, m6);
    const auto l7 = LoadOperandRead(glulx, m7);
    const auto s1 = StoreOperandRead(glulx, m8);
    return { l1, l2, l3, l4, l5, l6, l7, s1 };
}

auto OperandsReadSL(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto s1 = StoreOperandRead(glulx, m1);
    const auto l1 = LoadOperandRead(glulx, m2);
    return { s1, l1 };
}

auto OperandsReadSS(GlulxImpl& glulx) -> std::array<GlulxOperand, 2> {
    const auto [m1, m2] = OperandAddressModesRead2(glulx);
    const auto s1 = StoreOperandRead(glulx, m1);
    const auto s2 = StoreOperandRead(glulx, m2);
    return { s1, s2 };
}

auto OperandsReadLSS(GlulxImpl& glulx) -> std::array<GlulxOperand, 3> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto s1 = StoreOperandRead(glulx, m2);
    const auto s2 = StoreOperandRead(glulx, m3);
    return { l1, s1, s2 };
}

auto OperandsReadLLSS(GlulxImpl& glulx) -> std::array<GlulxOperand, 4> {
    const auto [m1, m2, m3, m4] = OperandAddressModesRead4(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto s1 = StoreOperandRead(glulx, m3);
    const auto s2 = StoreOperandRead(glulx, m4);
    return { l1, l2, s1, s2 };
}

auto OperandsReadLLLLSS(GlulxImpl& glulx) -> std::array<GlulxOperand, 6> {
    const auto [m1, m2, m3, m4, m5, m6] = OperandAddressModesRead6(glulx);
    const auto l1 = LoadOperandRead(glulx, m1);
    const auto l2 = LoadOperandRead(glulx, m2);
    const auto l3 = LoadOperandRead(glulx, m3);
    const auto l4 = LoadOperandRead(glulx, m4);
    const auto s1 = StoreOperandRead(glulx, m5);
    const auto s2 = StoreOperandRead(glulx, m6);
    return { l1, l2, l3, l4, s1, s2 };
}

}