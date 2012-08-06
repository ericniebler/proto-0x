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
            using nullary_expr = Expr<Tag, children<T>, Domain>;

            template<typename Tag, typename T>
            using unary_expr = Expr<Tag, children<T>, Domain>;

            template<typename Tag, typename L, typename R>
            using binary_expr = Expr<Tag, children<L, R>, Domain>;

            template<typename Tag, typename ...T>
            using nary_expr = Expr<Tag, children<T...>, Domain>;

            template<typename T>
            using terminal = Expr<terminal, children<T>, Domain>;

            template<typename T>
            using unary_plus = Expr<unary_plus, children<T>, Domain>;

            template<typename T>
            using negate = Expr<negate, children<T>, Domain>;

            template<typename T>
            using dereference = Expr<dereference, children<T>, Domain>;

            template<typename T>
            using complement = Expr<complement, children<T>, Domain>;

            template<typename T>
            using address_of = Expr<address_of, children<T>, Domain>;

            template<typename T>
            using logical_not = Expr<logical_not, children<T>, Domain>;

            template<typename T>
            using pre_inc = Expr<pre_inc, children<T>, Domain>;

            template<typename T>
            using pre_dec = Expr<pre_dec, children<T>, Domain>;

            template<typename T>
            using post_inc = Expr<post_inc, children<T>, Domain>;

            template<typename T>
            using post_dec = Expr<post_dec, children<T>, Domain>;

            template<typename L, typename R>
            using shift_left = Expr<shift_left, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_right = Expr<shift_right, children<L, R>, Domain>;

            template<typename L, typename R>
            using multiplies = Expr<multiplies, children<L, R>, Domain>;

            template<typename L, typename R>
            using divides = Expr<divides, children<L, R>, Domain>;

            template<typename L, typename R>
            using modulus = Expr<modulus, children<L, R>, Domain>;

            template<typename L, typename R>
            using plus = Expr<plus, children<L, R>, Domain>;

            template<typename L, typename R>
            using minus = Expr<minus, children<L, R>, Domain>;

            template<typename L, typename R>
            using less = Expr<less, children<L, R>, Domain>;

            template<typename L, typename R>
            using greater = Expr<greater, children<L, R>, Domain>;

            template<typename L, typename R>
            using less_equal = Expr<less_equal, children<L, R>, Domain>;

            template<typename L, typename R>
            using greater_equal = Expr<greater_equal, children<L, R>, Domain>;

            template<typename L, typename R>
            using equal_to = Expr<equal_to, children<L, R>, Domain>;

            template<typename L, typename R>
            using not_equal_to = Expr<not_equal_to, children<L, R>, Domain>;

            template<typename L, typename R>
            using logical_or = Expr<logical_or, children<L, R>, Domain>;

            template<typename L, typename R>
            using logical_and = Expr<logical_and, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_and = Expr<bitwise_and, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_or = Expr<bitwise_or, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_xor = Expr<bitwise_xor, children<L, R>, Domain>;

            template<typename L, typename R>
            using comma = Expr<comma, children<L, R>, Domain>;

            template<typename L, typename R>
            using mem_ptr = Expr<mem_ptr, children<L, R>, Domain>;

            template<typename L, typename R>
            using assign = Expr<assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_left_assign = Expr<shift_left_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using shift_right_assign = Expr<shift_right_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using multiplies_assign = Expr<multiplies_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using divides_assign = Expr<divides_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using modulus_assign = Expr<modulus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using plus_assign = Expr<plus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using minus_assign = Expr<minus_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_and_assign = Expr<bitwise_and_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_or_assign = Expr<bitwise_or_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using bitwise_xor_assign = Expr<bitwise_xor_assign, children<L, R>, Domain>;

            template<typename L, typename R>
            using subscript = Expr<subscript, children<L, R>, Domain>;

            template<typename L, typename R>
            using member = Expr<member, children<L, R>, Domain>;

            template<typename C, typename T, typename F>
            using if_else_ = Expr<if_else_, children<C, T, F>, Domain>;

            template<typename ...A>
            using function = Expr<function, children<A...>, Domain>;
        };

        template<template<typename...> class Expr>
        struct custom<Expr, void>
        {
            template<typename Tag, typename T>
            using nullary_expr = Expr<Tag, children<T>>;

            template<typename Tag, typename T>
            using unary_expr = Expr<Tag, children<T>>;

            template<typename Tag, typename L, typename R>
            using binary_expr = Expr<Tag, children<L, R>>;

            template<typename Tag, typename ...T>
            using nary_expr = Expr<Tag, children<T...>>;

            template<typename T>
            using terminal = Expr<terminal, children<T>>;

            template<typename T>
            using unary_plus = Expr<unary_plus, children<T>>;

            template<typename T>
            using negate = Expr<negate, children<T>>;

            template<typename T>
            using dereference = Expr<dereference, children<T>>;

            template<typename T>
            using complement = Expr<complement, children<T>>;

            template<typename T>
            using address_of = Expr<address_of, children<T>>;

            template<typename T>
            using logical_not = Expr<logical_not, children<T>>;

            template<typename T>
            using pre_inc = Expr<pre_inc, children<T>>;

            template<typename T>
            using pre_dec = Expr<pre_dec, children<T>>;

            template<typename T>
            using post_inc = Expr<post_inc, children<T>>;

            template<typename T>
            using post_dec = Expr<post_dec, children<T>>;

            template<typename L, typename R>
            using shift_left = Expr<shift_left, children<L, R>>;

            template<typename L, typename R>
            using shift_right = Expr<shift_right, children<L, R>>;

            template<typename L, typename R>
            using multiplies = Expr<multiplies, children<L, R>>;

            template<typename L, typename R>
            using divides = Expr<divides, children<L, R>>;

            template<typename L, typename R>
            using modulus = Expr<modulus, children<L, R>>;

            template<typename L, typename R>
            using plus = Expr<plus, children<L, R>>;

            template<typename L, typename R>
            using minus = Expr<minus, children<L, R>>;

            template<typename L, typename R>
            using less = Expr<less, children<L, R>>;

            template<typename L, typename R>
            using greater = Expr<greater, children<L, R>>;

            template<typename L, typename R>
            using less_equal = Expr<less_equal, children<L, R>>;

            template<typename L, typename R>
            using greater_equal = Expr<greater_equal, children<L, R>>;

            template<typename L, typename R>
            using equal_to = Expr<equal_to, children<L, R>>;

            template<typename L, typename R>
            using not_equal_to = Expr<not_equal_to, children<L, R>>;

            template<typename L, typename R>
            using logical_or = Expr<logical_or, children<L, R>>;

            template<typename L, typename R>
            using logical_and = Expr<logical_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and = Expr<bitwise_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or = Expr<bitwise_or, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor = Expr<bitwise_xor, children<L, R>>;

            template<typename L, typename R>
            using comma = Expr<comma, children<L, R>>;

            template<typename L, typename R>
            using mem_ptr = Expr<mem_ptr, children<L, R>>;

            template<typename L, typename R>
            using assign = Expr<assign, children<L, R>>;

            template<typename L, typename R>
            using shift_left_assign = Expr<shift_left_assign, children<L, R>>;

            template<typename L, typename R>
            using shift_right_assign = Expr<shift_right_assign, children<L, R>>;

            template<typename L, typename R>
            using multiplies_assign = Expr<multiplies_assign, children<L, R>>;

            template<typename L, typename R>
            using divides_assign = Expr<divides_assign, children<L, R>>;

            template<typename L, typename R>
            using modulus_assign = Expr<modulus_assign, children<L, R>>;

            template<typename L, typename R>
            using plus_assign = Expr<plus_assign, children<L, R>>;

            template<typename L, typename R>
            using minus_assign = Expr<minus_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and_assign = Expr<bitwise_and_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or_assign = Expr<bitwise_or_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor_assign = Expr<bitwise_xor_assign, children<L, R>>;

            template<typename L, typename R>
            using subscript = Expr<subscript, children<L, R>>;

            template<typename L, typename R>
            using member = Expr<member, children<L, R>>;

            template<typename C, typename T, typename F>
            using if_else_ = Expr<if_else_, children<C, T, F>>;

            template<typename ...A>
            using function = Expr<function, children<A...>>;
        };
    }
}

#endif
