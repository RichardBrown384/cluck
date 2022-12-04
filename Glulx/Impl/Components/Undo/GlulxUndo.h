#pragma once

#include <cstdint>
#include <vector>
#include <list>

namespace fiction::glulx {

class GlulxUndo {
public:
    GlulxUndo() = default;
    GlulxUndo(const GlulxUndo&) = delete;
    GlulxUndo(GlulxUndo&&) = delete;
    GlulxUndo& operator=(const GlulxUndo&) = delete;
    GlulxUndo& operator=(GlulxUndo&&) = delete;
    ~GlulxUndo() = default;

    [[nodiscard]] auto Empty() const -> bool;

    auto Push(std::vector<uint8_t>) -> void;
    auto Pop() -> void;
    auto Peek() -> const std::vector<uint8_t>&;

private:
    std::list<std::vector<uint8_t>> glulxs;
};

}