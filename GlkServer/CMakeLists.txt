add_library(
    GlkServer STATIC
    Impl/Call/GlkCall.cpp
    Impl/Call/Ops/GlkCase.cpp
    Impl/Call/Ops/GlkEvent.cpp
    Impl/Call/Ops/GlkFileref.cpp
    Impl/Call/Ops/GlkHyperlink.cpp
    Impl/Call/Ops/GlkImage.cpp
    Impl/Call/Ops/GlkMiscellaneous.cpp
    Impl/Call/Ops/GlkRead.cpp
    Impl/Call/Ops/GlkSelect.cpp
    Impl/Call/Ops/GlkSoundChannel.cpp
    Impl/Call/Ops/GlkStream.cpp
    Impl/Call/Ops/GlkStyle.cpp
    Impl/Call/Ops/GlkTime.cpp
    Impl/Call/Ops/GlkWindow.cpp
    Impl/Call/Ops/GlkWrite.cpp
    Array/GlkArrayRegistry.cpp
    GlkServer.cpp
    Impl/GlkServerImpl.cpp
    GlkMain.cpp
    Object/GlkObjectRegistry.cpp
    Impl/CallDirect/GlkCallDirect.cpp
)

target_link_libraries(GlkServer PRIVATE cheapglk)

target_include_directories(GlkServer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_compile_options(GlkServer PRIVATE -Wall -Wextra -Werror -pedantic)