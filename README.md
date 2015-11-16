# C++ Binary Fixed-Point Arithmetic

[![Build Status](https://travis-ci.org/johnmcfarlane/fixed_point.svg)](https://travis-ci.org/johnmcfarlane/fixed_point)
[![Build status](https://ci.appveyor.com/api/projects/status/u8gvlnbi8puo2eee?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/fixed-point)

## Introduction

The [fixed_point](https://github.com/johnmcfarlane/fixed_point)
library is designed to represent binary fixed-point real numbers using
built-in integer types. It is developed as part of
[SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and
SG6.
For an overview of the API, see draft proposal,
[LEWG, EWG, SG14, SG6: P0037](http://johnmcfarlane.github.io/fixed_point/docs/papers/p0037.html).

## Instructions

### Download

The library is hosted on [GitHub](https://github.com/):

    git clone https://github.com/johnmcfarlane/fixed_point.git --recursive
    cd fixed_point

### Linux

Tested on Debian GNU/Linux 8.2 using GCC 4.9 and Clang 3.5

To build:

    cmake -DCMAKE_BUILD_TYPE=Release
    make

To disable exception handling, add `-DEXCEPTIONS=OFF` to the `cmake` command:

    cmake -DCMAKE_BUILD_TYPE=Release -DEXCEPTIONS=OFF
    make

To run tests:

    ./fp_tests

To run benchmarks:

    ./fp_benchmarks

To profile benchmarks:

1. in *CMakeLists.txt*, append `-fno-omit-frame-pointer` to `COMMON_CXX_FLAGS`

2. then run:
   ```
   perf stat ./run_benchmarks
   perf report -g 'graph,0.5,caller'`
   ```

### Windows

Tested on *Windows 7 Professional* with *CMake 3.4.0*. Requires:

- MSBuild 14.0
- CMake 2.8.4

To build *vs/Release/fp_test.exe* and *vs/Release/fp_benchmark.exe*:

    mkdir vs
    cd vs
    cmake -G "Visual Studio 14 2015" ..
    MSBuild.exe /m fixed_point.sln /p:Configuration=Release

For 64-bit builds, append `Win64` to the `-G` option above:

    cmake -G "Visual Studio 14 2015 Win64" ..

To clean the project files:

    cd vs
    git clean -Xdf .
