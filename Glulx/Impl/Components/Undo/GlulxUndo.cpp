#include "GlulxUndo.h"

constexpr auto MAX_UNDO = 8u;

using namespace fiction::glulx;

auto GlulxUndo::Empty() const -> bool { return glulxs.empty(); }

auto GlulxUndo::Push(std::vector<uint8_t> buffer) -> void {
    glulxs.push_front(std::move(buffer));
    if (glulxs.size() > MAX_UNDO) {
        glulxs.pop_back();
    }
}

auto GlulxUndo::Pop() -> void { glulxs.pop_front(); }

auto GlulxUndo::Peek() -> const std::vector<uint8_t>& {
    return glulxs.front();
}