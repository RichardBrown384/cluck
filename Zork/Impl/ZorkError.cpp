#include "ZorkError.h"

#include <iostream>

namespace fiction::zork {

auto Error(const char* message) -> void {
    std::cerr << std::endl << "Zork Error" << std::endl;
    std::cerr << std::endl << message << std::endl;
    std::exit(1);
}

auto ErrorFormat(const char* format, ...) -> void {
    char buffer[80] = { };
    va_list vargs;
    va_start(vargs, format);
    int result = vsnprintf(buffer, sizeof(buffer), format, vargs);
    va_end(vargs);
    if (result >= 0u) {
        Error(buffer);
    }
    Error("Error occurred in formatting error message");
}

}