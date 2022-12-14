#include "ZorkObjects.h"

#include "Impl/Components/Memory/ZorkMemory.h"
#include "Impl/Header/ZorkHeader.h"
#include "Impl/ZorkError.h"

using namespace fiction::zork;

namespace {
constexpr auto COUNT_OBJECT_DEFAULT_PROPERTIES = 31u;

constexpr auto SIZE_OBJECT_DEFAULT_PROPERTY = 2u;
constexpr auto SIZE_OBJECT_ENTRY = 9u;

constexpr auto OFFSET_OBJECT_ATTRIBUTES = 0u;
constexpr auto OFFSET_OBJECT_PARENT = 4u;
constexpr auto OFFSET_OBJECT_SIBLING = 5u;
constexpr auto OFFSET_OBJECT_CHILD = 6u;
constexpr auto OFFSET_OBJECT_PROPERTIES_POINTER = 7u;
constexpr auto OFFSET_OBJECT_NAME = 1u;

auto GetObjectAttributeByte(uint32_t attribute) -> uint32_t {
    return (attribute >> 3u) & 0x3u;
}

auto GetObjectAttributeBit(uint32_t attribute) -> uint32_t {
    return (attribute ^ 0x7u) & 0x7u;
}
}

ZorkObjects::ZorkObjects(ZorkMemory& memory_in, const ZorkHeader& header_in) :
    memory(memory_in),
    header(header_in) {}

ZorkObjects::~ZorkObjects() = default;

auto ZorkObjects::GetDefaultPropertyAddress(uint32_t property) const -> uint32_t {
    return header.objectTable + (property - 1u) * SIZE_OBJECT_DEFAULT_PROPERTY;
}

auto ZorkObjects::GetObjectEntryAddress(uint32_t object) const -> uint32_t {
    return header.objectTable
           + (COUNT_OBJECT_DEFAULT_PROPERTIES * SIZE_OBJECT_DEFAULT_PROPERTY)
           + SIZE_OBJECT_ENTRY * (object - 1u);
}

auto ZorkObjects::GetObjectAttributeAddress(uint32_t object, uint32_t attribute) const -> uint32_t {
    const auto objectEntryAddress = GetObjectEntryAddress(object);
    const auto attributeByte = GetObjectAttributeByte(attribute);
    return objectEntryAddress + OFFSET_OBJECT_ATTRIBUTES + attributeByte;
}

auto ZorkObjects::GetObjectParentAddress(uint32_t object) const -> uint32_t {
    return GetObjectEntryAddress(object) + OFFSET_OBJECT_PARENT;
}

auto ZorkObjects::GetObjectSiblingAddress(uint32_t object) const -> uint32_t {
    return GetObjectEntryAddress(object) + OFFSET_OBJECT_SIBLING;
}

auto ZorkObjects::GetObjectChildAddress(uint32_t object) const -> uint32_t {
    return GetObjectEntryAddress(object) + OFFSET_OBJECT_CHILD;
}

auto ZorkObjects::GetObjectPropertyPointerAddress(uint32_t object) const -> uint32_t {
    return GetObjectEntryAddress(object) + OFFSET_OBJECT_PROPERTIES_POINTER;
}

auto ZorkObjects::GetObjectPropertiesHeaderAddress(uint32_t object) const -> uint32_t {
    return memory.Read16(GetObjectPropertyPointerAddress(object));
}

auto ZorkObjects::GetObjectNameAddress(uint32_t object) const -> uint32_t {
    return GetObjectPropertiesHeaderAddress(object) + OFFSET_OBJECT_NAME;
}

auto ZorkObjects::GetObjectPropertiesAddress(uint32_t object) const -> uint32_t {
    const auto objectPropertiesHeaderAddress = GetObjectPropertiesHeaderAddress(object);
    const auto nameLength = 2u * memory.Read8(objectPropertiesHeaderAddress);
    return objectPropertiesHeaderAddress + OFFSET_OBJECT_NAME + nameLength;
}

auto ZorkObjects::ReadObjectAttribute(uint32_t object, uint32_t attribute) const -> uint32_t {
    const auto attributeAddress = GetObjectAttributeAddress(object, attribute);
    const auto bit = GetObjectAttributeBit(attribute);
    const auto attributes = memory.Read8(attributeAddress);
    return (attributes >> bit) & 0x1u;
}

