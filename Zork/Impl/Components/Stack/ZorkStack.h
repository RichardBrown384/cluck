#pragma once

#include <cstdint>
#include <vector>

namespace fiction::zork {

class ZorkStack {
public:
    explicit ZorkStack();
    ZorkStack(const ZorkStack&) = delete;
    ZorkStack(ZorkStack&&) = delete;
    ZorkStack& operator=(const ZorkStack&) = delete;
    ZorkStack& operator=(ZorkStack&&) = delete;
    ~ZorkStack();

    auto Restart() -> void;

    auto PushStackFrame(uint32_t, uint32_t, const std::vector<uint32_t>&, const std::vector<uint32_t>&) -> void;
    auto PopStackFrame() -> std::tuple<uint32_t, uint32_t, uint32_t>;

    auto PushEvaluation(uint32_t) -> void;
    auto PopEvaluation() -> uint32_t;

    [[nodiscard]] auto ReadLocal(uint32_t) const -> uint32_t;
    auto WriteLocal(uint32_t, uint32_t) -> void;

private:
    [[nodiscard]] auto GetStackPointer() const -> uint32_t;
    auto SetStackPointer(uint32_t) -> void;

    auto Push8(uint32_t) -> void;
    auto Push16(uint32_t) -> void;
    auto Push24(uint32_t) -> void;

    auto Pop8() -> uint32_t;
    auto Pop16() -> uint32_t;
    auto Pop24() -> uint32_t;

    [[nodiscard]] auto GetFramePointer() const -> uint32_t;
    auto SetFramePointer(uint32_t) -> void;

    auto PushFramePointer() -> void;
    auto PopFramePointer() -> void;

    [[nodiscard]] auto GetSuppliedArgumentsPointer() const -> uint32_t;
    [[nodiscard]] auto GetEvaluationCountPointer() const -> uint32_t;
    [[nodiscard]] auto GetLocalPointer(uint32_t) const -> uint32_t;

    std::vector<uint32_t> framePointers;
    std::vector<uint8_t> stack;
    uint32_t stackPointer;
    uint32_t framePointer;
};
}