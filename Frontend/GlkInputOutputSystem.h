#include "GlulxInputOutputSystem.h"

namespace fiction::glk {
class GlkServer;
}

class GlkInputOutputSystem : public fiction::glulx::GlulxInputOutputSystem {
public:
    explicit GlkInputOutputSystem(fiction::glk::GlkServer&);
    ~GlkInputOutputSystem() override = default;
    auto PutChar8(fiction::glulx::GlulxImpl&, uint32_t c) -> void override;
    auto PutChar32(fiction::glulx::GlulxImpl&, uint32_t c)  -> void override;
    auto PutBuffer8(fiction::glulx::GlulxImpl&, uint32_t, const std::vector<uint8_t>&) -> void override;
    auto GetBuffer8(fiction::glulx::GlulxImpl&, uint32_t) -> std::vector<uint8_t> override;
private:
    fiction::glk::GlkServer& server;
};