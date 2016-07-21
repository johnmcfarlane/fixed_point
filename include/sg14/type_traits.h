
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `sg14::fixed_point` type

#if !defined(SG14_TYPE_TRAITS_H)
#define SG14_TYPE_TRAITS_H 1

#include "bits/config.h"

#include <cinttypes>
#include <climits>
#include <tuple>
#include <type_traits>

/// study group 14 of the C++ working group
namespace sg14 {
    using _width_type = unsigned;
    
    ////////////////////////////////////////////////////////////////////////////////
    // width - new type property which returns number of bits of information

    /// \brief provides width of numeric type
    ///
    /// \tparam T given numeric type
    ///
    /// \var value the width of the given numeric type in bits
    ///
    /// \remarks If \ref T is a fixed-point numeric type such as an integral type,
    /// \ref is the width of T in bits.
    /// \remarks The width is defined as the number of digits including any sign bit.

    template<class T>
    struct width;

    template<>
    struct width<char> : std::integral_constant<_width_type, sizeof(char)*CHAR_BIT> {
    };
    template<>
    struct width<wchar_t> : std::integral_constant<_width_type, sizeof(wchar_t)*CHAR_BIT> {
    };

    template<>
    struct width<signed char> : std::integral_constant<_width_type, sizeof(signed char)*CHAR_BIT> {
    };
    template<>
    struct width<unsigned char> : std::integral_constant<_width_type, sizeof(unsigned char)*CHAR_BIT> {
    };

    template<>
    struct width<signed short> : std::integral_constant<_width_type, sizeof(signed short)*CHAR_BIT> {
    };
    template<>
    struct width<unsigned short> : std::integral_constant<_width_type, sizeof(unsigned short)*CHAR_BIT> {
    };

    template<>
    struct width<signed int> : std::integral_constant<_width_type, sizeof(signed int)*CHAR_BIT> {
    };
    template<>
    struct width<unsigned int> : std::integral_constant<_width_type, sizeof(unsigned int)*CHAR_BIT> {
    };

    template<>
    struct width<signed long> : std::integral_constant<_width_type, sizeof(signed long)*CHAR_BIT> {
    };
    template<>
    struct width<unsigned long> : std::integral_constant<_width_type, sizeof(unsigned long)*CHAR_BIT> {
    };

    template<>
    struct width<signed long long> : std::integral_constant<_width_type, sizeof(signed long long)*CHAR_BIT> {
    };
    template<>
    struct width<unsigned long long> : std::integral_constant<_width_type, sizeof(unsigned long long)*CHAR_BIT> {
    };

    template<>
    struct width<float> : std::integral_constant<_width_type, sizeof(float)*CHAR_BIT> {
    };
    template<>
    struct width<double> : std::integral_constant<_width_type, sizeof(double)*CHAR_BIT> {
    };
    template<>
    struct width<long double> : std::integral_constant<_width_type, sizeof(long double)*CHAR_BIT> {
    };

    namespace _type_traits_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // built-in families

#if defined(SG14_INT128_ENABLED)
        using signed_family = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t, SG14_INT128>;
        using unsigned_family = std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, SG14_UINT128>;
#else
        using signed_family = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;
        using unsigned_family = std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;
#endif

#if defined(_MSC_VER)
        using float_family = std::tuple<float, double>;
#else
        using float_family = std::tuple<float, double, long double>;
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // first_fit

        template<_width_type MinNumBits, class Family, class = void>
        struct first_fit;

        template<_width_type MinNumBits, class FamilyMembersHead, class ... FamilyMembersTail>
        struct first_fit<
                MinNumBits,
                std::tuple<FamilyMembersHead, FamilyMembersTail ...>,
                typename std::enable_if<width<FamilyMembersHead>::value>=MinNumBits>::type> {
            using type = FamilyMembersHead;
        };

        template<_width_type MinNumBits, class FamilyMembersHead, class ... FamilyMembersTail>
        struct first_fit<
                MinNumBits,
                std::tuple<FamilyMembersHead, FamilyMembersTail ...>,
                typename std::enable_if<width<FamilyMembersHead>::value<MinNumBits>::type> {
            using _tail_base = first_fit<MinNumBits, std::tuple<FamilyMembersTail ...>>;
            using _tail_type = typename _tail_base::type;
        public:
            using type = typename std::conditional<
                    width<FamilyMembersHead>::value>=MinNumBits,
                    FamilyMembersHead,
                    _tail_type>::type;
        };
    }

