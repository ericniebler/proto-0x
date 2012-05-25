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
#define BOOST_PROTO_CLASS_DEFAULTS(CLASS)                                                           \
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
        namespace domainns
        {
            struct default_domain;
        }

        using domainns::default_domain;

        namespace exprns
        {
            template<typename T>
            struct term;

            template<typename ...T>
            struct args;

            template<typename Tag, typename Args>
            struct expr;
        }

        using exprns::args;
        using exprns::term;
        using exprns::expr;
    }
}

#endif
