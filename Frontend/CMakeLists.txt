add_executable(
    frontend
    FileSystem.cpp
    GlkGlulxClient.cpp
    GlkInputOutputSystem.cpp
    main.cpp
)

target_link_libraries(frontend PRIVATE GlkServer glulx)

target_compile_options(frontend PRIVATE -Wall -Wextra -Werror -pedantic)