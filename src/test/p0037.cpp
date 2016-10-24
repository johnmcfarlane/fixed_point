
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sample_functions.h"

#include <gtest/gtest.h>

#include <type_traits>

using sg14::fixed_point;
using sg14::make_fixed;
using sg14::make_ufixed;
using sg14::multiply;
using sg14::set_width_t;
using sg14::sqrt;
using sg14::width;
using std::is_same;
using sg14::_impl::identical;

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in P0037

TEST(proposal, make_ufixed)
{
    make_ufixed<4, 4> value{15.9375};
    ASSERT_EQ(value, 15.9375);
}

TEST(proposal, make_fixed)
{
    make_fixed<2, 29> value{3.141592653};
    ASSERT_EQ(value, 3.1415926516056061);
}

namespace conversion {
    // Conversion
    fixed_point<uint16_t, -8> a = uint8_t{0};
    //fixed_point<uint16_t, -8> b = uint16_t{};  // maximum value exceeded
    //fixed_point<uint16_t, -8> c = int8_t{};  // minimum value exceeded
    //fixed_point<uint16_t, -8> d = fixed_point<uint16_t, -16>{};  // precision loss
    //uint16_t e = fixed_point<uint16_t, -8>{};  // precision loss
    //fixed_point<uint16_t, -8> f = double{};  // all of the above

    static_assert(fixed_point<int, -1>{.499} == 0.0, "Incorrect information in proposal section, Conversion");
}

// Operator Overloads

static_assert(identical(fixed_point<uint8_t, -3>{8} + fixed_point<int8_t, -4>{3}, fixed_point<int, -4>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + 3, fixed_point<int, -3>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + float{3}, float{11}), "Incorrect information in P0037 section, Operator Overloads");

// Overflow
TEST(proposal, overflow) {
    switch (sizeof(int)) {
    case 4: {
        auto sum = make_ufixed<2, 30>(3) + make_ufixed<2, 30>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    case 8: {
        auto sum = make_ufixed<2, 62>(3) + make_ufixed<2, 62>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    default:
        FAIL() << "dying to know what architecture this is";
    }
}

// Underflow
static_assert(identical(make_fixed<7, 0>(15)/make_fixed<7, 0>(2), fixed_point<int, -7>(7.5f)),
        "Incorrect information in proposal section, Underflow");

// Named Arithmetic Functions
namespace named_arithmetic1 {
    constexpr auto f = fixed_point<uint8_t, -4>{15.9375};
    constexpr auto p = multiply(f, f);

    static_assert(identical(p, fixed_point<uint16_t, -8>{254.00390625}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

TEST(proposal, named_arithmetic2)
{
    auto f = fixed_point<unsigned, -28>{15.9375};
    auto p = f * f;

    static_assert(is_same<decltype(p), fixed_point<unsigned, -56>>::value, "Incorrect information in proposal section, Named Arithmetic Functions");
    ASSERT_EQ(p, 0.);
}

namespace named_arithmetic3 {
    constexpr auto f = fixed_point<unsigned, -28>{15.9375};
    constexpr auto p = multiply(f, f);

    static_assert(identical(p, fixed_point<uint64_t, -56>{254.00390625}),
            "Incorrect information in proposal section, Named Arithmetic Functions");
}

namespace named_arithmetic4 {
    constexpr auto a1 = fixed_point<int8_t, 32>{0x7f00000000LL};
    constexpr auto a2 = fixed_point<int8_t, 0>{0x7f};
    constexpr auto s = add(a1, a2);

    static_assert(identical(s, fixed_point<int64_t, 0>{0x7f0000007fLL}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

namespace named_arithmetic5 {
    constexpr auto n = fixed_point<uint32_t, -16>{1};
    constexpr auto d = fixed_point<uint32_t, -16>{2};

    constexpr auto q1 = n/d;
    static_assert(identical(q1, fixed_point<uint64_t, -32>{0.5}), "Incorrect information in proposal section, Named Arithmetic Functions");

    constexpr auto q2 = divide(n, d);
    static_assert(identical(q2, fixed_point<uint32_t, 0>{0}), "Incorrect information in proposal section, Named Arithmetic Functions");
}

// The `width` Helper Type
static_assert(width<int16_t>::value == 16, "Incorrect information in proposal section, The `width` Helper Type");

// The `set_width` and `set_width_t` Helper Types
static_assert(is_same<set_width_t<int8_t, 64>, int64_t>::value, "Incorrect information in proposal section, The `set_width` and `set_width_t` Helper Types");

// Examples
template<class Fp>
constexpr auto magnitude(Fp x, Fp y, Fp z)
-> decltype(sqrt(x*x+y*y+z*z))
{
    return sqrt(x*x+y*y+z*z);
}

TEST(proposal, examples)
{
    constexpr auto m = magnitude(
            make_ufixed<4, 12>(1),
            make_ufixed<4, 12>(4),
            make_ufixed<4, 12>(9));
#if defined(_MSC_VER)
    constexpr auto expected = make_fixed<7, 24>{9.8994948864};
    static_assert(std::is_same<decltype(m), decltype(expected)>::value, "Incorrect formation in proposal section, Examples");
    ASSERT_EQ(expected, m);
#else
    static_assert(identical(m, make_fixed<7, 24>{9.8994948864}), "Incorrect information in proposal section, Examples");
#endif
}

TEST(proposal, zero)
{
    static fixed_point<> zero;
    ASSERT_EQ(zero, fixed_point<>(0));
}
