#include "Impl/GlkServerImpl.h"
#include "Common/TakeFirst.h"

namespace fiction::glk {

auto GlkServerImpl::CallCurrentTime(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [resultRef] = TakeFirst<1>(arguments);
    glktimeval_t time {};
    glk_current_time(&time);
    WriteStruct(resultRef, time);
    return 0u;
}

auto GlkServerImpl::CallCurrentSimpleTime(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [factor] = TakeFirst<1>(arguments);
    return glk_current_simple_time(factor);
}

auto GlkServerImpl::CallTimeToDateUtc(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [timeRef, dateRef] = TakeFirst<2>(arguments);
    glktimeval_t time {};
    glkdate_t date {};
    ReadStruct(timeRef, time);
    glk_time_to_date_utc(&time, & date);
    WriteStruct(dateRef, date);
    return 0u;
}

auto GlkServerImpl::CallTimeToDateLocal(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [timeRef, dateRef] = TakeFirst<2>(arguments);
    glktimeval_t time {};
    glkdate_t date {};
    ReadStruct(timeRef, time);
    glk_time_to_date_local(&time, &date);
    WriteStruct(dateRef, date);
    return 0u;
}

auto GlkServerImpl::CallSimpleTimeToDateUtc(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [time, factor, dateRef] = TakeFirst<3>(arguments);
    glkdate_t date {};
    glk_simple_time_to_date_utc(static_cast<int32_t>(time), factor, &date);
    WriteStruct(dateRef, date);
    return 0u;
}

auto GlkServerImpl::CallSimpleTimeToDateLocal(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [time, factor, dateRef] = TakeFirst<3>(arguments);
    glkdate_t date {};
    glk_simple_time_to_date_local(static_cast<int32_t>(time), factor, &date);
    WriteStruct(dateRef, date);
    return 0u;
}

auto GlkServerImpl::CallDateToTimeUtc(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [dateRef, timeRef] = TakeFirst<2>(arguments);
    glkdate_t date {};
    glktimeval_t time {};
    ReadStruct(dateRef, date);
    glk_date_to_time_utc(&date, &time);
    WriteStruct(timeRef, time);
    return 0u;
}

auto GlkServerImpl::CallDateToTimeLocal(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [dateRef, timeRef] = TakeFirst<2>(arguments);
    glkdate_t date {};
    glktimeval_t time {};
    ReadStruct(dateRef, date);
    glk_date_to_time_local(&date, &time);
    WriteStruct(timeRef, time);
    return 0u;
}

auto GlkServerImpl::CallDateToSimpleTimeUtc(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [dateRef, factor] = TakeFirst<2>(arguments);
    glkdate_t date {};
    ReadStruct(dateRef, date);
    return glk_date_to_simple_time_utc(&date, factor);
}

auto GlkServerImpl::CallDateToSimpleTimeLocal(const std::vector<uint32_t>& arguments) -> uint32_t {
    const auto& [dateRef, factor] = TakeFirst<2>(arguments);
    glkdate_t date {};
    ReadStruct(dateRef, date);
    return glk_date_to_simple_time_local(&date, factor);
}

}