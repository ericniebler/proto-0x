////////////////////////////////////////////////////////////////////////////////////////////////////
// custom.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_CUSTOM_HPP_INCLUDED
#define BOOST_PROTO_V5_CUSTOM_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // custom
            template<template<typename...> class Expr, typename ...Rest>
            struct custom<Expr<_, Rest...>>
            {
                template<typename ExprDesc>
                using expr = Expr<ExprDesc, Rest...>;

                template<typename T>
                using terminal = Expr<terminal(T), Rest...>;

                template<typename T>
                using unary_plus = Expr<unary_plus(T), Rest...>;

                template<typename T>
                using negate = Expr<negate(T), Rest...>;

                template<typename T>
                using dereference = Expr<dereference(T), Rest...>;

                template<typename T>
                using complement = Expr<complement(T), Rest...>;

                template<typename T>
                using address_of = Expr<address_of(T), Rest...>;

                template<typename T>
                using logical_not = Expr<logical_not(T), Rest...>;

                template<typename T>
                using pre_inc = Expr<pre_inc(T), Rest...>;

                template<typename T>
                using pre_dec = Expr<pre_dec(T), Rest...>;

                template<typename T>
                using post_inc = Expr<post_inc(T), Rest...>;

                template<typename T>
                using post_dec = Expr<post_dec(T), Rest...>;

                template<typename L, typename R>
                using shift_left = Expr<shift_left(L, R), Rest...>;

                template<typename L, typename R>
                using shift_right = Expr<shift_right(L, R), Rest...>;

                template<typename L, typename R>
                using multiplies = Expr<multiplies(L, R), Rest...>;

                template<typename L, typename R>
                using divides = Expr<divides(L, R), Rest...>;

                template<typename L, typename R>
                using modulus = Expr<modulus(L, R), Rest...>;

                template<typename L, typename R>
                using plus = Expr<plus(L, R), Rest...>;

                template<typename L, typename R>
                using minus = Expr<minus(L, R), Rest...>;

                template<typename L, typename R>
                using less = Expr<less(L, R), Rest...>;

                template<typename L, typename R>
                using greater = Expr<greater(L, R), Rest...>;

                template<typename L, typename R>
                using less_equal = Expr<less_equal(L, R), Rest...>;

                template<typename L, typename R>
                using greater_equal = Expr<greater_equal(L, R), Rest...>;

                template<typename L, typename R>
                using equal_to = Expr<equal_to(L, R), Rest...>;

                template<typename L, typename R>
                using not_equal_to = Expr<not_equal_to(L, R), Rest...>;

                template<typename L, typename R>
                using logical_or = Expr<logical_or(L, R), Rest...>;

                template<typename L, typename R>
                using logical_and = Expr<logical_and(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_and = Expr<bitwise_and(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_or = Expr<bitwise_or(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_xor = Expr<bitwise_xor(L, R), Rest...>;

                template<typename L, typename R>
                using comma = Expr<comma(L, R), Rest...>;

                template<typename L, typename R>
                using mem_ptr = Expr<mem_ptr(L, R), Rest...>;

                template<typename L, typename R>
                using assign = Expr<assign(L, R), Rest...>;

                template<typename L, typename R>
                using shift_left_assign = Expr<shift_left_assign(L, R), Rest...>;

                template<typename L, typename R>
                using shift_right_assign = Expr<shift_right_assign(L, R), Rest...>;

                template<typename L, typename R>
                using multiplies_assign = Expr<multiplies_assign(L, R), Rest...>;

                template<typename L, typename R>
                using divides_assign = Expr<divides_assign(L, R), Rest...>;

                template<typename L, typename R>
                using modulus_assign = Expr<modulus_assign(L, R), Rest...>;

                template<typename L, typename R>
                using plus_assign = Expr<plus_assign(L, R), Rest...>;

                template<typename L, typename R>
                using minus_assign = Expr<minus_assign(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_and_assign = Expr<bitwise_and_assign(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_or_assign = Expr<bitwise_or_assign(L, R), Rest...>;

                template<typename L, typename R>
                using bitwise_xor_assign = Expr<bitwise_xor_assign(L, R), Rest...>;

                template<typename L, typename R>
                using subscript = Expr<subscript(L, R), Rest...>;

                template<typename L, typename R>
                using member = Expr<member(L, R), Rest...>;

                template<typename C, typename T, typename F>
                using if_else_ = Expr<if_else_(C, T, F), Rest...>;

                template<typename ...A>
                using function = Expr<function(A...), Rest...>;
            };
        }
    }
}

#endif
