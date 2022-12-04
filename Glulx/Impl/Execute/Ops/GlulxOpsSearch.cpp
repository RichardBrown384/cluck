#include "Impl/GlulxImpl.h"
#include "Impl/Execute/Instruction/GlulxInstruction.h"
#include "Impl/Execute/Search/GlulxSearch.h"

namespace fiction::glulx {

auto ExecuteLinearsearch(GlulxImpl& glulx) -> void {
    const auto& [
        key, keySize, start, structSize, numStructs, keyOffset, options, result
    ] = OperandsReadLLLLLLLS(glulx);
    const auto searchResult = LinearSearch(glulx, {
        .key = key.value,
        .keySize = keySize.value,
        .start = start.value,
        .structSize = structSize.value,
        .numStructs = numStructs.value,
        .keyOffset = keyOffset.value,
        .options = options.value
    });
    StoreOperandWrite32(glulx, result, searchResult);
}

auto ExecuteBinarysearch(GlulxImpl& glulx) -> void {
    const auto& [
        key, keySize, start, structSize, numStructs, keyOffset, options, result
    ] = OperandsReadLLLLLLLS(glulx);
    const auto searchResult = BinarySearch(glulx, {
        .key = key.value,
        .keySize = keySize.value,
        .start = start.value,
        .structSize = structSize.value,
        .numStructs = numStructs.value,
        .keyOffset = keyOffset.value,
        .options = options.value
    });
    StoreOperandWrite32(glulx, result, searchResult);
}

auto ExecuteLinkedsearch(GlulxImpl& glulx) -> void {
    const auto& [
        key, keySize, start, keyOffset, nextOffset, options, result
    ] = OperandsReadLLLLLLS(glulx);
    const auto searchResult = LinkedSearch(glulx, {
        .key = key.value,
        .keySize = keySize.value,
        .start = start.value,
        .keyOffset = keyOffset.value,
        .nextOffset = nextOffset.value,
        .options = options.value
    });
    StoreOperandWrite32(glulx, result, searchResult);
}

}