#pragma once

#include <cstdint>
#include <array>

namespace fiction::glulx {

class GlulxImpl;

constexpr auto OPERAND_SIZE_8 = 1u;
constexpr auto OPERAND_SIZE_16 = 2u;
constexpr auto OPERAND_SIZE_32 = 4u;

constexpr auto MODE_CONSTANT_ZERO = 0u;
constexpr auto MODE_CONSTANT_SIGNED_8 = 1u;
constexpr auto MODE_CONSTANT_SIGNED_16 = 2u;
constexpr auto MODE_CONSTANT_SIGNED_32 = 3u;
constexpr auto MODE_ADDRESS_8 = 5u;
constexpr auto MODE_ADDRESS_16 = 6u;
constexpr auto MODE_ADDRESS_32 = 7u;
constexpr auto MODE_STACK = 8u;
constexpr auto MODE_CALL_FRAME_8 = 9u;
constexpr auto MODE_CALL_FRAME_16 = 10u;
constexpr auto MODE_CALL_FRAME_32 = 11u;
constexpr auto MODE_RAM_8 = 13u;
constexpr auto MODE_RAM_16 = 14u;
constexpr auto MODE_RAM_32 = 15u;

struct GlulxOperand {
    uint32_t desttype;
    uint32_t value;
};

auto OpcodeStreamRead8(GlulxImpl&) -> uint32_t;
auto OpcodeStreamRead16(GlulxImpl&) -> uint32_t;
auto OpcodeStreamRead32(GlulxImpl&) -> uint32_t;

auto OpcodeRead(GlulxImpl&) -> uint32_t;

auto OperandAddressModesRead2(GlulxImpl&) -> std::array<uint32_t, 2>;
auto OperandAddressModesRead4(GlulxImpl&) -> std::array<uint32_t, 4>;
auto OperandAddressModesRead6(GlulxImpl&) -> std::array<uint32_t, 6>;
auto OperandAddressModesRead8(GlulxImpl&) -> std::array<uint32_t, 8>;

auto LoadOperandRead(GlulxImpl&, uint32_t, uint32_t = OPERAND_SIZE_32) -> GlulxOperand;
auto StoreOperandRead(GlulxImpl&, uint32_t) -> GlulxOperand;

auto OperandsReadL(GlulxImpl&) -> std::array<GlulxOperand, 1>;
auto OperandsReadLL(GlulxImpl&) -> std::array<GlulxOperand, 2>;
auto OperandsReadLLL(GlulxImpl&) -> std::array<GlulxOperand, 3>;
auto OperandsReadLLLL(GlulxImpl&) -> std::array<GlulxOperand, 4>;
auto OperandsReadLLLLL(GlulxImpl&) -> std::array<GlulxOperand, 5>;
auto OperandsReadLLLLLLL(GlulxImpl&) -> std::array<GlulxOperand, 7>;

auto OperandsReadS(GlulxImpl&) -> std::array<GlulxOperand, 1>;
auto OperandsReadLS(GlulxImpl&) -> std::array<GlulxOperand, 2>;
auto OperandsReadL8S(GlulxImpl&) -> std::array<GlulxOperand, 2>;
auto OperandsReadL16S(GlulxImpl&) -> std::array<GlulxOperand, 2>;
auto OperandsReadLLS(GlulxImpl&) -> std::array<GlulxOperand, 3>;
auto OperandsReadLLLS(GlulxImpl&) -> std::array<GlulxOperand, 4>;
auto OperandsReadLLLLS(GlulxImpl&) -> std::array<GlulxOperand, 5>;
auto OperandsReadLLLLLLS(GlulxImpl&) -> std::array<GlulxOperand, 7>;
auto OperandsReadLLLLLLLS(GlulxImpl&) -> std::array<GlulxOperand, 8>;

auto OperandsReadSL(GlulxImpl&) -> std::array<GlulxOperand, 2>;

auto OperandsReadSS(GlulxImpl&) -> std::array<GlulxOperand, 2>;
auto OperandsReadLSS(GlulxImpl&) -> std::array<GlulxOperand, 3>;
auto OperandsReadLLSS(GlulxImpl&) -> std::array<GlulxOperand, 4>;
auto OperandsReadLLLLSS(GlulxImpl&) -> std::array<GlulxOperand, 6>;

auto StoreOperandWrite8(GlulxImpl&, const GlulxOperand&, uint32_t) -> void;
auto StoreOperandWrite16(GlulxImpl&, const GlulxOperand&, uint32_t) -> void;
auto StoreOperandWrite32(GlulxImpl&, const GlulxOperand&, uint32_t) -> void;
auto StoreOperandWriteFloat(GlulxImpl&, const GlulxOperand&, float) -> void;
auto StoreOperandWriteDouble(GlulxImpl&, const GlulxOperand&, const GlulxOperand&, double) -> void;

}