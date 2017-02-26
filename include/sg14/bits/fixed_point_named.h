
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_NAMED_H)
#define SG14_FIXED_POINT_NAMED_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include "common.h"

#include "fixed_point_arithmetic.h"
#endif

/// study group 14 of the C++ working group
namespace sg14 {

    /// \brief calculates the negative of a \ref fixed_point value
    /// \headerfile sg14/fixed_point
    ///
    /// \param rhs input value
    ///
    /// \return negative: - rhs
    ///
    /// \note This function negates the value
    /// without performing any additional scaling or conversion.
    ///
    /// \sa add, subtract, multiply, divide

    template<class RhsRep, int RhsExponent>
    constexpr auto negate(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    /// \brief calculates the sum of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs augend and addend
    ///
    /// \return sum: lhs + rhs
    ///
    /// \note This function add the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, subtract, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto add(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag, _impl::add_tag>(lhs, rhs))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag, _impl::add_tag>(lhs, rhs);
    }

    /// \brief calculates the difference of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs minuend and subtrahend
    ///
    /// \return difference: lhs - rhs
    ///
    /// \note This function subtracts the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, multiply, divide

    template<class Lhs, class Rhs>
    constexpr auto subtract(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag, _impl::subtract_tag>(lhs,
            rhs))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag, _impl::subtract_tag>(lhs,
                rhs);
    }

    /// \brief calculates the product of two \ref fixed_point factors
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs the factors
    ///
    /// \return product: lhs * rhs
    ///
    /// \note This function multiplies the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, divide

    template<class Lhs, class Rhs>
    constexpr auto multiply(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag, _impl::multiply_tag>(lhs, rhs))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag, _impl::multiply_tag>(lhs, rhs);
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile sg14/fixed_point
    ///
    /// \param lhs, rhs dividend and divisor
    ///
    /// \return quotient: lhs / rhs
    ///
    /// \note This function divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, multiply

    template<class Lhs, class Rhs>
    constexpr auto divide(const Lhs& lhs, const Rhs& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_named_function_tag, _impl::divide_tag>(lhs,
            rhs))
    {
        return _impl::fp::operate<_impl::fp::division_named_function_tag, _impl::divide_tag>(lhs,
                rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point_reciprocal

    namespace _fixed_point_named_impl {
        template<typename Dividend, typename Integer, Integer Value>
        struct fixed_point_reciprocal_result {
            using divisor_type = sg14::const_integer<Integer, Value>;
            static constexpr auto divisor_digits = divisor_type::digits;
            static constexpr auto dividend_digits = std::numeric_limits<Dividend>::digits;

            // TODO: determine whether the -1 actually makes the code faster
            static constexpr auto result_digits = dividend_digits + divisor_digits - 1;

            using type = make_fixed<1, result_digits, Dividend>;
        };
    }

    /// \brief produces a fixed-point variable which serves as a reciprocal for the given divisor
    /// \headerfile sg14/fixed_point
    ///
    /// \tparam Dividend the type of values by which the reciprocal will be multiplied
    /// \tparam Integer the type of Value
    /// \tparam Value the value of the divisor, passed in as /ref divisor
    ///
    /// \param divisor the divisor from which to calculate the reciprocal
    ///
    /// \return the reciprocal as an optimally-typed fixed_point instantiation;
    /// an approximation of 1 / divisor

    template<typename Dividend = int, typename Integer = int, Integer Value = 0>
    constexpr auto fixed_point_reciprocal(sg14::const_integer<Integer, Value>)
    -> typename _fixed_point_named_impl::fixed_point_reciprocal_result<Dividend, Integer, Value>::type {
        static_assert(Value != 0, "divide by zero");

        using result_type = typename _fixed_point_named_impl::fixed_point_reciprocal_result<Dividend, Integer, Value>::type;

        // TODO: improve rounding
        return divide(result_type{1}, Value);
    }
}

#endif	// SG14_FIXED_POINT_NAMED_H
