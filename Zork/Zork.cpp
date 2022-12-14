#include "Zork.h"
#include "Impl/ZorkImpl.h"

using namespace fiction::zork;

Zork::Zork(const std::vector<uint8_t>& file) : impl(std::make_unique<ZorkImpl>(file)) { }

Zork::~Zork() = default;

auto Zork::IsRunning() const -> bool { return impl->GetRunning(); }

auto Zork::Restart() -> void { impl->Restart(); }
auto Zork::Execute() -> void { impl->Execute(); }