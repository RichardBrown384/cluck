#pragma once

#include <memory>
#include <vector>
#include <cstdint>

namespace fiction::glulx {

struct GlulxHeader;

class GlulxMemory {
public:
    GlulxMemory(std::vector<uint8_t>, const GlulxHeader&);
    GlulxMemory(const GlulxMemory&) = delete;
    GlulxMemory(GlulxMemory&&) = delete;
    GlulxMemory& operator=(const GlulxMemory&) = delete;
    GlulxMemory& operator=(GlulxMemory&&) = delete;
    ~GlulxMemory();

    [[nodiscard]] auto GetRomSummary() const -> std::vector<uint8_t>;
    [[nodiscard]] auto GetRamSummary() const -> std::vector<uint8_t>;
    [[nodiscard]] auto GetHeapSummary() const -> std::vector<std::pair<uint32_t,uint32_t>>;

    [[nodiscard]] auto SetRomSummary(const std::vector<uint8_t>&) const -> bool;
    auto SetRamSummary(uint32_t, const std::vector<uint8_t>&) -> void;
    auto SetHeapSummary(uint32_t, const std::vector<std::pair<uint32_t, uint32_t>>&) -> void;

    auto Restart() -> void;

    [[nodiscard]] auto GetSize() const -> uint32_t;
    [[nodiscard]] auto GetRamStart() const -> uint32_t;
    [[nodiscard]] auto GetHeapStart() const -> uint32_t;
    [[nodiscard]] auto GetExtStart() const -> uint32_t;
    [[nodiscard]] auto GetEndMem() const -> uint32_t;

    auto SetProtect(uint32_t, uint32_t) -> void;

    auto HeapClear() -> void;
    auto HeapActive() -> bool;

    auto SetSize(uint32_t) -> bool;

    [[nodiscard]] auto Read8(uint32_t) const -> uint32_t;
    [[nodiscard]] auto Read16(uint32_t) const -> uint32_t;
    [[nodiscard]] auto Read32(uint32_t) const -> uint32_t;

    auto Write8(uint32_t, uint32_t) -> void;
    auto Write8Protected(uint32_t, uint32_t) -> void;
    auto Write16(uint32_t, uint32_t) -> void;
    auto Write32(uint32_t, uint32_t) -> void;

    auto Malloc(uint32_t) -> uint32_t;
    auto Free(uint32_t) -> void;

    struct Data;

private:
    auto CheckReadableAddress(uint32_t, uint32_t) const -> void;
    auto CheckWriteableAddress(uint32_t, uint32_t) const -> void;

    std::unique_ptr<Data> data;
};
}