////////////////////////////////////////////////////////////////////////////////////////////////////
// operators.hpp
// Contains all the overloaded operators that make it possible to build
// Proto expression trees.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// NO INCLUDE GUARD. THIS FILE IS MEANT TO BE INCLUDED MULTIPLE TIMES.

#ifndef BOOST_PROTO_V5_OPERATORS_HPP_INCLUDED
#error This file is not meant to be included directly
#endif

#ifndef BOOST_PROTO_OPERATOR_TRAITS
#error Boost.Proto error: BOOST_PROTO_OPERATOR_TRAITS not defined.
#endif

#define BOOST_PROTO_TRAIT BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PROTO_OPERATOR_TRAITS)
#define BOOST_PROTO_DOMAIN BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PROTO_OPERATOR_TRAITS)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::unary_plus(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator +(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::unary_plus, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::negate(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator -(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::negate, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::dereference(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator *(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::dereference, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::complement(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ~(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::complement, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::address_of(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator &(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::address_of, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::logical_not(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator !(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::logical_not, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::pre_inc(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ++(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::pre_inc, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::pre_dec(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator --(Arg &&arg)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::pre_dec, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::post_inc(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ++(Arg &&arg, int)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::post_inc, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<
    typename Arg
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_UNARY_(BOOST_PROTO_TRAIT, Arg)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::post_dec(Arg)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator --(Arg &&arg, int)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::post_dec, BOOST_PROTO_DOMAIN>(
        static_cast<Arg &&>(arg)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::shift_left(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator <<(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::shift_left, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::shift_right(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator >>(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::shift_right, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::multiplies(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator *(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::multiplies, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::divides(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator /(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::divides, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::modulus(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator %(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::modulus, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::plus(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator +(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::plus, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::minus(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator -(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::minus, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::less(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator <(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::less, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::greater(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator >(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::greater, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::less_equal(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator <=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::less_equal, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::greater_equal(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator >=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::greater_equal, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::equal_to(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ==(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::equal_to, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::not_equal_to(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator !=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::not_equal_to, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::logical_or(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ||(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::logical_or, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::logical_and(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator &&(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::logical_and, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_and(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator &(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_and, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_or(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator |(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_or, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_xor(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ^(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_xor, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::comma(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ,(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::comma, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::mem_ptr(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ->*(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::mem_ptr, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::shift_left_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator <<=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::shift_left_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::shift_right_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator >>=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::shift_right_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::multiplies_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator *=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::multiplies_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::divides_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator /=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::divides_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::modulus_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator %=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::modulus_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::plus_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator +=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::plus_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::minus_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator -=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::minus_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_and_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator &=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_and_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_or_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator |=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_or_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

template<typename Left, typename Right
  , BOOST_PROTO_ENABLE_IF(
        BOOST_PROTO_APPLY_BINARY_(BOOST_PROTO_TRAIT, Left, Right)
    )
  , BOOST_PROTO_ENABLE_IF(
        ::boost::proto::v5::detail::is_grammatical<
            ::boost::proto::v5::bitwise_xor_assign(Left, Right)
          , BOOST_PROTO_DOMAIN
        >::value
    )
>
inline constexpr auto operator ^=(Left &&left, Right &&right)
BOOST_PROTO_AUTO_RETURN(
    ::boost::proto::v5::make_expr< ::boost::proto::v5::bitwise_xor_assign, BOOST_PROTO_DOMAIN>(
        static_cast<Left &&>(left)
      , static_cast<Right &&>(right)
    )
)

#undef BOOST_PROTO_DOMAIN
#undef BOOST_PROTO_TRAIT

#undef BOOST_PROTO_OPERATOR_TRAITS
