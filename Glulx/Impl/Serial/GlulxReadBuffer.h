#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxReadBuffer {
public:
    explicit GlulxReadBuffer(const std::vector<uint8_t>& buffer_in) : buffer(buffer_in), position(0) {}

    GlulxReadBuffer(const GlulxReadBuffer&) = delete;
    GlulxReadBuffer(GlulxReadBuffer&&) = delete;
    GlulxReadBuffer& operator=(const GlulxReadBuffer&) = delete;
    GlulxReadBuffer& operator=(GlulxReadBuffer&&) = delete;

    [[nodiscard]] auto GetSize() const -> uint32_t;
    [[nodiscard]] auto GetPosition() const -> uint32_t;
    auto Skip(uint32_t) -> void;

    auto Read8() -> uint32_t;
    auto Read32() -> uint32_t;
    auto ReadBuffer(uint32_t length) -> std::vector<uint8_t>;
    auto Align16() -> void;

private:
    const std::vector<uint8_t>& buffer;
    uint32_t position;
};

}