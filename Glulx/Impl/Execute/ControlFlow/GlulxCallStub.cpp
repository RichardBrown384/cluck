#include "Impl/GlulxImpl.h"
#include "Impl/GlulxError.h"

#include "GlulxCallStub.h"

#include "Impl/Execute/GlulxExecuteConstants.h"
#include "Impl/Execute/Stream/GlulxStream.h"

namespace fiction::glulx {

auto PushCallStub(GlulxImpl& glulx, uint32_t destinationType, uint32_t destinationAddress) -> void {
    glulx.StackPush32(destinationType);
    glulx.StackPush32(destinationAddress);
    glulx.StackPush32(glulx.GetProgramCounter());
    glulx.StackPush32(glulx.GetFramePointer());
}

auto PopCallStub(GlulxImpl& glulx, uint32_t returnValue) -> void {
    glulx.SetFramePointer(glulx.StackPop32());
    glulx.SetProgramCounter(glulx.StackPop32());
    const auto destinationAddress = glulx.StackPop32();
    const auto destinationType = glulx.StackPop32();
    const auto programCounter = glulx.GetProgramCounter();
    switch (destinationType) {
        case DESTINATION_NONE: return;
        case DESTINATION_MEMORY:
            return glulx.MemoryWrite32(destinationAddress, returnValue);
        case DESTINATION_LOCAL:
            return glulx.StackWriteLocal32(destinationAddress, returnValue);
        case DESTINATION_STACK:
            return glulx.StackPush32(returnValue);
        case DESTINATION_RESUME_PRINTING_COMPRESSED_STRING:
            return StreamCompressedString(glulx, programCounter, destinationAddress);
        case DESTINATION_RESUME_EXECUTING: return;
        case DESTINATION_RESUME_PRINTING_INTEGER:
            return StreamInteger(glulx, programCounter, destinationAddress);
        case DESTINATION_RESUME_PRINTING_C_STRING:
            return StreamCString(glulx, programCounter);
        case DESTINATION_RESUME_PRINTING_UNICODE_STRING:
            return StreamUnicodeString(glulx, programCounter);
        default: Error("Unsupported call stub destination type");
    }
}

}