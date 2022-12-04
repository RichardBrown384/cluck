#include "GlulxWriteBuffer.h"

#include "Impl/GlulxError.h"

namespace fiction::glulx {

auto GlulxWriteBuffer::GetSize() const -> uint32_t { return buffer.size(); }
auto GlulxWriteBuffer::GetPosition() const -> uint32_t { return position; }

auto GlulxWriteBuffer::GetBuffer() const -> const std::vector<uint8_t>& { return buffer; }

auto GlulxWriteBuffer::Write8At(uint32_t p, uint32_t v) -> void {
    if (p < buffer.size()) {
        buffer[p] = static_cast<uint8_t>(v);
        return;
    }
    Error("Tried to write to non-existent buffer position");
}

auto GlulxWriteBuffer::Write32At(uint32_t p, uint32_t v) -> void {
    Write8At(p, v >> 24u);
    Write8At(p + 1u, v >> 16u);
    Write8At(p + 2u, v >> 8u);
    Write8At(p + 3u, v);
}

auto GlulxWriteBuffer::Write8(uint32_t v) -> void {
    buffer.push_back(v);
    position += 1u;
}

auto GlulxWriteBuffer::Write32(uint32_t v) -> void {
    Write8(v >> 24u);
    Write8(v >> 16u);
    Write8(v >> 8u);
    Write8(v);
}

auto GlulxWriteBuffer::WriteBuffer(const std::vector<uint8_t>& buf) -> void {
    for (const auto& v: buf) {
        Write8(v);
    }
}

auto GlulxWriteBuffer::Align16() -> void {
    if (position & 1u) {
        Write8(0u);
    }
}

}