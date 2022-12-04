#include <string>

#include "Impl/GlulxImpl.h"
#include "GlulxInputOutputSystem.h"

#include "GlulxAcceleration.h"
#include "GlulxAccelerationFunctions.h"
#include "GlulxAccelerationParams.h"

#include "Impl/Execute/Search/GlulxSearch.h"

// Note: these functions were copied from the reference implementation

namespace {

constexpr auto THIRTY_SIX = 36u;

using namespace fiction::glulx;

auto Warning(GlulxImpl& glulx, const char* message) -> void {
    auto& ios = glulx.GetActiveInputOutputSystem();
    if (ios.IsFiltering()) {
        return;
    }
    std::string_view view(message);
    for (const auto& c : view) {
        ios.PutChar8(glulx, c);
    }
}

constexpr auto GetArgument(const std::vector<uint32_t>& arguments, size_t index) -> uint32_t {
    return arguments[index];
}

constexpr auto GetArgumentIfGiven(const std::vector<uint32_t>& arguments, size_t index) -> uint32_t {
    return (index < arguments.size()) ? GetArgument(arguments, index) : 0u;
}

auto ObjectInClass(GlulxImpl& glulx, uint32_t obj) -> bool {

    const auto classMetaclass = glulx.GetAccelerationParam(PARAM_CLASS_METACLASS);
    const auto numAttrBytes = glulx.GetAccelerationParam(PARAM_NUM_ATTR_BYTES);

    return glulx.MemoryRead32(obj + 13u + numAttrBytes) == classMetaclass;
}

auto GetProperty(GlulxImpl& glulx, uint32_t obj, uint32_t id) -> uint32_t {

    const auto classesTable = glulx.GetAccelerationParam(PARAM_CLASSES_TABLE);
    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);
    const auto self = glulx.GetAccelerationParam(PARAM_SELF);

    auto cla{ 0u };

    if (id & 0xFFFF0000u) {
        cla = glulx.MemoryRead32(classesTable + 4u * (id & 0xFFFFu));
        if (Function5OcCl(glulx, { obj, cla }) == 0) {
            return 0u;
        }
        id >>= 16;
        obj = cla;
    }

    const auto prop = Function2CpTab(glulx, { obj, id });
    if (prop == 0u) {
        return 0u;
    }

    if (ObjectInClass(glulx, obj) && (cla == 0u)) {
        if ((id < individualPropStart) || (id >= individualPropStart + 8u)) {
            return 0u;
        }
    }

    if (glulx.MemoryRead32(self) != obj) {
        if (glulx.MemoryRead8(prop + 9u) & 1u) {
            return 0u;
        }
    }
    return prop;
}

auto GetPropertyNew(GlulxImpl& glulx, uint32_t obj, uint32_t id) -> uint32_t {

    const auto classesTable = glulx.GetAccelerationParam(PARAM_CLASSES_TABLE);
    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);
    const auto self = glulx.GetAccelerationParam(PARAM_SELF);

    auto cla{ 0u };

    if (id & 0xFFFF0000u) {
        cla = glulx.MemoryRead32(classesTable + 4u * (id & 0xFFFFu));
        if (Function11OcCl(glulx, { obj, cla }) == 0) {
            return 0u;
        }
        id >>= 16;
        obj = cla;
    }

    const auto prop = Function8CpTab(glulx, { obj, id });
    if (prop == 0u) {
        return 0u;
    }

    if (ObjectInClass(glulx, obj) && (cla == 0u)) {
        if ((id < individualPropStart) || (id >= individualPropStart + 8u)) {
            return 0u;
        }
    }

    if (glulx.MemoryRead32(self) != obj) {
        if (glulx.MemoryRead8(prop + 9u) & 1u) {
            return 0u;
        }
    }
    return prop;
}

}

namespace fiction::glulx {

auto Function1ZRegion(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    if (arguments.empty()) {
        return 0u;
    }

    const auto address = GetArgument(arguments, 0);

    if (address < THIRTY_SIX || address >= glulx.GetMemSize()) {
        return 0u;
    }

    const auto tb = glulx.MemoryRead8(address);
    if (tb >= 0xE0u) {
        return 3u;
    }
    if (tb >= 0xC0u) {
        return 2u;
    }
    if (tb >= 0x70u && tb <= 0x7Fu && address >= glulx.GetRamStart()) {
        return 1u;
    }
    return 0u;
}


auto Function2CpTab(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    if (Function1ZRegion(glulx, { obj }) != 1u) {
        Warning(glulx, "[** Programming error: tried to find the \".\" of (something) **]");
        return 0u;
    }

    const auto otab = glulx.MemoryRead32(obj + 16u);
    if (!otab) {
        return 0u;
    }

    const auto max = glulx.MemoryRead32(otab);

    return BinarySearch(glulx, {
        .key = id,
        .keySize = 2u,
        .start = otab + 4u,
        .structSize = 10u,
        .numStructs = max,
        .keyOffset = 0u,
        .options = 0u
    });
}

auto Function3RaPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto prop = GetProperty(glulx, obj, id);