auto ZorkObjects::WriteObjectAttribute(uint32_t object, uint32_t attribute, uint32_t value) -> void {
    const auto attributeAddress = GetObjectAttributeAddress(object, attribute);
    const auto bit = GetObjectAttributeBit(attribute);
    const auto attributes = memory.Read8(attributeAddress);
    const auto updated = (attributes & ~(0x1u << bit)) | ((value & 0x1u) << bit);
    memory.Write8(attributeAddress, updated);
}

auto ZorkObjects::ReadObjectParent(uint32_t object) const -> uint32_t {
    return memory.Read8(GetObjectParentAddress(object));
}

auto ZorkObjects::WriteObjectParent(uint32_t object, uint32_t value) -> void {
    memory.Write8(GetObjectParentAddress(object), value);
}

auto ZorkObjects::ReadObjectSibling(uint32_t object) const -> uint32_t {
    return memory.Read8(GetObjectSiblingAddress(object));
}

auto ZorkObjects::WriteObjectSibling(uint32_t object, uint32_t value) -> void {
    return memory.Write8(GetObjectSiblingAddress(object), value);
}

auto ZorkObjects::ReadObjectChild(uint32_t object) const -> uint32_t {
    return memory.Read8(GetObjectChildAddress(object));
}

auto ZorkObjects::WriteObjectChild(uint32_t object, uint32_t value) -> void {
    return memory.Write8(GetObjectChildAddress(object), value);
}

auto ZorkObjects::ReadObjectPropertyNumberAndSize(uint32_t address) const-> std::tuple<uint32_t, uint32_t> {
    const auto data = memory.Read8(address);
    return { data & 0x1Fu, 1u + (data >> 5u) };
}

auto ZorkObjects::FindObjectPropertySizeAndAddress(
    uint32_t object,
    uint32_t property) const -> std::tuple<uint32_t, uint32_t> {
    auto address = GetObjectPropertiesAddress(object);
    for (auto i = 0u; i < 32u; ++i) {
        const auto& [ number, size ] = ReadObjectPropertyNumberAndSize(address);
        address += 1;
        if (number == 0u) {
            return { 0u, 0u };
        } else if (number == property) {
            return { size, address };
        }
        address += size;
    }
    return { 0u, 0u };
}

auto ZorkObjects::FindObjectNextPropertyNumber(uint32_t object, uint32_t property) const -> uint32_t {
    if (property == 0u) {
        const auto address = GetObjectPropertiesAddress(object);
        const auto& [number, size] = ReadObjectPropertyNumberAndSize(address);
        return number;
    }
    const auto& [size, address] = FindObjectPropertySizeAndAddress(object, property);
    if (address == 0u) {
        ErrorFormat("Property %u not found on object %u", property, object);
    }
    const auto& [number, propSize] = ReadObjectPropertyNumberAndSize(address + size);
    return number;
}

auto ZorkObjects::ReadObjectProperty(uint32_t object, uint32_t property) const -> uint32_t {
    const auto& [size, address] = FindObjectPropertySizeAndAddress(object, property);
    if (address == 0u) {
        return memory.Read16(GetDefaultPropertyAddress(property));
    } else if (size == 1u) {
        return memory.Read8(address);
    }
    return memory.Read16(address);
}

auto ZorkObjects::WriteObjectProperty(uint32_t object, uint32_t property, uint32_t value) -> void {
    const auto& [size, address] = FindObjectPropertySizeAndAddress(object, property);
    if (address == 0u) {
        ErrorFormat("Property %u not found on object %u", property, object);
    } else if (size == 1u) {
        memory.Write8(address, value);
    } else {
        memory.Write16(address, value);
    }
}

auto ZorkObjects::RemoveObject(uint32_t object) -> void {
    const auto parent = ReadObjectParent(object);
    if (parent == 0u) {
        return;
    }
    const auto sibling = ReadObjectSibling(object);
    auto previous = ReadObjectChild(parent);
    if (previous == object) {
        WriteObjectChild(parent, sibling);
    } else {
        while (ReadObjectSibling(previous) != object) {
            previous = ReadObjectSibling(previous);
        }
        WriteObjectSibling(previous, sibling);
    }
    WriteObjectParent(object, 0u);
    WriteObjectSibling(object, 0u);
}

auto ZorkObjects::InsertObject(uint32_t object, uint32_t destination) -> void {
    RemoveObject(object);
    WriteObjectParent(object, destination);
    WriteObjectSibling(object, ReadObjectChild(destination));
    WriteObjectChild(destination, object);
}