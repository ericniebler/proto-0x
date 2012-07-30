////////////////////////////////////////////////////////////////////////////////////////////////////
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
//#include <limits>
#include <climits>
#include <boost/preprocessor/cat.hpp>

// Usage: auto fun(T t) BOOST_PROTO_AUTO_RETURN( some-expression )
//   noexcept clause from Dave Abrahams, tweaked by me.
#define BOOST_PROTO_AUTO_RETURN(...)                                                                \
    noexcept(noexcept(                                                                              \
        decltype(__VA_ARGS__)(boost::proto::detail::move_declval<decltype(__VA_ARGS__)>())          \
    )) -> decltype(__VA_ARGS__)                                                                     \
    {                                                                                               \
        return (__VA_ARGS__);                                                                       \
    }                                                                                               \
    /**/

#define BOOST_PROTO_RETURN(...)                                                                     \
    noexcept(noexcept(                                                                              \
        decltype(__VA_ARGS__)(boost::proto::detail::move_declval<decltype(__VA_ARGS__)>())          \
    ))                                                                                              \
    {                                                                                               \
        return (__VA_ARGS__);                                                                       \
    }                                                                                               \
    /**/

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF(...)                                                                  \
    typename std::enable_if<static_cast<bool>(__VA_ARGS__)>::type *& = boost::proto::detail::enabler

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF_VALID_EXPR(...)                                                       \
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

#define BOOST_PROTO_IGNORE_UNUSED(...)                                                              \
    static const struct BOOST_PP_CAT(boost_proto_ignore_unused_, __LINE__)                          \
    {                                                                                               \
        explicit BOOST_PP_CAT(boost_proto_ignore_unused_, __LINE__)(int)                            \
        {}                                                                                          \
    } BOOST_PP_CAT(boost_proto_ignore_unused_var_, __LINE__){                                       \
        (boost::proto::utility::ignore(__VA_ARGS__), 0)                                             \
    }                                                                                               \
    /**/

namespace boost
{
    namespace proto
    {
        struct _;

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // is_base_of
            template<typename T, typename U>
            struct is_base_of;

            ////////////////////////////////////////////////////////////////////////////////////////
            // any
            struct any;

            ////////////////////////////////////////////////////////////////////////////////////////
            // static_const
            template<typename T>
            struct static_const;
        }

        namespace detail
        {
            typedef char yes_type;
            typedef char (&no_type)[2];

            ////////////////////////////////////////////////////////////////////////////////////////
            // rvalue_type
            template<typename T>
            struct rvalue_type
            {
                typedef T && type;
            };

            template<typename T>
            struct rvalue_type<T &>
            {
                typedef T && type;
            };

            template<>
            struct rvalue_type<void>
            {
                typedef void type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // a declval+move that allows void
            template<typename T>
            typename rvalue_type<T>::type move_declval() noexcept;

            ////////////////////////////////////////////////////////////////////////////////////////
            // sized_type
            template<int N>
            struct sized_type
            {
                typedef char (&type)[N];
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // for use by BOOST_PROTO_ENABLE_IF
            extern void* enabler;

            struct not_a_grammar;
            struct not_a_domain;

            struct _eval;

            struct any_terminal;
            struct any_non_terminal;

            template<typename Tag>
            struct nullary_tag { typedef Tag type; };

            template<>
            struct nullary_tag<proto::_> { typedef any_terminal type; };

            template<typename Tag>
            struct unary_tag { typedef Tag type; };

            template<>
            struct unary_tag<proto::_> { typedef any_non_terminal type; };
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operator tags
        namespace tags
        {
            template<typename Tag, typename IsTerminal = std::false_type>
            struct def;

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

                // Transform environment tags
                struct data_type;
                struct local_type;
            }
        }

        using namespace tags;

        namespace domains
        {
            typedef detail::not_a_domain no_super_domain;

            struct deduce_domain;

            struct default_grammar;

            template<
                typename SubDomain
              , typename Grammar        = default_grammar
              , typename SuperDomain    = no_super_domain
            >
            struct domain;

            struct default_domain;

            struct basic_default_domain;

            template<template<typename...> class Expr, typename Domain = void>
            struct make_custom_expr;

            namespace functional
            {
                template<typename Domain>
                struct make_expr;

                template<typename Domain>
                struct as_expr;
            }

            namespace result_of
            {
                template<typename Domain, typename Tag, typename ...T>
                struct make_expr;

                template<typename Domain, typename T>
                struct as_expr;
            }
        }

        using domains::no_super_domain;
        using domains::deduce_domain;
        using domains::default_grammar;
        using domains::domain;
        using domains::default_domain;
        using domains::basic_default_domain;
        using domains::make_custom_expr;

