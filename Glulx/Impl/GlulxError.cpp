#include "GlulxError.h"

#include <iostream>

namespace fiction::glulx {

auto Error(const char* message) -> void {
    std::cerr << std::endl << "Glulx Error" << std::endl;
    std::cerr << std::endl << message << std::endl;
    std::exit(1);
}

}