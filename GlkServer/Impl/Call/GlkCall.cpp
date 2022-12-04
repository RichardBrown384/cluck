#include "Impl/GlkServerImpl.h"

namespace {
constexpr auto FUNCTION_EXIT = 0x0001u;
constexpr auto FUNCTION_SET_INTERRUPT_HANDLER = 0x0002u;
constexpr auto FUNCTION_TICK = 0x0003u;
constexpr auto FUNCTION_GESTALT = 0x0004u;
constexpr auto FUNCTION_GESTALT_EXT = 0x0005u;

constexpr auto FUNCTION_WINDOW_ITERATE = 0x0020u;
constexpr auto FUNCTION_WINDOW_GET_ROCK = 0x0021u;
constexpr auto FUNCTION_WINDOW_GET_ROOT = 0x0022u;
constexpr auto FUNCTION_WINDOW_OPEN = 0x0023u;
constexpr auto FUNCTION_WINDOW_CLOSE = 0x0024u;
constexpr auto FUNCTION_WINDOW_GET_SIZE = 0x0025u;
constexpr auto FUNCTION_WINDOW_SET_ARRANGEMENT = 0x0026u;
constexpr auto FUNCTION_WINDOW_GET_ARRANGEMENT = 0x0027u;
constexpr auto FUNCTION_WINDOW_GET_TYPE = 0x0028u;
constexpr auto FUNCTION_WINDOW_GET_PARENT = 0x0029u;
constexpr auto FUNCTION_WINDOW_CLEAR = 0x002Au;
constexpr auto FUNCTION_WINDOW_MOVE_CURSOR = 0x002Bu;
constexpr auto FUNCTION_WINDOW_GET_STREAM = 0x002Cu;
constexpr auto FUNCTION_WINDOW_SET_ECHO_STREAM = 0x002Du;
constexpr auto FUNCTION_WINDOW_GET_ECHO_STREAM = 0x002Eu;
constexpr auto FUNCTION_SET_WINDOW = 0x002Fu;
constexpr auto FUNCTION_WINDOW_GET_SIBLING = 0x0030u;

constexpr auto FUNCTION_STREAM_ITERATE = 0x0040u;
constexpr auto FUNCTION_STREAM_GET_ROCK = 0x0041u;
constexpr auto FUNCTION_STREAM_OPEN_FILE = 0x0042u;
constexpr auto FUNCTION_STREAM_OPEN_MEMORY = 0x0043u;
constexpr auto FUNCTION_STREAM_CLOSE = 0x0044u;
constexpr auto FUNCTION_STREAM_SET_POSITION = 0x0045u;
constexpr auto FUNCTION_STREAM_GET_POSITION = 0x0046u;
constexpr auto FUNCTION_STREAM_SET_CURRENT = 0x0047u;
constexpr auto FUNCTION_STREAM_GET_CURRENT = 0x0048u;
constexpr auto FUNCTION_STREAM_OPEN_RESOURCE = 0x0049u;

constexpr auto FUNCTION_FILEREF_CREATE_TEMP = 0x0060u;
constexpr auto FUNCTION_FILEREF_CREATE_BY_NAME = 0x0061u;
constexpr auto FUNCTION_FILEREF_CREATE_BY_PROMPT = 0x0062u;
constexpr auto FUNCTION_FILEREF_DESTROY = 0x0063u;
constexpr auto FUNCTION_FILEREF_ITERATE = 0x0064u;
constexpr auto FUNCTION_FILEREF_GET_ROCK = 0x0065u;
constexpr auto FUNCTION_FILEREF_DELETE_FILE = 0x0066u;
constexpr auto FUNCTION_FILEREF_DOES_FILE_EXIST = 0x0067u;
constexpr auto FUNCTION_FILEREF_CREATE_FROM_FILEREF = 0x0068u;

constexpr auto FUNCTION_PUT_CHAR = 0x0080u;
constexpr auto FUNCTION_PUT_CHAR_STREAM = 0x0081u;
constexpr auto FUNCTION_PUT_STRING = 0x0082u;
constexpr auto FUNCTION_PUT_STRING_STREAM = 0x0083u;
constexpr auto FUNCTION_PUT_BUFFER = 0x0084u;
constexpr auto FUNCTION_PUT_BUFFER_STREAM = 0x0085u;
constexpr auto FUNCTION_SET_STYLE = 0x0086u;
constexpr auto FUNCTION_SET_STYLE_STREAM = 0x0087u;

constexpr auto FUNCTION_GET_CHAR_STREAM = 0x0090u;
constexpr auto FUNCTION_GET_LINE_STREAM = 0x0091u;
constexpr auto FUNCTION_GET_BUFFER_STREAM = 0x0092u;

constexpr auto FUNCTION_CHAR_TO_LOWER = 0x00A0u;
constexpr auto FUNCTION_CHAR_TO_UPPER = 0x00A1u;

constexpr auto FUNCTION_STYLEHINT_SET = 0x00B0u;
constexpr auto FUNCTION_STYLEHINT_CLEAR = 0x00B1u;
constexpr auto FUNCTION_STYLE_DISTINGUISH = 0x00B2u;
constexpr auto FUNCTION_STYLE_MEASURE = 0x00B3u;

constexpr auto FUNCTION_SELECT = 0x00C0u;
constexpr auto FUNCTION_SELECT_POLL = 0x00C1u;

constexpr auto FUNCTION_REQUEST_LINE_EVENT = 0x00D0u;
constexpr auto FUNCTION_CANCEL_LINE_EVENT = 0x00D1u;
constexpr auto FUNCTION_REQUEST_CHAR_EVENT = 0x00D2u;
constexpr auto FUNCTION_CANCEL_CHAR_EVENT = 0x00D3u;
constexpr auto FUNCTION_REQUEST_MOUSE_EVENT = 0x00D4u;
constexpr auto FUNCTION_CANCEL_MOUSE_EVENT = 0x00D5u;
constexpr auto FUNCTION_REQUEST_TIMER_EVENTS = 0x00D6u;

constexpr auto FUNCTION_IMAGE_GET_INFO = 0x00E0u;
constexpr auto FUNCTION_IMAGE_DRAW = 0x00E1u;
constexpr auto FUNCTION_IMAGE_DRAW_SCALED = 0x00E2u;
constexpr auto FUNCTION_WINDOW_FLOW_BREAK = 0x00E8u;
constexpr auto FUNCTION_WINDOW_ERASE_RECT = 0x00E9u;
constexpr auto FUNCTION_WINDOW_FILL_RECT = 0x00EAu;
constexpr auto FUNCTION_WINDOW_SET_BACKGROUND_COLOR = 0x00EBu;

constexpr auto FUNCTION_SCHANNEL_ITERATE = 0x00F0u;
constexpr auto FUNCTION_SCHANNEL_GET_ROCK = 0x00F1u;
constexpr auto FUNCTION_SCHANNEL_CREATE = 0x00F2u;
constexpr auto FUNCTION_SCHANNEL_DESTROY = 0x00F3u;
constexpr auto FUNCTION_SCHANNEL_CREATE_EXT = 0x00F4u;
constexpr auto FUNCTION_SCHANNEL_PLAY_MULTI = 0x00F7u;
constexpr auto FUNCTION_SCHANNEL_PLAY = 0x00F8u;
constexpr auto FUNCTION_SCHANNEL_PLAY_EXT = 0x00F9u;
constexpr auto FUNCTION_SCHANNEL_STOP = 0x00FAu;
constexpr auto FUNCTION_SCHANNEL_SET_VOLUME = 0x00FBu;
constexpr auto FUNCTION_SOUND_LOAD_HINT = 0x00FCu;
constexpr auto FUNCTION_SCHANNEL_SET_VOLUME_EXT = 0x00FDu;
constexpr auto FUNCTION_SCHANNEL_PAUSE = 0x00FEu;
constexpr auto FUNCTION_SCHANNEL_UNPAUSE = 0x00FFu;

constexpr auto FUNCTION_SET_HYPERLINK = 0x0100u;
constexpr auto FUNCTION_SET_HYPERLINK_STREAM = 0x0101u;
constexpr auto FUNCTION_REQUEST_HYPERLINK_EVENT = 0x0102u;
constexpr auto FUNCTION_CANCEL_HYPERLINK_EVENT = 0x0103u;

constexpr auto FUNCTION_BUFFER_TO_LOWER_CASE_UNI = 0x0120u;
constexpr auto FUNCTION_BUFFER_TO_UPPER_CASE_UNI = 0x0121u;
constexpr auto FUNCTION_BUFFER_TO_TITLE_CASE_UNI = 0x0122u;
constexpr auto FUNCTION_BUFFER_CANON_DECOMPOSE_UNI = 0x0123u;
constexpr auto FUNCTION_BUFFER_CANON_NORMALIZE_UNI = 0x0124u;
constexpr auto FUNCTION_PUT_CHAR_UNI = 0x0128u;
constexpr auto FUNCTION_PUT_STRING_UNI = 0x0129u;
constexpr auto FUNCTION_PUT_BUFFER_UNI = 0x012Au;
constexpr auto FUNCTION_PUT_CHAR_STREAM_UNI = 0x012Bu;
constexpr auto FUNCTION_PUT_STRING_STREAM_UNI = 0x012Cu;
constexpr auto FUNCTION_PUT_BUFFER_STREAM_UNI = 0x012Du;

constexpr auto FUNCTION_GET_CHAR_STREAM_UNI = 0x0130u;
constexpr auto FUNCTION_GET_BUFFER_STREAM_UNI = 0x0131u;
constexpr auto FUNCTION_GET_LINE_STREAM_UNI = 0x0132u;
constexpr auto FUNCTION_STREAM_OPEN_FILE_UNI = 0x0138u;
constexpr auto FUNCTION_STREAM_OPEN_MEMORY_UNI = 0x0139u;
constexpr auto FUNCTION_STREAM_OPEN_RESOURCE_UNI = 0x013Au;

constexpr auto FUNCTION_REQUEST_CHAR_EVENT_UNI = 0x0140u;
constexpr auto FUNCTION_REQUEST_LINE_EVENT_UNI = 0x0141u;

constexpr auto FUNCTION_SET_ECHO_LINE_EVENT = 0x0150u;
constexpr auto FUNCTION_SET_TERMINATORS_LINE_EVENT = 0x0151u;

constexpr auto FUNCTION_CURRENT_TIME = 0x0160u;
constexpr auto FUNCTION_CURRENT_SIMPLE_TIME = 0x0161u;
constexpr auto FUNCTION_TIME_TO_DATE_UTC = 0x0168u;
constexpr auto FUNCTION_TIME_TO_DATE_LOCAL = 0x0169u;
constexpr auto FUNCTION_SIMPLE_TIME_TO_DATE_UTC = 0x016Au;
constexpr auto FUNCTION_SIMPLE_TIME_TO_DATE_LOCAL = 0x016Bu;
constexpr auto FUNCTION_DATE_TO_TIME_UTC = 0x016Cu;
constexpr auto FUNCTION_DATE_TO_TIME_LOCAL = 0x016Du;
constexpr auto FUNCTION_DATE_TO_SIMPLE_TIME_UTC = 0x016Eu;
constexpr auto FUNCTION_DATE_TO_SIMPLE_TIME_LOCAL = 0x016Fu;
}

