#pragma once

#include <cstdint>
#include <vector>
#include <memory>

extern "C" {
#include "glk.h"
}

namespace fiction::glk {

class GlkObjectRegistry;
class GlkArrayRegistry;
class GlkClient;
using GlkUniqueStream = std::unique_ptr<glk_stream_struct, void(*)(glk_stream_struct*)>;

class GlkServerImpl {
public:
    GlkServerImpl(
        GlkObjectRegistry&,
        GlkArrayRegistry&,
        GlkClient&,
        const std::vector<uint8_t>&);

    auto Tick() -> void;
    auto PutChar8(uint32_t) -> void;
    auto PutChar32(uint32_t) -> void;
    auto PutBuffer8(uint32_t, const std::vector<uint8_t>&) -> void;
    auto GetBuffer8(uint32_t) -> std::vector<uint8_t>;

    auto Call(uint32_t, const std::vector<uint32_t>&) -> uint32_t;

private:
    GlkObjectRegistry& objects;
    GlkArrayRegistry& arrays;
    GlkClient& client;
    GlkUniqueStream blorb;

private:
    template<class T>
    auto GetObject(uint32_t) -> T;
    auto GetObject(uint32_t) -> void*;
    auto GetObjectId(void*) -> uint32_t;

    auto CreateArray8(uint32_t, const std::function<void(char*)>&) -> char*;
    auto CreateArray32(uint32_t, const std::function<void(uint32_t*)>&) -> uint32_t*;

    auto ReadArray8(uint32_t, uint32_t) -> std::vector<char>;
    auto ReadArray32(uint32_t, uint32_t) -> std::vector<uint32_t>;

    auto ReadString8(uint32_t) -> std::vector<char>;
    auto ReadString32(uint32_t) -> std::vector<uint32_t>;

    auto ReadStructField(uint32_t, uint32_t) -> int32_t;
    auto ReadStruct(uint32_t, glktimeval_t&) -> void;
    auto ReadStruct(uint32_t reference, glkdate_t& result) -> void;

    auto WriteReference(uint32_t, uint32_t) -> void;

    auto WriteArray8(char*, uint32_t, uint32_t) -> void;
    auto WriteArray32(uint32_t*, uint32_t, uint32_t) -> void;

    auto WriteStructField(uint32_t, uint32_t, uint32_t) -> void;
    auto WriteStruct(uint32_t, event_t&) -> void;
    auto WriteStruct(uint32_t, const stream_result_t&) -> void;
    auto WriteStruct(uint32_t, const glktimeval_t&) -> void;
    auto WriteStruct(uint32_t, const glkdate_t&) -> void;

    auto CallExit(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetInterruptHandler(const std::vector<uint32_t>&) -> uint32_t;
    auto CallTick(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGestalt(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGestaltExt(const std::vector<uint32_t>&) -> uint32_t;

    auto CallWindowIterate(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetRock(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetRoot(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowOpen(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowClose(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetSize(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowSetArrangement(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetArrangement(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetType(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetParent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowClear(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowMoveCursor(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowSetEchoStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetEchoStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetWindow(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowGetSibling(const std::vector<uint32_t>&) -> uint32_t;

    auto CallStreamIterate(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamGetRock(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenFile(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenMemory(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamClose(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamSetPosition(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamGetPosition(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamSetCurrent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamGetCurrent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenResource(const std::vector<uint32_t>&) -> uint32_t;

    auto CallFilerefCreateTemp(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefCreateByName(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefCreateByPrompt(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefDestroy(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefIterate(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefGetRock(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefDeleteFile(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefDoesFileExist(const std::vector<uint32_t>&) -> uint32_t;
    auto CallFilerefCreateFromFileref(const std::vector<uint32_t>&) -> uint32_t;

    auto CallPutChar(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutCharStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutString(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutStringStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutBuffer(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutBufferStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetStyle(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetStyleStream(const std::vector<uint32_t>&) -> uint32_t;

    auto CallGetCharStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGetLineStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGetBufferStream(const std::vector<uint32_t>&) -> uint32_t;

    auto CallCharToLower(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCharToUpper(const std::vector<uint32_t>&) -> uint32_t;

    auto CallStylehintSet(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStylehintClear(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStyleDistinguish(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStyleMeasure(const std::vector<uint32_t>&) -> uint32_t;

    auto CallSelect(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSelectPoll(const std::vector<uint32_t>&) -> uint32_t;

    auto CallRequestLineEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCancelLineEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallRequestCharEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCancelCharEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallRequestMouseEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCancelMouseEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallRequestTimerEvents(const std::vector<uint32_t>&) -> uint32_t;

    auto CallImageGetInfo(const std::vector<uint32_t>&) -> uint32_t;
    auto CallImageDraw(const std::vector<uint32_t>&) -> uint32_t;
    auto CallImageDrawScaled(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowFlowBreak(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowEraseRect(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowFillRect(const std::vector<uint32_t>&) -> uint32_t;
    auto CallWindowSetBackgroundColor(const std::vector<uint32_t>&) -> uint32_t;

    auto CallSchannelIterate(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelGetRock(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelCreate(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelDestroy(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelCreateExt(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelPlayMulti(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelPlay(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelPlayExt(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelStop(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelSetVolume(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSoundLoadHint(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelSetVolumeExt(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelPause(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSchannelUnpause(const std::vector<uint32_t>&) -> uint32_t;

    auto CallSetHyperlink(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetHyperlinkStream(const std::vector<uint32_t>&) -> uint32_t;
    auto CallRequestHyperlinkEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCancelHyperlinkEvent(const std::vector<uint32_t>&) -> uint32_t;

    auto CallBufferToLowerCaseUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallBufferToUpperCaseUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallBufferToTitleCaseUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallBufferCanonDecomposeUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallBufferCanonNormalizeUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutCharUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutStringUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutBufferUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutCharStreamUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutStringStreamUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallPutBufferStreamUni(const std::vector<uint32_t>&) -> uint32_t;

    auto CallGetCharStreamUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGetBufferStreamUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallGetLineStreamUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenFileUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenMemoryUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallStreamOpenResourceUni(const std::vector<uint32_t>&) -> uint32_t;

    auto CallRequestCharEventUni(const std::vector<uint32_t>&) -> uint32_t;
    auto CallRequestLineEventUni(const std::vector<uint32_t>&) -> uint32_t;

    auto CallSetEchoLineEvent(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSetTerminatorsLineEvent(const std::vector<uint32_t>&) -> uint32_t;

    auto CallCurrentTime(const std::vector<uint32_t>&) -> uint32_t;
    auto CallCurrentSimpleTime(const std::vector<uint32_t>&) -> uint32_t;
    auto CallTimeToDateUtc(const std::vector<uint32_t>&) -> uint32_t;
    auto CallTimeToDateLocal(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSimpleTimeToDateUtc(const std::vector<uint32_t>&) -> uint32_t;
    auto CallSimpleTimeToDateLocal(const std::vector<uint32_t>&) -> uint32_t;
    auto CallDateToTimeUtc(const std::vector<uint32_t>&) -> uint32_t;
    auto CallDateToTimeLocal(const std::vector<uint32_t>&) -> uint32_t;
    auto CallDateToSimpleTimeUtc(const std::vector<uint32_t>&) -> uint32_t;
    auto CallDateToSimpleTimeLocal(const std::vector<uint32_t>&) -> uint32_t;
};

template<class T>
auto GlkServerImpl::GetObject(uint32_t objectId) -> T {
    return static_cast<T>(GetObject(objectId));
}

}