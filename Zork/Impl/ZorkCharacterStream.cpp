#include "ZorkImpl.h"

#include "ZorkCharacterStream.h"

using namespace fiction::zork;

ZorkCharacterStream::ZorkCharacterStream(ZorkImpl& impl_in, uint32_t address_in) :
    impl(impl_in),
    done(false),
    address(address_in),
    buffer(0u),
    remaining(0u) {}

auto ZorkCharacterStream::GetAddress() const -> uint32_t { return address; }
auto ZorkCharacterStream::Done() const -> bool { return done; }

auto ZorkCharacterStream::Next() -> uint32_t {
    if (Done()) {
        return 0u;
    }
    if (remaining == 0u) {
        buffer = impl.MemoryRead16(address);
        address += 2;
        remaining = 3u;
    }
    auto shift = 10u - 5u * (3u - remaining);
    --remaining;
    done = (remaining == 0) && ((buffer & 0x8000u) != 0u);
    return (buffer >> shift) & 0x1Fu;
}