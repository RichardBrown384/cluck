# Cluck - A Glulx Interpreter for Interactive Fiction

## Project requirements

This project requires the following:

* A C and C++ compiler with C++20 support (Clang 14 or later).
* [CMake][cmake] 3.21 or later

## About

Cluck is Virtual Machine for Interactive Fiction implementing of the [Glulx][glulx] 3.1.3 standard.

The code uses C++20 features where possible, limited only by the level of support offered
by our compiler. Where possible, workarounds are used and documented.

It currently uses a fork of [Cheap Glk][cheap-glk] as its [Glk][glk] implementation.

It currently doesn't support [Blorbs][blorb] as that requires some further thought on the design.

## Why Cluck? 

One has no idea how one would pronounce Glulx.

### Design decisions

The design was motivated by the principle that the Virtual Machine should know nothing
about Glk and Glk should know nothing about the Virtual Machine.

Out of the box, the Virtual Machine has the two Input/Output systems mandated by the standard
(Null and Filter respectively) and the `glk` opcode reads the arguments and then throws them away.

If you wish to use the Glk with the Virtual Machine then it's up to you to configure
a Glk Input/Output System and instruct the Virtual Machine on what to do with the `glk` opcode.

The example of how this can be done can be found in the `Frontend` project.

Furthermore, it was decided that we should use our own Glk dispatch layer.

The motivation for this is twofold.

Firstly, adapting the exiting dispatch would require making code changes to our dependencies
and that is perhaps less than desirable (the existing fork currently serves only to ease the 
integration with CMake in our build process). 

Secondly, we would perhaps like to provide our own Glk implementation at a future date.

### Implementation Decisions

In comparison to the [reference][glulxe] implementation there are a few differences.

The motivation here was to keep things as simple as possible, potentially at the expense 
of Virtual Machine performance.

They are:

1. No attempt is made to cache the compressed String Table
2. Streaming strings and numbers to the Output System always push Call Stubs
   1. This was for simplicity's sake, the reference implementation tries to only push a Stub when necessary
3. The stack is stored big-endian fashion in memory
   1. This has the benefit that serializing the stack is now trivial
4. The stack rotation is done in place and doesn't require any additional storage
   1. The algorithm used is O(n) in time complexity
5. The decision about how to parse the instruction operands has been moved from run to compile time
6. The Save and Undo systems both use the same data format, there's no attempt made to optimise for storage
7. Our own Glk dispatch layer moves the decision on how to pass arguments from run to compile time
8. While Glk imposes a two-step program initialisation scheme, we hide that behind a facade

### Implementation Non-decisions

1. The Save and Undo systems would benefit from using C++20 ranges to reduce the number of copies
   1. Experiments were made with ranges but the method return types were verbose
   2. This should be revisited at a later date

### Glk Server and Glk Client

The Glk Server serves as the potential interface between a Glulx Virtual Machine and Glk.

The Glk Server needs to be able to perform certain operations on its clients (Push and pop
from the stack, read and write from/to memory) and these operations are encapsulated
in the Glk Client interface.

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

## Building with CMake

### Generating a solution

```bash
cmake -G <preferred generator> /path/to/src
```

### Building in the build directory

```bash
cmake --build .
```

## Licence and copyright

Cluck is licensed under MIT. Copyright 2022 Richard Brown.

***
[cmake]: https://cmake.org/
[glulx]: https://eblong.com/zarf/glulx/
[glk]: https://eblong.com/zarf/glk/
[blorb]: https://eblong.com/zarf/blorb/
[cheap-glk]: https://github.com/erkyrath/cheapglk
[glulxe]: https://github.com/erkyrath/glulxe
