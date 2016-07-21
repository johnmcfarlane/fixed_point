
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sample_functions.h"

#include <sg14/bits/int128.h>

#include <benchmark/benchmark.h>

#define ESCAPE(X) escape_cppcon2015(&X)
//#define ESCAPE(X) escape_codedive2015(&X)
//#define ESCAPE(x) benchmark::DoNotOptimize(x)

using std::numeric_limits;
using sg14::make_fixed;
using sg14::make_ufixed;

////////////////////////////////////////////////////////////////////////////////
// entry point

BENCHMARK_MAIN();
