#include <unordered_map>

#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "GlulxAcceleration.h"
#include "GlulxAccelerationParams.h"
#include "GlulxAccelerationFunctions.h"

namespace fiction::glulx {
namespace {
using AcceleratedFunction = uint32_t(*)(GlulxImpl&, const std::vector<uint32_t>&);

constexpr std::array<AcceleratedFunction, 14> FUNCTIONS = {
    nullptr,
    Function1ZRegion,
    Function2CpTab,
    Function3RaPr,
    Function4RlPr,
    Function5OcCl,
    Function6RvPr,
    Function7OpPr,
    Function8CpTab,
    Function9RaPr,
    Function10RlPr,
    Function11OcCl,
    Function12RvPr,
    Function13OpPr
};

}

struct GlulxAcceleration::Data {
    Data();
    std::unordered_map<uint32_t, AcceleratedFunction> functionByAddress;
    std::array<AcceleratedFunction, FUNCTIONS.size()> functions;
    std::array<uint32_t, PARAM_COUNT> parameters;
};

GlulxAcceleration::Data::Data() :
    functionByAddress(),
    functions(FUNCTIONS),
    parameters() {}

GlulxAcceleration::GlulxAcceleration() :
    data(std::make_unique<Data>()){}

GlulxAcceleration::~GlulxAcceleration() = default;

auto GlulxAcceleration::SetParam(uint32_t param, uint32_t value) -> void {
    auto& d = GetData();
    if (param < d.parameters.size()) {
        d.parameters[param] = value;
    }
}

auto GlulxAcceleration::GetParam(uint32_t param) const -> uint32_t {
    const auto& d = GetData();
    if (param < d.parameters.size()) {
        return d.parameters[param];
    }
    return 0u;
}

auto GlulxAcceleration::IsFunctionSupported(uint32_t function) const -> bool {
    return function > 0u && function < GetData().functions.size();
}

auto GlulxAcceleration::SetFunction(GlulxImpl& glulx, uint32_t function, uint32_t address) -> void {
    auto& d = GetData();
    if (function == 0u) {
        d.functionByAddress.erase(address);
    } else if (function < d.functions.size()) {
        const auto type = glulx.MemoryRead8(address);
        if (!IsFunctionType(type)) {
            Error("Attempted to accelerate a non function type");
        }
        d.functionByAddress[address] = d.functions[function];
    }
}

auto GlulxAcceleration::CallFunction(
    GlulxImpl& glulx,
    uint32_t address,
    const std::vector<uint32_t>& arguments,
    uint32_t* result) -> bool {
    auto& d = GetData();
    if (result == nullptr) {
        Error("Attempted to call an accelerated function without being able to capture the result");
    }
    if (d.functionByAddress.contains(address)) {
        *result = d.functionByAddress[address](glulx, arguments);
        return true;
    }
    return false;
}

}