    return prop == 0u ? 0u : glulx.MemoryRead32(prop + 4u);
}

auto Function4RlPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto prop = GetProperty(glulx, obj, id);

    return prop == 0u ? 0u : 4u * glulx.MemoryRead16(prop + 2u);
}

auto Function5OcCl(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto classMetaclass = glulx.GetAccelerationParam(PARAM_CLASS_METACLASS);
    const auto objectMetaclass = glulx.GetAccelerationParam(PARAM_OBJECT_METACLASS);
    const auto routineMetaclass = glulx.GetAccelerationParam(PARAM_ROUTINE_METACLASS);
    const auto stringMetaclass = glulx.GetAccelerationParam(PARAM_STRING_METACLASS);

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto cla = GetArgumentIfGiven(arguments, 1);

    const auto zr = Function1ZRegion(glulx, { obj });
    if (zr == 3u) {
        return cla == stringMetaclass ? 1u : 0u;
    }
    if (zr == 2u) {
        return cla == routineMetaclass ? 1u : 0u;
    }
    if (zr != 1u) {
        return 0u;
    }

    if (cla == classMetaclass) {
        if (ObjectInClass(glulx, obj)) { return 1u; }
        if (obj == classMetaclass) { return 1u; }
        if (obj == stringMetaclass) { return 1u; }
        if (obj == routineMetaclass) { return 1u; }
        if (obj == objectMetaclass) { return 1u; }
        return 0u;
    }
    if (cla == objectMetaclass) {
        if (ObjectInClass(glulx, obj)) { return 0u; }
        if (obj == classMetaclass) { return 0u; }
        if (obj == stringMetaclass) { return 0u; }
        if (obj == routineMetaclass) { return 0u; }
        if (obj == objectMetaclass) { return 0u; }
        return 1u;
    }
    if ((cla == stringMetaclass) || (cla == routineMetaclass)) {
        return 0u;
    }

    if (!ObjectInClass(glulx, cla)) {
        Warning(glulx, "[** Programming error: tried to apply 'ofclass' with non-class **]");
        return 0u;
    }

    const auto prop = GetProperty(glulx, obj, 2u);
    if (prop == 0u) {
        return 0u;
    }

    const auto listAddress = glulx.MemoryRead32(prop + 4u);
    if (listAddress == 0u) {
        return 0u;
    }

    const auto listLength = glulx.MemoryRead16(prop + 2u);
    for (auto i = 0u; i < listLength; i++) {
        if (glulx.MemoryRead32(listAddress + 4u * i) == cla) {
            return 1u;
        }
    }
    return 0u;
}

auto Function6RvPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto cpvStart = glulx.GetAccelerationParam(PARAM_CPV_START);
    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);

    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto address = Function3RaPr(glulx, arguments);
    if (address == 0u) {
        if ((id > 0u) && (id < individualPropStart)) {
            return glulx.MemoryRead32(cpvStart + 4u * id);
        }
        Warning(glulx, "[** Programming error: tried to read (something) **]");
        return 0u;
    }

    return glulx.MemoryRead32(address);
}

auto Function7OpPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto zr = Function1ZRegion(glulx, { obj });
    if (zr == 3u) {
        // print is INDIVIDUAL_PROP_START + 6
        if (id == individualPropStart + 6u) { return 1u; }
        // print_to_array is INDIVIDUAL_PROP_START + 7
        if (id == individualPropStart + 7u) { return 1u; }
        return 0u;
    }
    if (zr == 2u) {
        // call is INDIVIDUAL_PROP_START + 5
        return (id == individualPropStart + 5u) ? 1u : 0u;
    }
    if (zr != 1u) {
        return 0u;
    }

    if ((id >= individualPropStart) && (id < individualPropStart + 8u)) {
        if (ObjectInClass(glulx, obj)) {
            return 1u;
        }
    }

    return Function3RaPr(glulx, arguments) ? 1u : 0u;
}

auto Function8CpTab(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto numAttrBytes = glulx.GetAccelerationParam(PARAM_NUM_ATTR_BYTES);

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    if (Function1ZRegion(glulx, { obj }) != 1u) {
        Warning(glulx, "[** Programming error: tried to find the \".\" of (something) **]");
        return 0u;
    }

    const auto otab = glulx.MemoryRead32(obj + 4u * (3u + numAttrBytes / 4u));
    if (!otab) {
        return 0u;
    }

    const auto max = glulx.MemoryRead32(otab);

    return BinarySearch(glulx, {
        .key = id,
        .keySize = 2u,
        .start = otab + 4u,
        .structSize = 10u,
        .numStructs = max,
        .keyOffset = 0u,
        .options = 0u
    });
}

