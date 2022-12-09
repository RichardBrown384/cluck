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

[Blorbs][blorb] are supported to the extent that [Cheap Glk][cheap-glk] supports them.

## Building with CMake

### Generating a solution

```bash
cmake -G <preferred generator> /path/to/src
```

### Building in the build directory

```bash
cmake --build .
```

## Why Cluck?

One has no idea how one would pronounce Glulx.

## Licence and copyright

Cluck is licensed under MIT. Copyright 2022 Richard Brown.

***
[cmake]: https://cmake.org/
[glulx]: https://eblong.com/zarf/glulx/
[glk]: https://eblong.com/zarf/glk/
[blorb]: https://eblong.com/zarf/blorb/
[cheap-glk]: https://github.com/erkyrath/cheapglk
[glulxe]: https://github.com/erkyrath/glulxe
