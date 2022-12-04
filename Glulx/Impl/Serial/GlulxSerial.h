#pragma once

#include <cstdint>
#include <vector>

namespace fiction::glulx {

class GlulxMemory;
class GlulxStack;

auto WriteIff(const GlulxMemory&, const GlulxStack&) -> std::vector<uint8_t>;

auto ReadIff(const std::vector<uint8_t>&, GlulxMemory&, GlulxStack&) -> bool;

}