auto Function9RaPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto prop = GetPropertyNew(glulx, obj, id);

    return prop == 0u ? 0u : glulx.MemoryRead32(prop + 4u);
}

auto Function10RlPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto prop = GetPropertyNew(glulx, obj, id);

    return prop == 0u ? 0u : 4u * glulx.MemoryRead16(prop + 2u);
}

auto Function11OcCl(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto classMetaclass = glulx.GetAccelerationParam(PARAM_CLASS_METACLASS);
    const auto objectMetaclass = glulx.GetAccelerationParam(PARAM_OBJECT_METACLASS);
    const auto routineMetaclass = glulx.GetAccelerationParam(PARAM_ROUTINE_METACLASS);
    const auto stringMetaclass = glulx.GetAccelerationParam(PARAM_STRING_METACLASS);

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto cla = GetArgumentIfGiven(arguments, 1);

    const auto zr = Function1ZRegion(glulx, { obj });
    if (zr == 3u) {
        return cla == stringMetaclass ? 1u : 0u;
    }
    if (zr == 2u) {
        return cla == routineMetaclass ? 1u : 0u;
    }
    if (zr != 1u) {
        return 0u;
    }

    if (cla == classMetaclass) {
        if (ObjectInClass(glulx, obj)) { return 1u; }
        if (obj == classMetaclass) { return 1u; }
        if (obj == stringMetaclass) { return 1u; }
        if (obj == routineMetaclass) { return 1u; }
        if (obj == objectMetaclass) { return 1u; }
        return 0u;
    }
    if (cla == objectMetaclass) {
        if (ObjectInClass(glulx, obj)) { return 0u; }
        if (obj == classMetaclass) { return 0u; }
        if (obj == stringMetaclass) { return 0u; }
        if (obj == routineMetaclass) { return 0u; }
        if (obj == objectMetaclass) { return 0u; }
        return 1u;
    }
    if ((cla == stringMetaclass) || (cla == routineMetaclass)) {
        return 0u;
    }

    if (!ObjectInClass(glulx, cla)) {
        Warning(glulx, "[** Programming error: tried to apply 'ofclass' with non-class **]");
        return 0u;
    }

    const auto prop = GetPropertyNew(glulx, obj, 2u);
    if (prop == 0u) {
        return 0u;
    }

    const auto listAddress = glulx.MemoryRead32(prop + 4u);
    if (listAddress == 0u) {
        return 0u;
    }

    const auto listLength = glulx.MemoryRead16(prop + 2u);
    for (auto i = 0u; i < listLength; i++) {
        if (glulx.MemoryRead32(listAddress + 4u * i) == cla) {
            return 1u;
        }
    }
    return 0u;
}

auto Function12RvPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);

    const auto id = GetArgumentIfGiven(arguments, 1);
    const auto cpvStart = glulx.GetAccelerationParam(PARAM_CPV_START);

    const auto address = Function9RaPr(glulx, arguments);
    if (address == 0u) {
        if ((id > 0u) && (id < individualPropStart)) {
            return glulx.MemoryRead32(cpvStart + 4u * id);
        }
        Warning(glulx, "[** Programming error: tried to read (something) **]");
        return 0u;
    }

    return glulx.MemoryRead32(address);
}

auto Function13OpPr(GlulxImpl& glulx, const std::vector<uint32_t>& arguments) -> uint32_t {

    const auto individualPropStart = glulx.GetAccelerationParam(PARAM_INDIVIDUAL_PROP_START);

    const auto obj = GetArgumentIfGiven(arguments, 0);
    const auto id = GetArgumentIfGiven(arguments, 1);

    const auto zr = Function1ZRegion(glulx, { obj });
    if (zr == 3u) {
        // print is INDIVIDUAL_PROP_START + 6
        if (id == individualPropStart + 6u) {return 1u;}
        // print_to_array is INDIVIDUAL_PROP_START + 7
        if (id == individualPropStart + 7u) {return 1u;}
        return 0u;
    }
    if (zr == 2u) {
        // call is INDIVIDUAL_PROP_START + 5
        return (id == individualPropStart + 5u) ? 1u : 0u;
    }
    if (zr != 1u) {
        return 0u;
    }

    if ((id >= individualPropStart) && (id < individualPropStart + 8u)) {
        if (ObjectInClass(glulx, obj)) {
            return 1u;
        }
    }

    return Function9RaPr(glulx, arguments) ? 1u : 0u;
}
}