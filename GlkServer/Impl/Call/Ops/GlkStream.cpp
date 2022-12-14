#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallStreamIterate(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, rockRef] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    auto rock { 0u };
    const auto next = glk_stream_iterate(stream, &rock);
    WriteReference(rockRef, rock);
    return GetObjectId(next);
}

auto GlkServerImpl::CallStreamGetRock(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId] = TakeFirst<1>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    return glk_stream_get_rock(stream);
}

auto GlkServerImpl::CallStreamOpenFile(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId, fmode, rock] = TakeFirst<3>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    const auto stream = glk_stream_open_file(fileRef, fmode, rock);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallStreamOpenFileUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [fileRefId, fmode, rock] = TakeFirst<3>(arguments);
    auto fileRef = GetObject<frefid_t>(fileRefId);
    const auto stream = glk_stream_open_file_uni(fileRef, fmode, rock);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallStreamOpenMemory(const std::vector<uint32_t>& arguments) -> uint32_t {
    // Note: the specific lambda captures are to deal with the fact Apple clang version 14.0.0
    // does not support capturing structured bindings, contrary to the C++20 standard
    const auto& [address, maxlen, fmode, rock] = TakeFirst<4>(arguments);
    auto buffer = CreateArray8(maxlen, [=, maxlen=maxlen, address=address](char* array) {
        WriteArray8(array, maxlen, address);
    });
    auto stream = glk_stream_open_memory(buffer, maxlen, fmode, rock);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallStreamOpenMemoryUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    // Note: the specific lambda captures are to deal with the fact Apple clang version 14.0.0
    // does not support capturing structured bindings, contrary to the C++20 standard
    const auto& [address, maxlen, fmode, rock] = TakeFirst<4>(arguments);
    auto buffer = CreateArray32(maxlen, [=, maxlen=maxlen, address=address](uint32_t* array) {
        WriteArray32(array, maxlen, address);
    });
    auto stream = glk_stream_open_memory_uni(buffer, maxlen, fmode, rock);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallStreamClose(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, resultRef] = TakeFirst<2>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    stream_result_t result {};
    glk_stream_close(stream, &result);
    WriteStruct(resultRef, result);
    return 0u;
}

auto GlkServerImpl::CallStreamSetPosition(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId, pos, seekMode] = TakeFirst<3>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_stream_set_position(stream, static_cast<int32_t>(pos), seekMode);
    return 0u;
}

auto GlkServerImpl::CallStreamGetPosition(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId] = TakeFirst<1>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    return glk_stream_get_position(stream);
}

auto GlkServerImpl::CallStreamSetCurrent(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [streamId] = TakeFirst<1>(arguments);
    auto stream = GetObject<strid_t>(streamId);
    glk_stream_set_current(stream);
    return 0u;
}

auto GlkServerImpl::CallStreamGetCurrent(const std::vector<uint32_t>&) -> uint32_t {
    return GetObjectId(glk_stream_get_current());
}

auto GlkServerImpl::CallStreamOpenResource(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [filenum, rock] = TakeFirst<2>(arguments);
    auto stream = glk_stream_open_resource(filenum, rock);
    return GetObjectId(stream);
}

auto GlkServerImpl::CallStreamOpenResourceUni(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [filenum, rock] = TakeFirst<2>(arguments);
    auto stream = glk_stream_open_resource_uni(filenum, rock);
    return GetObjectId(stream);
}

}