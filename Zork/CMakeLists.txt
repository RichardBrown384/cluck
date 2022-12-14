add_library(
    zork STATIC
    Zork.cpp
    Impl/Execute/ZorkExecute.cpp
    Impl/Components/Dictionary/ZorkDictionary.cpp
    Impl/Components/Memory/ZorkMemory.cpp
    Impl/Components/Objects/ZorkObjects.cpp
    Impl/Components/Random/ZorkRandom.cpp
    Impl/Components/Stack/ZorkStack.cpp
    Impl/Components/Tokenizer/ZorkTokenizer.cpp
    Impl/Header/ZorkHeader.cpp
    Impl/ZorkCharacterStream.cpp
    Impl/ZorkImpl.cpp
    Impl/ZorkError.cpp
    Impl/ZorkImplBuffers.cpp
    Impl/ZorkImplControlFlow.cpp
    Impl/ZorkImplInstruction.cpp
    Impl/ZorkImplPrint.cpp
)

target_include_directories(zork
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(zork PRIVATE common)

target_compile_options(zork PRIVATE -Wall -Wextra -Werror -pedantic)