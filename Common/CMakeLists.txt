add_library(
    common STATIC
    Common/Endian.cpp
)

target_include_directories(common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_compile_options(common PRIVATE -Wall -Wextra -Werror -pedantic)