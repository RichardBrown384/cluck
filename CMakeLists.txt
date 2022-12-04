cmake_minimum_required(VERSION 3.21)
project(Cluck)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(CheapGlk
    GIT_REPOSITORY   https://github.com/RichardBrown384/cheapglk.git
    GIT_TAG          master
    )

FetchContent_MakeAvailable(CheapGlk)

add_subdirectory(Frontend)
add_subdirectory(GlkServer)
add_subdirectory(Glulx)
