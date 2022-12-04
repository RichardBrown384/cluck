#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxWriteBuffer {
public:
    explicit GlulxWriteBuffer(uint32_t size) : buffer(), position(0) { buffer.reserve(size); }

    GlulxWriteBuffer(const GlulxWriteBuffer&) = delete;
    GlulxWriteBuffer(GlulxWriteBuffer&&) = delete;
    GlulxWriteBuffer& operator=(const GlulxWriteBuffer&) = delete;
    GlulxWriteBuffer& operator=(GlulxWriteBuffer&&) = delete;

    [[nodiscard]] auto GetSize() const -> uint32_t;
    [[nodiscard]] auto GetPosition() const -> uint32_t;

    [[nodiscard]] auto GetBuffer() const -> const std::vector<uint8_t>&;

    auto Write8At(uint32_t, uint32_t) -> void;
    auto Write32At(uint32_t, uint32_t) -> void;
    auto Write8(uint32_t) -> void;
    auto Write32(uint32_t) -> void;
    auto WriteBuffer(const std::vector<uint8_t>&) -> void;
    auto Align16() -> void;

private:
    std::vector<uint8_t> buffer;
    uint32_t position;
};

}