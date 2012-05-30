///////////////////////////////////////////////////////////////////////////////
// proto_fwd.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FWD_HPP_INCLUDED
#define BOOST_PROTO_FWD_HPP_INCLUDED

#include <utility>
#include <type_traits>

// C++11 eliminates the need for macros! Oh, wait ...
#define BOOST_PROTO_RETURN(...)                                                                     \
    -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF(...)                                                                  \
    typename std::enable_if<(__VA_ARGS__)>::type *& = boost::proto::detail::enabler

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF_EXPR(...)                                                             \
    decltype(static_cast<void>(__VA_ARGS__)) *& = boost::proto::detail::enabler

// For adding defaulted default, copy and move constructors, and move/copy assign.
#define BOOST_PROTO_REGULAR_TRIVIAL_CLASS(CLASS)                                                    \
    CLASS() = default; /*required for the type to be trivial!*/                                     \
    CLASS(CLASS const &) = default; /* memberwise copy */                                           \
    CLASS(CLASS &&) = default; /* member-wise move */                                               \
    /* These would otherwise be deleted because we */                                               \
    /* declared a move constructor! */                                                              \
    CLASS &operator=(CLASS const &) = default; /* memberwise copy assign */                         \
    CLASS &operator=(CLASS &&) = default /* memberwise move assign */                               \
    /**/

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            extern void* enabler;
            template<typename ...T> void ignore(T &&...);
            struct not_a_generator;
            struct not_a_grammar;
            struct not_a_domain;
            template<typename T>
            struct unrefwrap;
            template<typename T>
            using as_arg = typename unrefwrap<typename std::decay<T>::type>::type;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operator tags
        namespace tagns
        {
            namespace tag
            {
                struct terminal;
                struct unary_plus;
                struct negate;
                struct dereference;
                struct complement;
                struct address_of;
                struct logical_not;
                struct pre_inc;
                struct pre_dec;
                struct post_inc;
                struct post_dec;

                struct shift_left;
                struct shift_right;
                struct multiplies;
                struct divides;
                struct modulus;
                struct plus;
                struct minus;
                struct less;
                struct greater;
                struct less_equal;
                struct greater_equal;
                struct equal_to;
                struct not_equal_to;
                struct logical_or;
                struct logical_and;
                struct bitwise_and;
                struct bitwise_or;
                struct bitwise_xor;
                struct comma;
                struct mem_ptr;

                struct assign;
                struct shift_left_assign;
                struct shift_right_assign;
                struct multiplies_assign;
                struct divides_assign;
                struct modulus_assign;
                struct plus_assign;
                struct minus_assign;
                struct bitwise_and_assign;
                struct bitwise_or_assign;
                struct bitwise_xor_assign;
                struct subscript;
                struct member;
                struct if_else_;
                struct function;

                // Fusion tags
                struct proto_expr;
                struct proto_expr_iterator;
                struct proto_flat_view;
            }
        }

        using namespace tagns;

        namespace wildcardns
        {
            struct _;
        }

        using namespace wildcardns;

        ////////////////////////////////////////////////////////////////////////////////////////////////
        struct identity;

        template<typename Fun>
        struct static_cast_with;

        template<template<class Expr> class Extends>
        struct expr_wrap;

        template<typename ConvertFun = identity, typename CastFun = static_cast_with<ConvertFun>>
        struct generator;

        struct default_generator;

        namespace domainns
        {
            typedef detail::not_a_domain no_super_domain;

            template<
                typename Generator  = default_generator
              , typename Grammar    = _
              , typename Super      = no_super_domain
            >
            struct domain;

            struct default_domain;
        }

        using namespace domainns;

        template<typename T>
        struct is_expr;

        namespace exprns
        {
            template<typename T>
            struct term;

            template<typename ...T>
            struct args;

            template<typename Tag, typename Args>
            struct expr;

            template<typename T>
            using literal = expr<tag::terminal, term<T>>;
        }

        using exprns::args;
        using exprns::term;
        using exprns::expr;
        using exprns::literal;
    }
}

#endif