namespace fiction::glk {

auto GlkServerImpl::Call(uint32_t function, const std::vector<uint32_t>& arguments) -> uint32_t {
    switch (function) {
        case FUNCTION_EXIT: return CallExit(arguments);
        case FUNCTION_SET_INTERRUPT_HANDLER: return CallSetInterruptHandler(arguments);
        case FUNCTION_TICK: return CallTick(arguments);
        case FUNCTION_GESTALT: return CallGestalt(arguments);
        case FUNCTION_GESTALT_EXT: return CallGestaltExt(arguments);

        case FUNCTION_WINDOW_ITERATE: return CallWindowIterate(arguments);
        case FUNCTION_WINDOW_GET_ROCK: return CallWindowGetRock(arguments);
        case FUNCTION_WINDOW_GET_ROOT: return CallWindowGetRoot(arguments);
        case FUNCTION_WINDOW_OPEN: return CallWindowOpen(arguments);
        case FUNCTION_WINDOW_CLOSE: return CallWindowClose(arguments);
        case FUNCTION_WINDOW_GET_SIZE: return CallWindowGetSize(arguments);
        case FUNCTION_WINDOW_SET_ARRANGEMENT: return CallWindowSetArrangement(arguments);
        case FUNCTION_WINDOW_GET_ARRANGEMENT: return CallWindowGetArrangement(arguments);
        case FUNCTION_WINDOW_GET_TYPE: return CallWindowGetType(arguments);
        case FUNCTION_WINDOW_GET_PARENT: return CallWindowGetParent(arguments);
        case FUNCTION_WINDOW_CLEAR: return CallWindowClear(arguments);
        case FUNCTION_WINDOW_MOVE_CURSOR: return CallWindowMoveCursor(arguments);
        case FUNCTION_WINDOW_GET_STREAM: return CallWindowGetStream(arguments);
        case FUNCTION_WINDOW_SET_ECHO_STREAM: return CallWindowSetEchoStream(arguments);
        case FUNCTION_WINDOW_GET_ECHO_STREAM: return CallWindowGetEchoStream(arguments);
        case FUNCTION_SET_WINDOW: return CallSetWindow(arguments);
        case FUNCTION_WINDOW_GET_SIBLING: return CallWindowGetSibling(arguments);

        case FUNCTION_STREAM_ITERATE: return CallStreamIterate(arguments);
        case FUNCTION_STREAM_GET_ROCK: return CallStreamGetRock(arguments);
        case FUNCTION_STREAM_OPEN_FILE: return CallStreamOpenFile(arguments);
        case FUNCTION_STREAM_OPEN_MEMORY: return CallStreamOpenMemory(arguments);
        case FUNCTION_STREAM_CLOSE: return CallStreamClose(arguments);
        case FUNCTION_STREAM_SET_POSITION: return CallStreamSetPosition(arguments);
        case FUNCTION_STREAM_GET_POSITION: return CallStreamGetPosition(arguments);
        case FUNCTION_STREAM_SET_CURRENT: return CallStreamSetCurrent(arguments);
        case FUNCTION_STREAM_GET_CURRENT: return CallStreamGetCurrent(arguments);
        case FUNCTION_STREAM_OPEN_RESOURCE: return CallStreamOpenResource(arguments);

        case FUNCTION_FILEREF_CREATE_TEMP: return CallFilerefCreateTemp(arguments);
        case FUNCTION_FILEREF_CREATE_BY_NAME: return CallFilerefCreateByName(arguments);
        case FUNCTION_FILEREF_CREATE_BY_PROMPT: return CallFilerefCreateByPrompt(arguments);
        case FUNCTION_FILEREF_DESTROY: return CallFilerefDestroy(arguments);
        case FUNCTION_FILEREF_ITERATE: return CallFilerefIterate(arguments);
        case FUNCTION_FILEREF_GET_ROCK: return CallFilerefGetRock(arguments);
        case FUNCTION_FILEREF_DELETE_FILE: return CallFilerefDeleteFile(arguments);
        case FUNCTION_FILEREF_DOES_FILE_EXIST: return CallFilerefDoesFileExist(arguments);
        case FUNCTION_FILEREF_CREATE_FROM_FILEREF: return CallFilerefCreateFromFileref(arguments);

        case FUNCTION_PUT_CHAR: return CallPutChar(arguments);
        case FUNCTION_PUT_CHAR_STREAM: return CallPutCharStream(arguments);
        case FUNCTION_PUT_STRING: return CallPutString(arguments);
        case FUNCTION_PUT_STRING_STREAM: return CallPutStringStream(arguments);
        case FUNCTION_PUT_BUFFER: return CallPutBuffer(arguments);
        case FUNCTION_PUT_BUFFER_STREAM: return CallPutBufferStream(arguments);
        case FUNCTION_SET_STYLE: return CallSetStyle(arguments);
        case FUNCTION_SET_STYLE_STREAM: return CallSetStyleStream(arguments);

        case FUNCTION_GET_CHAR_STREAM: return CallGetCharStream(arguments);
        case FUNCTION_GET_LINE_STREAM: return CallGetLineStream(arguments);
        case FUNCTION_GET_BUFFER_STREAM: return CallGetBufferStream(arguments);

        case FUNCTION_CHAR_TO_LOWER: return CallCharToLower(arguments);
        case FUNCTION_CHAR_TO_UPPER: return CallCharToUpper(arguments);

        case FUNCTION_STYLEHINT_SET: return CallStylehintSet(arguments);
        case FUNCTION_STYLEHINT_CLEAR: return CallStylehintClear(arguments);
        case FUNCTION_STYLE_DISTINGUISH: return CallStyleDistinguish(arguments);
        case FUNCTION_STYLE_MEASURE: return CallStyleMeasure(arguments);

        case FUNCTION_SELECT: return CallSelect(arguments);
        case FUNCTION_SELECT_POLL: return CallSelectPoll(arguments);

        case FUNCTION_REQUEST_LINE_EVENT: return CallRequestLineEvent(arguments);
        case FUNCTION_CANCEL_LINE_EVENT: return CallCancelLineEvent(arguments);
        case FUNCTION_REQUEST_CHAR_EVENT: return CallRequestCharEvent(arguments);
        case FUNCTION_CANCEL_CHAR_EVENT: return CallCancelCharEvent(arguments);
        case FUNCTION_REQUEST_MOUSE_EVENT: return CallRequestMouseEvent(arguments);
        case FUNCTION_CANCEL_MOUSE_EVENT: return CallCancelMouseEvent(arguments);
        case FUNCTION_REQUEST_TIMER_EVENTS: return CallRequestTimerEvents(arguments);

        case FUNCTION_IMAGE_GET_INFO: return CallImageGetInfo(arguments);
        case FUNCTION_IMAGE_DRAW: return CallImageDraw(arguments);
        case FUNCTION_IMAGE_DRAW_SCALED: return CallImageDrawScaled(arguments);
        case FUNCTION_WINDOW_FLOW_BREAK: return CallWindowFlowBreak(arguments);
        case FUNCTION_WINDOW_ERASE_RECT: return CallWindowEraseRect(arguments);
        case FUNCTION_WINDOW_FILL_RECT: return CallWindowFillRect(arguments);
        case FUNCTION_WINDOW_SET_BACKGROUND_COLOR: return CallWindowSetBackgroundColor(arguments);

        case FUNCTION_SCHANNEL_ITERATE: return CallSchannelIterate(arguments);
        case FUNCTION_SCHANNEL_GET_ROCK: return CallSchannelGetRock(arguments);
        case FUNCTION_SCHANNEL_CREATE: return CallSchannelCreate(arguments);
        case FUNCTION_SCHANNEL_DESTROY: return CallSchannelDestroy(arguments);
        case FUNCTION_SCHANNEL_CREATE_EXT: return CallSchannelCreateExt(arguments);
        case FUNCTION_SCHANNEL_PLAY_MULTI: return CallSchannelPlayMulti(arguments);
        case FUNCTION_SCHANNEL_PLAY: return CallSchannelPlay(arguments);
        case FUNCTION_SCHANNEL_PLAY_EXT: return CallSchannelPlayExt(arguments);
        case FUNCTION_SCHANNEL_STOP: return CallSchannelStop(arguments);
        case FUNCTION_SCHANNEL_SET_VOLUME: return CallSchannelSetVolume(arguments);
        case FUNCTION_SOUND_LOAD_HINT: return CallSoundLoadHint(arguments);
        case FUNCTION_SCHANNEL_SET_VOLUME_EXT: return CallSchannelSetVolumeExt(arguments);
        case FUNCTION_SCHANNEL_PAUSE: return CallSchannelPause(arguments);
        case FUNCTION_SCHANNEL_UNPAUSE: return CallSchannelUnpause(arguments);

        case FUNCTION_SET_HYPERLINK: return CallSetHyperlink(arguments);
        case FUNCTION_SET_HYPERLINK_STREAM: return CallSetHyperlinkStream(arguments);
        case FUNCTION_REQUEST_HYPERLINK_EVENT: return CallRequestHyperlinkEvent(arguments);
        case FUNCTION_CANCEL_HYPERLINK_EVENT: return CallCancelHyperlinkEvent(arguments);

        case FUNCTION_BUFFER_TO_LOWER_CASE_UNI: return CallBufferToLowerCaseUni(arguments);
        case FUNCTION_BUFFER_TO_UPPER_CASE_UNI: return CallBufferToUpperCaseUni(arguments);
        case FUNCTION_BUFFER_TO_TITLE_CASE_UNI: return CallBufferToTitleCaseUni(arguments);
        case FUNCTION_BUFFER_CANON_DECOMPOSE_UNI: return CallBufferCanonDecomposeUni(arguments);
        case FUNCTION_BUFFER_CANON_NORMALIZE_UNI: return CallBufferCanonNormalizeUni(arguments);
        case FUNCTION_PUT_CHAR_UNI: return CallPutCharUni(arguments);
        case FUNCTION_PUT_STRING_UNI: return CallPutStringUni(arguments);
        case FUNCTION_PUT_BUFFER_UNI: return CallPutBufferUni(arguments);
        case FUNCTION_PUT_CHAR_STREAM_UNI: return CallPutCharStreamUni(arguments);
        case FUNCTION_PUT_STRING_STREAM_UNI: return CallPutStringStreamUni(arguments);
        case FUNCTION_PUT_BUFFER_STREAM_UNI: return CallPutBufferStreamUni(arguments);

        case FUNCTION_GET_CHAR_STREAM_UNI: return CallGetCharStreamUni(arguments);
        case FUNCTION_GET_BUFFER_STREAM_UNI: return CallGetBufferStreamUni(arguments);
        case FUNCTION_GET_LINE_STREAM_UNI: return CallGetLineStreamUni(arguments);
        case FUNCTION_STREAM_OPEN_FILE_UNI: return CallStreamOpenFileUni(arguments);
        case FUNCTION_STREAM_OPEN_MEMORY_UNI: return CallStreamOpenMemoryUni(arguments);
        case FUNCTION_STREAM_OPEN_RESOURCE_UNI: return CallStreamOpenResourceUni(arguments);

        case FUNCTION_REQUEST_CHAR_EVENT_UNI: return CallRequestCharEventUni(arguments);
        case FUNCTION_REQUEST_LINE_EVENT_UNI: return CallRequestLineEventUni(arguments);

        case FUNCTION_SET_ECHO_LINE_EVENT: return CallSetEchoLineEvent(arguments);
        case FUNCTION_SET_TERMINATORS_LINE_EVENT: return CallSetTerminatorsLineEvent(arguments);

        case FUNCTION_CURRENT_TIME: return CallCurrentTime(arguments);
        case FUNCTION_CURRENT_SIMPLE_TIME: return CallCurrentSimpleTime(arguments);
        case FUNCTION_TIME_TO_DATE_UTC: return CallTimeToDateUtc(arguments);
        case FUNCTION_TIME_TO_DATE_LOCAL: return CallTimeToDateLocal(arguments);
        case FUNCTION_SIMPLE_TIME_TO_DATE_UTC: return CallSimpleTimeToDateUtc(arguments);
        case FUNCTION_SIMPLE_TIME_TO_DATE_LOCAL: return CallSimpleTimeToDateLocal(arguments);
        case FUNCTION_DATE_TO_TIME_UTC: return CallDateToTimeUtc(arguments);
        case FUNCTION_DATE_TO_TIME_LOCAL: return CallDateToTimeLocal(arguments);
        case FUNCTION_DATE_TO_SIMPLE_TIME_UTC: return CallDateToSimpleTimeUtc(arguments);
        case FUNCTION_DATE_TO_SIMPLE_TIME_LOCAL: return CallDateToSimpleTimeLocal(arguments);

        default: return 0u;
    }
}

}