
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief test of reciprocal trick: https://godbolt.org/g/8oVh3V

#include <sg14/fixed_point>
#include <gtest/gtest.h>
#include <algorithm>

using namespace sg14;

int compiler_divide(int32_t dividend, int32_t divisor) {
    return dividend / divisor;
}

int fixed_point_divide(int32_t dividend, fixed_point<int64_t, -31> reciprocal) {
    return dividend * reciprocal;
}

TEST(zero_cost_reciprocal, stress) {
    constexpr int32_t samples[] = {
        0, 1, 2, 3, 7, 23, 127, 128, 1000, 65535, 65536, 777777, 2000000000, std::numeric_limits<int32_t>::max()
    };
    auto samples_begin = std::begin(samples);
    auto samples_end = std::end(samples);
    std::for_each (samples_begin + 1, samples_end, [&](int32_t divisor) {
#if (__cplusplus > 201402L)
        auto reciprocal = fixed_point{1} / divisor;
#else
        auto reciprocal = fixed_point<>{1} / divisor;
#endif
        reciprocal += std::numeric_limits<decltype(reciprocal)>::min() * (((divisor + 1) / 2));

        std::for_each (samples_begin, samples_end, [&](int32_t dividend) {
            auto actual = fixed_point_divide(dividend, reciprocal);
            auto expected = compiler_divide(dividend, divisor);
            assert(abs(expected - actual) <= 0);
        });
    });
}
