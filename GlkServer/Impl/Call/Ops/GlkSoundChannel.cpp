#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallSchannelIterate(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId, rockRef] = TakeFirst<2>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    auto rock { 0u };
    const auto next = glk_schannel_iterate(schannel, &rock);
    WriteReference(rockRef, rock);
    return GetObjectId(next);
}

auto GlkServerImpl::CallSchannelGetRock(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId] = TakeFirst<1>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    return glk_schannel_get_rock(schannel);
}

auto GlkServerImpl::CallSchannelCreate(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [rock] = TakeFirst<1>(arguments);
    auto schannel = glk_schannel_create(rock);
    return GetObjectId(schannel);
}

auto GlkServerImpl::CallSchannelDestroy(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId] = TakeFirst<1>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_destroy(schannel);
    return 0u;
}

auto GlkServerImpl::CallSchannelCreateExt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [rock, volume] = TakeFirst<2>(arguments);
    auto schannel = glk_schannel_create_ext(rock, volume);
    return GetObjectId(schannel);
}

auto GlkServerImpl::CallSchannelPlayMulti(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [
        schannelAddress, schannelCount, soundAddress, soundCount, notify
    ] = TakeFirst<5>(arguments);
    auto schannelIds = ReadArray32(schannelAddress, schannelCount);
    std::vector<schanid_t> schannels(schannelCount);
    for (const auto& id : schannelIds) {
        schannels.push_back(GetObject<schanid_t>(id));
    }
    auto sounds = ReadArray32(soundAddress, soundCount);
    return glk_schannel_play_multi(schannels.data(), schannelCount, sounds.data(), soundCount, notify);
}

auto GlkServerImpl::CallSchannelPlay(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId, sound] = TakeFirst<2>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    return glk_schannel_play(schannel, sound);
}

auto GlkServerImpl::CallSchannelPlayExt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId, sound, repeats, notify] = TakeFirst<4>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    return glk_schannel_play_ext(schannel, sound, repeats, notify);
}

auto GlkServerImpl::CallSchannelStop(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId] = TakeFirst<1>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_stop(schannel);
    return 0u;
}

auto GlkServerImpl::CallSchannelSetVolume(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId, volume] = TakeFirst<2>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_set_volume(schannel, volume);
    return 0u;
}

auto GlkServerImpl::CallSoundLoadHint(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [sound, flag] = TakeFirst<2>(arguments);
    glk_sound_load_hint(sound, flag);
    return 0u;
}

auto GlkServerImpl::CallSchannelSetVolumeExt(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId, volume, duration, notify] = TakeFirst<4>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_set_volume_ext(schannel, volume, duration, notify);
    return 0u;
}

auto GlkServerImpl::CallSchannelPause(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId] = TakeFirst<1>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_pause(schannel);
    return 0u;
}

auto GlkServerImpl::CallSchannelUnpause(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [schannelId] = TakeFirst<1>(arguments);
    auto schannel = GetObject<schanid_t>(schannelId);
    glk_schannel_unpause(schannel);
    return 0u;
}

}