#if (__cplusplus >= 201402L)
    ////////////////////////////////////////////////////////////////////////////////
    // width_v - equals number of bits of information in given type

    /// \brief provides width of numeric type
    ///
    /// \tparam T given numeric type
    ///
    /// \remarks The width is defined as the number of digits including any sign bit.

    template<class T>
    constexpr unsigned width_v = width<T>::value;
#endif

    /// resizes a type;
    /// can be specialized for any type for which resizing that type makes sense
    ///
    /// \sa set_width_t
    template<class Archetype, _width_type MinNumBits>
    struct set_width;

    // sg14::set_width specialized for char/wchar_t
    template<_width_type MinNumBits>
    struct set_width<char, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, typename std::conditional<std::is_signed<char>::value, _type_traits_impl::signed_family, _type_traits_impl::unsigned_family>::type> {
    };
    template<_width_type MinNumBits>
    struct set_width<wchar_t, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, typename std::conditional<std::is_signed<char>::value, _type_traits_impl::signed_family, _type_traits_impl::unsigned_family>::type> {
    };

    // sg14::set_width specialized for 8-bit built-in integers
    template<_width_type MinNumBits>
    struct set_width<signed char, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::signed_family> {
    };
    template<_width_type MinNumBits>
    struct set_width<unsigned char, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::unsigned_family> {
    };

    // sg14::set_width specialized for 16-bit built-in integers
    template<_width_type MinNumBits>
    struct set_width<signed short, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::signed_family> {
    };
    template<_width_type MinNumBits>
    struct set_width<unsigned short, MinNumBits>
            : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::unsigned_family> {
    };

    // sg14::set_width specialized for 32-bit built-in integers
    template<_width_type MinNumBits>
    struct set_width<signed int, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::signed_family> {
    };
    template<_width_type MinNumBits>
    struct set_width<unsigned int, MinNumBits>
            : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::unsigned_family> {
    };

    // sg14::set_width specialized for 64-bit built-in integers
    template<_width_type MinNumBits>
    struct set_width<signed long, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::signed_family> {
    };
    template<_width_type MinNumBits>
    struct set_width<unsigned long, MinNumBits>
            : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::unsigned_family> {
    };

    // sg14::set_width specialized for 64-bit built-in integers
    template<_width_type MinNumBits>
    struct set_width<signed long long, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::signed_family> {
    };
    template<_width_type MinNumBits>
    struct set_width<unsigned long long, MinNumBits>
            : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::unsigned_family> {
    };

    // sg14::set_width specialized for float
    template<_width_type MinNumBits>
    struct set_width<float, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::float_family> {
    };

    // sg14::set_width specialized for double
    template<_width_type MinNumBits>
    struct set_width<double, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::float_family> {
    };

    // sg14::set_width specialized for long double
    template<_width_type MinNumBits>
    struct set_width<long double, MinNumBits> : _type_traits_impl::first_fit<MinNumBits, _type_traits_impl::float_family> {
    };

    /// \brief resizes a type
    ///
    /// \tparam Archetype the type to resize
    /// \tparam MinNumBits the desired width in bits
    ///
    /// \par Examples
    ///
    /// To resize a native-sized unsigned int to 2-bytes:
    /// \snippet snippets.cpp use set_width 1
    ///
    /// To resize a signed byte type to a built-in signed type of at least 5 bytes:
    /// \snippet snippets.cpp use set_width 2
    ///
    /// To resize a signed, 1-byte fixed-point type to a fixed-point type of at least 3 bytes:
    /// \snippet snippets.cpp use set_width 3
    template<class Archetype, _width_type MinNumBits>
    using set_width_t = typename set_width<Archetype, MinNumBits>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // import selected <type_traits> definitions from std namespace

    // common_type
    template<class ...T>
    struct common_type;

    template<class ...T>
    struct common_type : std::common_type<T...> {
    };

    // is_integral
    template<class T>
    struct is_integral;

    template<class T>
    struct is_integral : std::is_integral<T> {
    };

    // is_signed
    template<class T>
    struct is_signed;

    template<class T>
    struct is_signed : std::is_signed<T> {
    };

    // is_unsigned
    template<class T>
    struct is_unsigned;

    template<class T>
    struct is_unsigned : std::is_unsigned<T> {
    };

    // make_signed
    template<class T>
    struct make_signed;

    template<class T>
    struct make_signed : std::make_signed<T> {
    };

    // make_unsigned
    template<class T>
    struct make_unsigned;

    template<class T>
    struct make_unsigned : std::make_unsigned<T> {
    };
}

#endif	// SG14_TYPE_TRAITS_H
