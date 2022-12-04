#include "Impl/GlkServerImpl.h"
#include "Util/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallFilerefCreateTemp(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [usage, rock] = TakeFirst<2>(arguments);
    const auto fileRef = glk_fileref_create_temp(usage, rock);
    return GetObjectId(fileRef);
}

auto GlkServerImpl::CallFilerefCreateByName(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [usage, address, rock] = TakeFirst<3>(arguments);
    auto name = ReadString8(address);
    const auto fileRef = glk_fileref_create_by_name(usage, name.data(), rock);
    return GetObjectId(fileRef);
}

auto GlkServerImpl::CallFilerefCreateByPrompt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [usage, fmode, rock] = TakeFirst<3>(arguments);
    const auto fileRef = glk_fileref_create_by_prompt(usage, fmode, rock);
    return GetObjectId(fileRef);
}

auto GlkServerImpl::CallFilerefDestroy(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId] = TakeFirst<1>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    glk_fileref_destroy(fileRef);
    return 0u;
}

auto GlkServerImpl::CallFilerefIterate(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId, rockRef] = TakeFirst<2>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    auto rock { 0u };
    const auto next = glk_fileref_iterate(fileRef, &rock);
    WriteReference(rockRef, rock);
    return GetObjectId(next);
}

auto GlkServerImpl::CallFilerefGetRock(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId] = TakeFirst<1>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    return glk_fileref_get_rock(fileRef);
}

auto GlkServerImpl::CallFilerefDeleteFile(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId] = TakeFirst<1>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    glk_fileref_delete_file(fileRef);
    return 0u;
}

auto GlkServerImpl::CallFilerefDoesFileExist(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId] = TakeFirst<1>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    return glk_fileref_does_file_exist(fileRef);
}

auto GlkServerImpl::CallFilerefCreateFromFileref(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [usage, otherFileRefId, rock] = TakeFirst<3>(arguments);
    auto otherRef = GetObject<frefid_t>(otherFileRefId);
    auto fileRef = glk_fileref_create_from_fileref(usage, otherRef, rock);
    return GetObjectId(fileRef);
}

}