        namespace functional
        {
            struct make_expr;

            struct as_expr;
        }

        namespace result_of
        {
            template<typename Tag, typename ...T>
            struct make_expr;

            template<typename T>
            struct as_expr;
        }

        namespace exprs
        {
            template<typename ...T>
            struct args;

            template<std::size_t I, typename Args>
            struct args_element;

            template<typename Expr, typename Domain>
            struct expr_assign;

            template<typename Expr, typename Domain>
            struct expr_subscript;

            template<typename Expr, typename Domain>
            struct expr_function;

            struct expr_base;

            template<typename Tag, typename Args, typename Domain>
            struct basic_expr;

            template<typename Tag, typename Args, typename Domain = default_domain>
            struct expr;

            template<typename Tag, typename Args, typename Domain>
            struct virtual_member_;

            template<typename Expr>
            struct virtual_;
        }

        using exprs::args;
        using exprs::args_element;
        using exprs::expr_assign;
        using exprs::expr_subscript;
        using exprs::expr_function;
        using exprs::expr_base;
        using exprs::basic_expr;
        using exprs::expr;

        template<typename This, typename Value, typename Domain = default_domain>
        using virtual_member =
            exprs::virtual_member_<
                tag::member
              , args<exprs::virtual_<This>, expr<tag::terminal, args<Value>>>
              , Domain
            >;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // stuff for transforms here
        namespace envs
        {
            struct key_not_found;

            struct empty_env;

            template<typename Tag, typename Value, typename Base = empty_env>
            struct env;
        }

        using envs::key_not_found;
        using envs::empty_env;
        using envs::env;

        struct transform_base;

        template<typename Transform>
        struct transform;

        template<typename T, int I = 0>
        struct _protect;

        struct _expr;

        struct _state;

        template<typename Tag>
        struct _env_var;

        struct _env;

        typedef _env_var<tag::data_type> _data;

        struct _value;

        template<std::size_t N>
        struct _child;

        typedef _child<0> _left;
        typedef _child<1> _right;

        template<typename T>
        struct is_transform;

        template<typename T, int = 0>
        struct as_transform;

        template<typename Grammar, typename Transform = Grammar>
        struct when;

        template<typename Transform>
        using otherwise = when<_, Transform>;

        template<typename T, T Value>
        struct _constant;

        template<int I>
        using _int = _constant<int, I>;

        template<std::size_t N>
        using _size_t = _constant<std::size_t, N>;

        template<typename Expr>
        struct _pass_through;

        template<typename Grammar = detail::_eval>
        struct _eval;

        template<typename Seq, typename State0, typename Fun>
        struct _fold;

        template<typename Seq, typename State0, typename Fun>
        struct _recursive_fold;

        template<typename Seq, typename State0, typename Fun>
        struct _reverse_fold;

        template<typename Seq, typename State0, typename Fun>
        struct _reverse_recursive_fold;
        
        template<typename T>
        struct noinvoke;

        struct _deep_copy;

        struct _tag_of;

        struct _arity_of;

        struct _domain_of;

        struct pack;

        struct apply;

        struct construct;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Misc. traits
        template<typename T>
        struct is_expr;

        template<typename Expr>
        struct is_terminal;

        template<typename Expr>
        struct arity_of;

        template<typename Expr>
        struct domain_of;

        template<typename Expr>
        struct tag_of;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Stuff for grammar building
        template<typename T>
        struct exact;

        template<typename T>
        struct convertible_to;

        template<typename Grammar>
        struct vararg;

        // Boost bug https://svn.boost.org/trac/boost/ticket/4602
        //int const N = INT_MAX;
        //constexpr int N = (std::numeric_limits<int>::max() >> 10);
        constexpr int N = (INT_MAX >> 10);

        template<typename... Grammar>
        struct or_;

        template<typename Grammar>
        struct not_;

        template<typename... Grammar>
        struct and_;

        template<typename If, typename Then = _, typename Else = not_<_>>
        struct if_;

        template<typename Cases, typename Transform = _tag_of>
        struct switch_;

        template<typename Expr, typename Grammar>
        struct matches;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Handy expression template aliases
        template<typename Tag, typename T>
        using nullary_expr = expr<typename detail::nullary_tag<Tag>::type, args<T>>;

        template<typename Tag, typename T>
        using unary_expr = expr<typename detail::unary_tag<Tag>::type, args<T>>;

        template<typename Tag, typename L, typename R>
        using binary_expr = expr<Tag, args<L, R>>;

        template<typename Tag, typename ...T>
        using nary_expr = expr<Tag, args<T...>>;

