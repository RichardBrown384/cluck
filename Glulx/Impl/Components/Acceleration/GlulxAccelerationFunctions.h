#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxImpl;

auto Function1ZRegion(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function2CpTab(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function3RaPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function4RlPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function5OcCl(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function6RvPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function7OpPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function8CpTab(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function9RaPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function10RlPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function11OcCl(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function12RvPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;
auto Function13OpPr(GlulxImpl&, const std::vector<uint32_t>&) -> uint32_t;

}