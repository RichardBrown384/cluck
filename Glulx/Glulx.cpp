#include "Glulx.h"
#include "Impl/GlulxImpl.h"

using namespace fiction::glulx;

Glulx::Glulx(std::vector<uint8_t> file) : impl(std::make_unique<GlulxImpl>(std::move(file))) { }

Glulx::~Glulx() = default;

auto Glulx::IsFileValid() const -> bool { return impl->IsFileValid(); }

auto Glulx::SetInputOutputSystem(uint32_t id, GlulxInputOutputSystem& system) -> void {
    impl->SetInputOutputSystem(id, system);
}

auto Glulx::SetServerFunction(std::function<uint32_t(uint32_t, const std::vector<uint32_t>&)> function) -> void {
    impl->SetServerFunction(std::move(function));
}

auto Glulx::IsRunning() const -> bool { return impl->GetRunning(); }
auto Glulx::Restart() -> void { impl->Restart(); }
auto Glulx::Execute() -> void { impl->Execute(); }

auto Glulx::StackPush32(uint32_t v) -> void { impl->StackPush32(v); }
auto Glulx::StackPop32() -> uint32_t { return impl->StackPop32(); }

auto Glulx::MemoryRead8(uint32_t address) const -> uint32_t { return impl->MemoryRead8(address); }
auto Glulx::MemoryRead32(uint32_t address) const -> uint32_t { return impl->MemoryRead32(address); }

auto Glulx::MemoryWrite8(uint32_t address, uint32_t v) -> void { impl->MemoryWrite8(address, v); }
auto Glulx::MemoryWrite32(uint32_t address, uint32_t v) -> void { impl->MemoryWrite32(address, v); }