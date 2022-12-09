# Cluck - Design overview

## Design decisions

The design was motivated by the principle that the Virtual Machine should know nothing about Glk and Glk should know
nothing about the Virtual Machine.

Out of the box, the Virtual Machine has the two Input/Output systems mandated by the standard
(Null and Filter respectively) and the `glk` opcode reads the arguments and then throws them away.

If you wish to use the Glk with the Virtual Machine then it's up to you to configure a Glk Input/Output System and
instruct the Virtual Machine on what to do with the `glk` opcode.

The example of how this can be done can be found in the `Frontend` project.

Furthermore, it was decided that we should use our own Glk dispatch layer.

The motivation for this is twofold.

Firstly, adapting the exiting dispatch for this Virtual Machine implementation would require
making code changes to our dependencies and that is perhaps less than desirable
(the existing fork currently serves only to ease the integration with CMake in our build process).

Secondly, we would perhaps like to provide our own Glk implementation at a future date.

## Implementation Decisions

In comparison to the [reference][glulxe] implementation there are a few differences.

The motivation here was to keep things as simple as possible, potentially at the expense of Virtual Machine performance.

They are:

1. No attempt is made to cache the compressed String Table
2. Streaming strings and numbers to the Output System always push Call Stubs
    1. This was for simplicity's sake, the reference implementation tries to only push a Stub when necessary
3. The stack is stored big-endian fashion in memory
    1. This has the benefit that serializing the stack is now trivial
4. The stack rotation is done in place and doesn't require any additional storage
    1. The algorithm used is O(n) in time complexity
5. The decision about how to parse the instruction operands has been moved from run to compile time
    1. This is more about comprehensibility than performance
6. The Save and Undo systems both use the same data format, there's no attempt made to optimise for storage
7. Our own Glk dispatch layer moves the decision on how to pass arguments from run to compile time
    1. This is more about comprehensibility than performance
8. While Glk imposes a two-step program initialisation scheme, we hide that behind a facade

## Implementation Non-decisions

1. The Save and Undo systems would benefit from using C++20 ranges or spans to reduce the number of copies

## Glk Server and Glk Client

The Glk Server serves as the potential interface between a Glulx Virtual Machine and Glk.

The Glk Server needs to be able to perform certain operations on its clients (Push and pop from the stack, read and
write from/to memory) and these operations are encapsulated in the Glk Client interface.

The Glk Server also provides the following methods to allow direct interaction with the Glk

```c++
auto Tick() -> void;
auto PutChar8(uint32_t) -> void;
auto PutChar32(uint32_t) -> void;
auto PutBuffer8(uint32_t, const std::vector<uint8_t>&) -> void;
auto GetBuffer8(uint32_t) -> std::vector<uint8_t>;
```

The `Tick` method allows the application to call `glk_tick` as required by the specification.

The `PutChar` methods allow the application to implement a Glk Output Stream.

The `PutBuffer8` and `GetBuffer8` allow the application to implement saving and restoring of game state.

***
[glulxe]: https://github.com/erkyrath/glulxe
