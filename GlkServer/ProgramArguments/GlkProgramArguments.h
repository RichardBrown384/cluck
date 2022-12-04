#pragma once

#include <vector>
#include <string>

namespace fiction::glk {

class GlkProgramArguments {
public:
    static GlkProgramArguments& GetInstance() {
        // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
        static GlkProgramArguments instance;
        return instance;
    }
    GlkProgramArguments(const GlkProgramArguments&) = delete;
    GlkProgramArguments(const GlkProgramArguments&&) = delete;
    void operator=(const GlkProgramArguments&) = delete;
    void operator=(const GlkProgramArguments&&) = delete;

    auto GetArguments() -> std::vector<std::string>& { return arguments; }

private:
    GlkProgramArguments() = default;
    std::vector<std::string> arguments;
};
}