        template<typename T>
        using terminal = expr<tag::terminal, args<T>>;

        template<typename T>
        using unary_plus = expr<tag::unary_plus, args<T>>;

        template<typename T>
        using negate = expr<tag::negate, args<T>>;

        template<typename T>
        using dereference = expr<tag::dereference, args<T>>;

        template<typename T>
        using complement = expr<tag::complement, args<T>>;

        template<typename T>
        using address_of = expr<tag::address_of, args<T>>;

        template<typename T>
        using logical_not = expr<tag::logical_not, args<T>>;

        template<typename T>
        using pre_inc = expr<tag::pre_inc, args<T>>;

        template<typename T>
        using pre_dec = expr<tag::pre_dec, args<T>>;

        template<typename T>
        using post_inc = expr<tag::post_inc, args<T>>;

        template<typename T>
        using post_dec = expr<tag::post_dec, args<T>>;

        template<typename L, typename R>
        using shift_left = expr<tag::shift_left, args<L, R>>;

        template<typename L, typename R>
        using shift_right = expr<tag::shift_right, args<L, R>>;

        template<typename L, typename R>
        using multiplies = expr<tag::multiplies, args<L, R>>;

        template<typename L, typename R>
        using divides = expr<tag::divides, args<L, R>>;

        template<typename L, typename R>
        using modulus = expr<tag::modulus, args<L, R>>;

        template<typename L, typename R>
        using plus = expr<tag::plus, args<L, R>>;

        template<typename L, typename R>
        using minus = expr<tag::minus, args<L, R>>;

        template<typename L, typename R>
        using less = expr<tag::less, args<L, R>>;

        template<typename L, typename R>
        using greater = expr<tag::greater, args<L, R>>;

        template<typename L, typename R>
        using less_equal = expr<tag::less_equal, args<L, R>>;

        template<typename L, typename R>
        using greater_equal = expr<tag::greater_equal, args<L, R>>;

        template<typename L, typename R>
        using equal_to = expr<tag::equal_to, args<L, R>>;

        template<typename L, typename R>
        using not_equal_to = expr<tag::not_equal_to, args<L, R>>;

        template<typename L, typename R>
        using logical_or = expr<tag::logical_or, args<L, R>>;

        template<typename L, typename R>
        using logical_and = expr<tag::logical_and, args<L, R>>;

        template<typename L, typename R>
        using bitwise_and = expr<tag::bitwise_and, args<L, R>>;

        template<typename L, typename R>
        using bitwise_or = expr<tag::bitwise_or, args<L, R>>;

        template<typename L, typename R>
        using bitwise_xor = expr<tag::bitwise_xor, args<L, R>>;

        template<typename L, typename R>
        using comma = expr<tag::comma, args<L, R>>;

        template<typename L, typename R>
        using mem_ptr = expr<tag::mem_ptr, args<L, R>>;

        template<typename L, typename R>
        using assign = expr<tag::assign, args<L, R>>;

        template<typename L, typename R>
        using shift_left_assign = expr<tag::shift_left_assign, args<L, R>>;

        template<typename L, typename R>
        using shift_right_assign = expr<tag::shift_right_assign, args<L, R>>;

        template<typename L, typename R>
        using multiplies_assign = expr<tag::multiplies_assign, args<L, R>>;

        template<typename L, typename R>
        using divides_assign = expr<tag::divides_assign, args<L, R>>;

        template<typename L, typename R>
        using modulus_assign = expr<tag::modulus_assign, args<L, R>>;

        template<typename L, typename R>
        using plus_assign = expr<tag::plus_assign, args<L, R>>;

        template<typename L, typename R>
        using minus_assign = expr<tag::minus_assign, args<L, R>>;

        template<typename L, typename R>
        using bitwise_and_assign = expr<tag::bitwise_and_assign, args<L, R>>;

        template<typename L, typename R>
        using bitwise_or_assign = expr<tag::bitwise_or_assign, args<L, R>>;

        template<typename L, typename R>
        using bitwise_xor_assign = expr<tag::bitwise_xor_assign, args<L, R>>;

        template<typename L, typename R>
        using subscript = expr<tag::subscript, args<L, R>>;

        template<typename L, typename R>
        using member = expr<tag::member, args<L, R>>;

        template<typename C, typename T, typename F>
        using if_else_ = expr<tag::if_else_, args<C, T, F>>;

        template<typename ...A>
        using function = expr<tag::function, args<A...>>;

        template<template<typename...> class Expr, typename Domain = void>
        struct custom;

        namespace functional
        {
            struct deep_copy;
        }
    }
}

#endif
