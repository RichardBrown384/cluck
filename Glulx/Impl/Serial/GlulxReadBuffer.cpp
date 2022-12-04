#include "GlulxReadBuffer.h"

#include "Util/Endian.h"

using namespace fiction::endian;

namespace fiction::glulx {

auto GlulxReadBuffer::GetSize() const -> uint32_t { return buffer.size(); }
auto GlulxReadBuffer::GetPosition() const -> uint32_t { return position; }
auto GlulxReadBuffer::Skip(uint32_t n) -> void { position += n; }

auto GlulxReadBuffer::Read8() -> uint32_t {
    const auto v = buffer[position];
    position += 1u;
    return v;
}

auto GlulxReadBuffer::Read32() -> uint32_t {
    const auto a = Read8();
    const auto b = Read8();
    const auto c = Read8();
    const auto d = Read8();
    return PackBigEndian32(a, b, c, d);
}

auto GlulxReadBuffer::ReadBuffer(uint32_t length) -> std::vector<uint8_t> {
    auto start = position;
    position += length;
    return { buffer.begin() + start, buffer.begin() + start + length };
}

auto GlulxReadBuffer::Align16() -> void {
    position += (position & 1u);
}

}