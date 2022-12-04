#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxImpl;
struct GlulxHeader;

class GlulxStack {
public:
    explicit GlulxStack(const GlulxHeader&);
    GlulxStack(const GlulxStack&) = delete;
    GlulxStack(GlulxStack&&) = delete;
    GlulxStack& operator=(const GlulxStack&) = delete;
    GlulxStack& operator=(GlulxStack&&) = delete;
    ~GlulxStack();

    [[nodiscard]] auto GetSize() const -> uint32_t { return stack.size(); }

    [[nodiscard]] auto GetSummary() const -> std::vector<uint8_t>;
    auto SetSummary(const std::vector<uint8_t>&) -> void;

    auto Restart() -> void;

    [[nodiscard]] auto GetStackPointer() const -> uint32_t;
    [[nodiscard]] auto GetFramePointer() const -> uint32_t;
    [[nodiscard]] auto GetLocalsPointer() const -> uint32_t;
    [[nodiscard]] auto GetValuesPointer() const -> uint32_t;

    auto SetStackPointer(uint32_t) -> uint32_t;
    auto SetFramePointer(uint32_t) -> uint32_t;

    [[nodiscard]] auto ReadLocal8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadLocal16(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadLocal32(uint32_t) const-> uint32_t;

    auto WriteLocal8(uint32_t, uint32_t) -> void;
    auto WriteLocal16(uint32_t, uint32_t) -> void;
    auto WriteLocal32(uint32_t, uint32_t) -> void;

    auto Push8(uint32_t) -> void;
    auto Push16(uint32_t) -> void;
    auto Push32(uint32_t) -> void;

    auto Pop32() -> uint32_t;

    auto PopArguments(uint32_t) -> std::vector<uint32_t>;

    [[nodiscard]] auto GetCount() const -> uint32_t;
    [[nodiscard]] auto Peek(uint32_t) const -> uint32_t;
    auto Rotate(uint32_t, uint32_t) -> void;
    auto Copy(uint32_t) -> void;

private:
    [[nodiscard]] auto Read8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto Read16(uint32_t) const -> uint32_t;
    [[nodiscard]] auto Read32(uint32_t) const -> uint32_t;

    auto Write8(uint32_t, uint32_t) -> void;
    auto Write16(uint32_t, uint32_t) -> void;
    auto Write32(uint32_t, uint32_t) -> void;

    std::vector<uint8_t> stack;
    uint32_t stackpointer;
    uint32_t framepointer;
};

}