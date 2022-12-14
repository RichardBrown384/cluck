#pragma once

namespace fiction::zork {

[[noreturn]] auto Error(const char*) -> void;
[[noreturn]] auto ErrorFormat(const char* format, ...) -> void;

}