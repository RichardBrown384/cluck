#pragma once

#include <cstdint>
#include <tuple>

namespace fiction::zork {

struct ZorkHeader;
class ZorkMemory;

class ZorkObjects {
public:
    ZorkObjects(ZorkMemory&, const ZorkHeader&);
    ZorkObjects(const ZorkObjects&) = delete;
    ZorkObjects(ZorkObjects&&) = delete;
    ZorkObjects& operator=(const ZorkObjects&) = delete;
    ZorkObjects& operator=(ZorkObjects&&) = delete;
    ~ZorkObjects();
private:
    [[nodiscard]] auto GetDefaultPropertyAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectEntryAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectAttributeAddress(uint32_t, uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectParentAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectSiblingAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectChildAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectPropertyPointerAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectPropertiesHeaderAddress(uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetObjectPropertiesAddress(uint32_t) const -> uint32_t;
public:
    [[nodiscard]] auto GetObjectNameAddress(uint32_t) const -> uint32_t;

    [[nodiscard]] auto ReadObjectAttribute(uint32_t, uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectParent(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectSibling(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectChild(uint32_t) const -> uint32_t;
    [[nodiscard]] auto ReadObjectPropertyNumberAndSize(uint32_t) const -> std::tuple<uint32_t, uint32_t>;
    [[nodiscard]] auto ReadObjectProperty(uint32_t, uint32_t) const -> uint32_t;

    [[nodiscard]] auto FindObjectPropertySizeAndAddress(uint32_t, uint32_t) const -> std::tuple<uint32_t, uint32_t>;
    [[nodiscard]] auto FindObjectNextPropertyNumber(uint32_t, uint32_t) const -> uint32_t;

    auto WriteObjectParent(uint32_t, uint32_t) -> void;
    auto WriteObjectAttribute(uint32_t, uint32_t, uint32_t) -> void;
    auto WriteObjectSibling(uint32_t, uint32_t) -> void;
    auto WriteObjectChild(uint32_t, uint32_t) -> void;
    auto WriteObjectProperty(uint32_t, uint32_t, uint32_t) -> void;

    auto RemoveObject(uint32_t) -> void;
    auto InsertObject(uint32_t, uint32_t) -> void;
private:
    ZorkMemory& memory;
    const ZorkHeader& header;
};

}