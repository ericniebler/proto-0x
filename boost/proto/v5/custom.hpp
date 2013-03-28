////////////////////////////////////////////////////////////////////////////////////////////////////
// custom.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_CUSTOM_HPP_INCLUDED
#define BOOST_PROTO_CXX11_CUSTOM_HPP_INCLUDED

#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>
#include <boost/proto/v5/domain.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // custom
            template<template<typename...> class Expr, typename Domain>
            struct custom
            {
                template<typename ExprDesc>
                using expr = Expr<ExprDesc, Domain>;

                template<typename T>
                using terminal = Expr<terminal(T), Domain>;

                template<typename T>
                using unary_plus = Expr<unary_plus(T), Domain>;

                template<typename T>
                using negate = Expr<negate(T), Domain>;

                template<typename T>
                using dereference = Expr<dereference(T), Domain>;

                template<typename T>
                using complement = Expr<complement(T), Domain>;

                template<typename T>
                using address_of = Expr<address_of(T), Domain>;

                template<typename T>
                using logical_not = Expr<logical_not(T), Domain>;

                template<typename T>
                using pre_inc = Expr<pre_inc(T), Domain>;

                template<typename T>
                using pre_dec = Expr<pre_dec(T), Domain>;

                template<typename T>
                using post_inc = Expr<post_inc(T), Domain>;

                template<typename T>
                using post_dec = Expr<post_dec(T), Domain>;

                template<typename L, typename R>
                using shift_left = Expr<shift_left(L, R), Domain>;

                template<typename L, typename R>
                using shift_right = Expr<shift_right(L, R), Domain>;

                template<typename L, typename R>
                using multiplies = Expr<multiplies(L, R), Domain>;

                template<typename L, typename R>
                using divides = Expr<divides(L, R), Domain>;

                template<typename L, typename R>
                using modulus = Expr<modulus(L, R), Domain>;

                template<typename L, typename R>
                using plus = Expr<plus(L, R), Domain>;

                template<typename L, typename R>
                using minus = Expr<minus(L, R), Domain>;

                template<typename L, typename R>
                using less = Expr<less(L, R), Domain>;

                template<typename L, typename R>
                using greater = Expr<greater(L, R), Domain>;

                template<typename L, typename R>
                using less_equal = Expr<less_equal(L, R), Domain>;

                template<typename L, typename R>
                using greater_equal = Expr<greater_equal(L, R), Domain>;

                template<typename L, typename R>
                using equal_to = Expr<equal_to(L, R), Domain>;

                template<typename L, typename R>
                using not_equal_to = Expr<not_equal_to(L, R), Domain>;

                template<typename L, typename R>
                using logical_or = Expr<logical_or(L, R), Domain>;

                template<typename L, typename R>
                using logical_and = Expr<logical_and(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_and = Expr<bitwise_and(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_or = Expr<bitwise_or(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_xor = Expr<bitwise_xor(L, R), Domain>;

                template<typename L, typename R>
                using comma = Expr<comma(L, R), Domain>;

                template<typename L, typename R>
                using mem_ptr = Expr<mem_ptr(L, R), Domain>;

                template<typename L, typename R>
                using assign = Expr<assign(L, R), Domain>;

                template<typename L, typename R>
                using shift_left_assign = Expr<shift_left_assign(L, R), Domain>;

                template<typename L, typename R>
                using shift_right_assign = Expr<shift_right_assign(L, R), Domain>;

                template<typename L, typename R>
                using multiplies_assign = Expr<multiplies_assign(L, R), Domain>;

                template<typename L, typename R>
                using divides_assign = Expr<divides_assign(L, R), Domain>;

                template<typename L, typename R>
                using modulus_assign = Expr<modulus_assign(L, R), Domain>;

                template<typename L, typename R>
                using plus_assign = Expr<plus_assign(L, R), Domain>;

                template<typename L, typename R>
                using minus_assign = Expr<minus_assign(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_and_assign = Expr<bitwise_and_assign(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_or_assign = Expr<bitwise_or_assign(L, R), Domain>;

                template<typename L, typename R>
                using bitwise_xor_assign = Expr<bitwise_xor_assign(L, R), Domain>;

                template<typename L, typename R>
                using subscript = Expr<subscript(L, R), Domain>;

                template<typename L, typename R>
                using member = Expr<member(L, R), Domain>;

                template<typename C, typename T, typename F>
                using if_else_ = Expr<if_else_(C, T, F), Domain>;

                template<typename ...A>
                using function = Expr<function(A...), Domain>;
            };

            template<template<typename...> class Expr>
            struct custom<Expr, void>
            {
                template<typename ExprDesc>
                using expr = Expr<ExprDesc>;

                template<typename T>
                using terminal = Expr<terminal(T)>;

                template<typename T>
                using unary_plus = Expr<unary_plus(T)>;

                template<typename T>
                using negate = Expr<negate(T)>;

                template<typename T>
                using dereference = Expr<dereference(T)>;

                template<typename T>
                using complement = Expr<complement(T)>;

                template<typename T>
                using address_of = Expr<address_of(T)>;

                template<typename T>
                using logical_not = Expr<logical_not(T)>;

                template<typename T>
                using pre_inc = Expr<pre_inc(T)>;

                template<typename T>
                using pre_dec = Expr<pre_dec(T)>;

                template<typename T>
                using post_inc = Expr<post_inc(T)>;

                template<typename T>
                using post_dec = Expr<post_dec(T)>;

                template<typename L, typename R>
                using shift_left = Expr<shift_left(L, R)>;

                template<typename L, typename R>
                using shift_right = Expr<shift_right(L, R)>;

                template<typename L, typename R>
                using multiplies = Expr<multiplies(L, R)>;

                template<typename L, typename R>
                using divides = Expr<divides(L, R)>;

                template<typename L, typename R>
                using modulus = Expr<modulus(L, R)>;

                template<typename L, typename R>
                using plus = Expr<plus(L, R)>;

                template<typename L, typename R>
                using minus = Expr<minus(L, R)>;

                template<typename L, typename R>
                using less = Expr<less(L, R)>;

                template<typename L, typename R>
                using greater = Expr<greater(L, R)>;

                template<typename L, typename R>
                using less_equal = Expr<less_equal(L, R)>;

                template<typename L, typename R>
                using greater_equal = Expr<greater_equal(L, R)>;

                template<typename L, typename R>
                using equal_to = Expr<equal_to(L, R)>;

                template<typename L, typename R>
                using not_equal_to = Expr<not_equal_to(L, R)>;

                template<typename L, typename R>
                using logical_or = Expr<logical_or(L, R)>;

                template<typename L, typename R>
                using logical_and = Expr<logical_and(L, R)>;

                template<typename L, typename R>
                using bitwise_and = Expr<bitwise_and(L, R)>;

                template<typename L, typename R>
                using bitwise_or = Expr<bitwise_or(L, R)>;

                template<typename L, typename R>
                using bitwise_xor = Expr<bitwise_xor(L, R)>;

                template<typename L, typename R>
                using comma = Expr<comma(L, R)>;

                template<typename L, typename R>
                using mem_ptr = Expr<mem_ptr(L, R)>;

                template<typename L, typename R>
                using assign = Expr<assign(L, R)>;

                template<typename L, typename R>
                using shift_left_assign = Expr<shift_left_assign(L, R)>;

                template<typename L, typename R>
                using shift_right_assign = Expr<shift_right_assign(L, R)>;

                template<typename L, typename R>
                using multiplies_assign = Expr<multiplies_assign(L, R)>;

                template<typename L, typename R>
                using divides_assign = Expr<divides_assign(L, R)>;

                template<typename L, typename R>
                using modulus_assign = Expr<modulus_assign(L, R)>;

                template<typename L, typename R>
                using plus_assign = Expr<plus_assign(L, R)>;

                template<typename L, typename R>
                using minus_assign = Expr<minus_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_and_assign = Expr<bitwise_and_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_or_assign = Expr<bitwise_or_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_xor_assign = Expr<bitwise_xor_assign(L, R)>;

                template<typename L, typename R>
                using subscript = Expr<subscript(L, R)>;

                template<typename L, typename R>
                using member = Expr<member(L, R)>;

                template<typename C, typename T, typename F>
                using if_else_ = Expr<if_else_(C, T, F)>;

                template<typename ...A>
                using function = Expr<function(A...)>;
            };
        }
    }
}

#endif
