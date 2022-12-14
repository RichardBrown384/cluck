add_library(
    glulx STATIC
    Glulx.cpp
    Impl/Blorb/GlulxBlorbRead.cpp
    Impl/Components/Acceleration/GlulxAcceleration.cpp
    Impl/Components/Acceleration/GlulxAccelerationFunctions.cpp
    Impl/Components/Gestalt/GlulxGestalt.cpp
    Impl/Components/IO/GlulxFilterInputOutputSystem.cpp
    Impl/Components/IO/GlulxInputOutputSystems.cpp
    Impl/Components/Memory/GlulxMemory.cpp
    Impl/Components/Random/GlulxRandom.cpp
    Impl/Components/Stack/GlulxStack.cpp
    Impl/Components/Undo/GlulxUndo.cpp
    Impl/Execute/ControlFlow/GlulxCallReturn.cpp
    Impl/Execute/ControlFlow/GlulxCallStub.cpp
    Impl/Execute/ControlFlow/GlulxFunction.cpp
    Impl/Execute/GlulxExecute.cpp
    Impl/Execute/Instruction/GlulxInstruction.cpp
    Impl/Execute/Instruction/GlulxOperandLoad.cpp
    Impl/Execute/Instruction/GlulxOperandStore.cpp
    Impl/Execute/Instruction/GlulxOperandWrite.cpp
    Impl/Execute/Ops/GlulxOpsAcceleratedFunctions.cpp
    Impl/Execute/Ops/GlulxOpsArray.cpp
    Impl/Execute/Ops/GlulxOpsBlockCopyAndClear.cpp
    Impl/Execute/Ops/GlulxOpsBranch.cpp
    Impl/Execute/Ops/GlulxOpsBranchDouble.cpp
    Impl/Execute/Ops/GlulxOpsBranchFloat.cpp
    Impl/Execute/Ops/GlulxOpsContinuations.cpp
    Impl/Execute/Ops/GlulxOpsDouble.cpp
    Impl/Execute/Ops/GlulxOpsFloat.cpp
    Impl/Execute/Ops/GlulxOpsFunctions.cpp
    Impl/Execute/Ops/GlulxOpsGameState.cpp
    Impl/Execute/Ops/GlulxOpsInteger.cpp
    Impl/Execute/Ops/GlulxOpsMemoryAllocationHeap.cpp
    Impl/Execute/Ops/GlulxOpsMemoryMap.cpp
    Impl/Execute/Ops/GlulxOpsMiscellaneous.cpp
    Impl/Execute/Ops/GlulxOpsMove.cpp
    Impl/Execute/Ops/GlulxOpsOutput.cpp
    Impl/Execute/Ops/GlulxOpsRandomNumberGenerator.cpp
    Impl/Execute/Ops/GlulxOpsSearch.cpp
    Impl/Execute/Ops/GlulxOpsStack.cpp
    Impl/Execute/Search/GlulxSearch.cpp
    Impl/Execute/Stream/GlulxStream.cpp
    Impl/GlulxError.cpp
    Impl/GlulxImpl.cpp
    Impl/Header/GlulxHeader.cpp
    Impl/Serial/GlulxIff.cpp
    Impl/Serial/GlulxReadBuffer.cpp
    Impl/Serial/GlulxCompression.cpp
    Impl/Serial/GlulxSerialRead.cpp
    Impl/Serial/GlulxSerialWrite.cpp
    Impl/Serial/GlulxWriteBuffer.cpp
)

target_include_directories(glulx
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(glulx PRIVATE common)

target_compile_options(glulx PRIVATE -Wall -Wextra -Werror -pedantic)