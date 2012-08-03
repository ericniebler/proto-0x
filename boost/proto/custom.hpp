///////////////////////////////////////////////////////////////////////////////
// custom.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CUSTOM_HPP_INCLUDED
#define BOOST_PROTO_CUSTOM_HPP_INCLUDED

#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/children.hpp>
#include <boost/proto/domain.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // custom
        template<template<typename...> class Expr, typename Domain>
        struct custom
        {
            template<typename Tag, typename T>
            using nullary_expr = Expr<typename detail::nullary_tag<Tag>::type, children<T>, Domain>;

            template<typename Tag, typename T>
            using unary_expr = Expr<typename detail::unary_tag<Tag>::type, children<T>, Domain>;

            template<typename Tag, typename L, typename R>
            using binary_expr = Expr<Tag, children<L, R>, Domain>;

            template<typename Tag, typename ...T>
            using nary_expr = Expr<Tag, children<T...>, Domain>;

            template<typename T>
            using terminal = Expr<tag::terminal, children<T>, Domain>;

            template<typename T>
            using unary_plus = Expr<tag::unary_plus, children<T>, Domain>;

            template<typename T>
            using negate = Expr<tag::negate, children<T>, Domain>;

            template<typename T>
            using dereference = Expr<tag::dereference, children<T>, Domain>;

            template<typename T>
            using complement = Expr<tag::complement, children<T>, Domain>;

            template<typename T>
            using address_of = Expr<tag::address_of, children<T>, Domain>;

            template<typename T>
            using logical_not = Expr<tag::logical_not, children<T>, Domain>;

            template<typename T>
            using pre_inc = Expr<tag::pre_inc, children<T>, Domain>;

            template<typename T>
            using pre_dec = Expr<tag::pre_dec, children<T>, Domain>;

            template<typename T>
            using post_inc = Expr<tag::post_inc, children<T>, Domain>;

            template<typename T>
            using post_dec = Expr<tag::post_dec, children<T>, Domain>;

            template<typename L, typename R>
            using shift_left = Expr<tag::shift_left, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_right = Expr<tag::shift_right, children<L, R>, Domain>;

            template<typename L, typename R>
            using multiplies = Expr<tag::multiplies, children<L, R>, Domain>;

            template<typename L, typename R>
            using divides = Expr<tag::divides, children<L, R>, Domain>;

            template<typename L, typename R>
            using modulus = Expr<tag::modulus, children<L, R>, Domain>;

            template<typename L, typename R>
            using plus = Expr<tag::plus, children<L, R>, Domain>;

            template<typename L, typename R>
            using minus = Expr<tag::minus, children<L, R>, Domain>;

            template<typename L, typename R>
            using less = Expr<tag::less, children<L, R>, Domain>;

            template<typename L, typename R>
            using greater = Expr<tag::greater, children<L, R>, Domain>;

            template<typename L, typename R>
            using less_equal = Expr<tag::less_equal, children<L, R>, Domain>;

            template<typename L, typename R>
            using greater_equal = Expr<tag::greater_equal, children<L, R>, Domain>;

            template<typename L, typename R>
            using equal_to = Expr<tag::equal_to, children<L, R>, Domain>;

            template<typename L, typename R>
            using not_equal_to = Expr<tag::not_equal_to, children<L, R>, Domain>;

            template<typename L, typename R>
            using logical_or = Expr<tag::logical_or, children<L, R>, Domain>;

            template<typename L, typename R>
            using logical_and = Expr<tag::logical_and, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_and = Expr<tag::bitwise_and, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_or = Expr<tag::bitwise_or, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_xor = Expr<tag::bitwise_xor, children<L, R>, Domain>;

            template<typename L, typename R>
            using comma = Expr<tag::comma, children<L, R>, Domain>;

            template<typename L, typename R>
            using mem_ptr = Expr<tag::mem_ptr, children<L, R>, Domain>;

            template<typename L, typename R>
            using assign = Expr<tag::assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_left_assign = Expr<tag::shift_left_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_right_assign = Expr<tag::shift_right_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using multiplies_assign = Expr<tag::multiplies_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using divides_assign = Expr<tag::divides_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using modulus_assign = Expr<tag::modulus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using plus_assign = Expr<tag::plus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using minus_assign = Expr<tag::minus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_and_assign = Expr<tag::bitwise_and_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_or_assign = Expr<tag::bitwise_or_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_xor_assign = Expr<tag::bitwise_xor_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using subscript = Expr<tag::subscript, children<L, R>, Domain>;

            template<typename L, typename R>
            using member = Expr<tag::member, children<L, R>, Domain>;

            template<typename C, typename T, typename F>
            using if_else_ = Expr<tag::if_else_, children<C, T, F>, Domain>;

            template<typename ...A>
            using function = Expr<tag::function, children<A...>, Domain>;
        };

        template<template<typename...> class Expr>
        struct custom<Expr, void>
        {
            template<typename Tag, typename T>
            using nullary_expr = Expr<typename detail::nullary_tag<Tag>::type, children<T>>;

            template<typename Tag, typename T>
            using unary_expr = Expr<typename detail::unary_tag<Tag>::type, children<T>>;

            template<typename Tag, typename L, typename R>
            using binary_expr = Expr<Tag, children<L, R>>;

            template<typename Tag, typename ...T>
            using nary_expr = Expr<Tag, children<T...>>;

            template<typename T>
            using terminal = Expr<tag::terminal, children<T>>;

            template<typename T>
            using unary_plus = Expr<tag::unary_plus, children<T>>;

            template<typename T>
            using negate = Expr<tag::negate, children<T>>;

            template<typename T>
            using dereference = Expr<tag::dereference, children<T>>;

            template<typename T>
            using complement = Expr<tag::complement, children<T>>;

            template<typename T>
            using address_of = Expr<tag::address_of, children<T>>;

            template<typename T>
            using logical_not = Expr<tag::logical_not, children<T>>;

            template<typename T>
            using pre_inc = Expr<tag::pre_inc, children<T>>;

            template<typename T>
            using pre_dec = Expr<tag::pre_dec, children<T>>;

            template<typename T>
            using post_inc = Expr<tag::post_inc, children<T>>;

            template<typename T>
            using post_dec = Expr<tag::post_dec, children<T>>;

            template<typename L, typename R>
            using shift_left = Expr<tag::shift_left, children<L, R>>;

            template<typename L, typename R>
            using shift_right = Expr<tag::shift_right, children<L, R>>;

            template<typename L, typename R>
            using multiplies = Expr<tag::multiplies, children<L, R>>;

            template<typename L, typename R>
            using divides = Expr<tag::divides, children<L, R>>;

            template<typename L, typename R>
            using modulus = Expr<tag::modulus, children<L, R>>;

            template<typename L, typename R>
            using plus = Expr<tag::plus, children<L, R>>;

            template<typename L, typename R>
            using minus = Expr<tag::minus, children<L, R>>;

            template<typename L, typename R>
            using less = Expr<tag::less, children<L, R>>;

            template<typename L, typename R>
            using greater = Expr<tag::greater, children<L, R>>;

            template<typename L, typename R>
            using less_equal = Expr<tag::less_equal, children<L, R>>;

            template<typename L, typename R>
            using greater_equal = Expr<tag::greater_equal, children<L, R>>;

            template<typename L, typename R>
            using equal_to = Expr<tag::equal_to, children<L, R>>;

            template<typename L, typename R>
            using not_equal_to = Expr<tag::not_equal_to, children<L, R>>;

            template<typename L, typename R>
            using logical_or = Expr<tag::logical_or, children<L, R>>;

            template<typename L, typename R>
            using logical_and = Expr<tag::logical_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and = Expr<tag::bitwise_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or = Expr<tag::bitwise_or, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor = Expr<tag::bitwise_xor, children<L, R>>;

            template<typename L, typename R>
            using comma = Expr<tag::comma, children<L, R>>;

            template<typename L, typename R>
            using mem_ptr = Expr<tag::mem_ptr, children<L, R>>;

            template<typename L, typename R>
            using assign = Expr<tag::assign, children<L, R>>;

            template<typename L, typename R>
            using shift_left_assign = Expr<tag::shift_left_assign, children<L, R>>;

            template<typename L, typename R>
            using shift_right_assign = Expr<tag::shift_right_assign, children<L, R>>;

            template<typename L, typename R>
            using multiplies_assign = Expr<tag::multiplies_assign, children<L, R>>;

            template<typename L, typename R>
            using divides_assign = Expr<tag::divides_assign, children<L, R>>;

            template<typename L, typename R>
            using modulus_assign = Expr<tag::modulus_assign, children<L, R>>;

            template<typename L, typename R>
            using plus_assign = Expr<tag::plus_assign, children<L, R>>;

            template<typename L, typename R>
            using minus_assign = Expr<tag::minus_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and_assign = Expr<tag::bitwise_and_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or_assign = Expr<tag::bitwise_or_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor_assign = Expr<tag::bitwise_xor_assign, children<L, R>>;

            template<typename L, typename R>
            using subscript = Expr<tag::subscript, children<L, R>>;

            template<typename L, typename R>
            using member = Expr<tag::member, children<L, R>>;

            template<typename C, typename T, typename F>
            using if_else_ = Expr<tag::if_else_, children<C, T, F>>;

            template<typename ...A>
            using function = Expr<tag::function, children<A...>>;
        };
    }
}

#endif
