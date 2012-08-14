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
        decltype(__VA_ARGS__)(boost::proto::utility::move_declval<decltype(__VA_ARGS__)>())         \
    )) -> decltype(__VA_ARGS__)                                                                     \
    {                                                                                               \
        return (__VA_ARGS__);                                                                       \
    }                                                                                               \
    /**/

#define BOOST_PROTO_RETURN(...)                                                                     \
    noexcept(noexcept(                                                                              \
        decltype(__VA_ARGS__)(boost::proto::utility::move_declval<decltype(__VA_ARGS__)>())         \
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

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // for use by BOOST_PROTO_ENABLE_IF
            extern void* enabler;

            struct not_a_grammar;
            struct not_a_domain;

            struct _eval;

            struct empty_state
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // rvalue_type
            template<typename T>
            struct rvalue_type
            {
                typedef T && type;
            };

            template<>
            struct rvalue_type<void>
            {
                typedef void type;
            };
        }

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

            ////////////////////////////////////////////////////////////////////////////////////////
            // a declval+move that allows void
            template<typename T>
            typename detail::rvalue_type<T>::type move_declval() noexcept;

            ////////////////////////////////////////////////////////////////////////////////////////
            // sized_type
            template<int N>
            struct sized_type;

            ////////////////////////////////////////////////////////////////////////////////////////
            // sfinae_error
            template<typename Sig>
            struct sfinae_error;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operator tags
        namespace tags
        {
            struct expr_tag_base;

            template<typename Tag, typename IsTerminal = std::false_type>
            struct expr_tag;

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

            struct nullary_expr;
            struct unary_expr;
            struct binary_expr;
            struct nary_expr;

            // Fusion tags
            struct proto_expr;
            struct proto_expr_iterator;
            struct proto_flat_view;

            template<typename Tag>
            struct env_var_tag;

            // Action environment tags
            struct data_type;
        }

        using namespace tags;

        struct or_;
        struct not_;
        struct and_;
        struct if_;
        struct switch_;
        struct when;
        struct pass_through;
        struct match;

        typedef and_ block;
        typedef block then;
        typedef block else_;

        template<typename Expr>
        struct tag_of;

        template<typename T>
        struct is_tag;

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

            template<typename Key>
            struct env_var;
        }

        namespace result_of
        {
            template<typename Tag, typename ...T>
            struct make_expr;

            template<typename T>
            struct as_expr;

            template<typename Env, typename Key>
            struct env_var;
        }

        namespace exprs
        {
            template<typename ...T>
            struct children;

            template<std::size_t I, typename Children>
            struct children_element;

            template<typename Expr, typename Domain>
            struct expr_assign;

            template<typename Expr, typename Domain>
            struct expr_subscript;

            template<typename Expr, typename Domain>
            struct expr_function;

            struct expr_base;

            template<typename Tag, typename Children, typename Domain>
            struct basic_expr;

            template<typename Tag, typename Children, typename Domain = default_domain>
            struct expr;

            template<typename Tag, typename Children, typename Domain>
            struct virtual_member_;

            template<typename Expr>
            struct virtual_;
        }

        using exprs::children;
        using exprs::children_element;
        using exprs::expr_assign;
        using exprs::expr_subscript;
        using exprs::expr_function;
        using exprs::expr_base;
        using exprs::basic_expr;
        using exprs::expr;

        template<typename This, typename Value, typename Domain = default_domain>
        using virtual_member =
            exprs::virtual_member_<
                member
              , children<exprs::virtual_<This>, expr<terminal, children<Value>>>
              , Domain
            >;

        ////////////////////////////////////////////////////////////////////////////////////////////
        // stuff for actions here
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

        struct action_base;

        template<typename Action>
        struct basic_action;

        template<typename T, int I = 0>
        struct _protect;

        struct _expr;

        struct _state;

        template<typename Tag>
        struct _env_var;

        struct _env;

        typedef _env_var<data_type> _data;

        struct _value;

        template<std::size_t N>
        struct _child;

        typedef _child<0> _left;
        typedef _child<1> _right;

        template<typename T>
        struct is_action;

        template<typename T, typename Enable = void>
        struct action;

        template<typename T>
        struct active_grammar;

        template<typename T, T Value>
        struct _integral_constant;

        template<int I>
        using _int = _integral_constant<int, I>;

        template<std::size_t N>
        using _size_t = _integral_constant<std::size_t, N>;

        template<typename Grammar = detail::_eval>
        struct _eval;

        template<typename T>
        struct noinvoke;

        struct _deep_copy;

        struct _tag_of;

        struct _arity_of;

        struct _domain_of;

        template<typename Local>
        struct _local;

        struct pack;

        struct apply;

        struct construct;

        struct let;

        struct fold;
        struct reverse_fold;
        struct recursive_fold;
        struct reverse_recursive_fold;

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Stuff for grammar building
        template<typename T>
        struct fuzzy;

        template<typename T>
        struct exact;

        template<typename T>
        struct convertible_to;

        // Boost bug https://svn.boost.org/trac/boost/ticket/4602
        //int const N = INT_MAX;
        //constexpr int N = (std::numeric_limits<int>::max() >> 10);
        constexpr int N = (INT_MAX >> 10);

        template<typename Grammar>
        struct grammar;

        template<typename Grammar>
        struct grammar_of;

        template<typename Expr, typename Grammar, typename Enable = void>
        struct matches;

        namespace exprs
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            // Handy expression template aliases
            template<typename Tag, typename T>
            using nullary_expr = expr<Tag, children<T>>;

            template<typename Tag, typename T>
            using unary_expr = expr<Tag, children<T>>;

            template<typename Tag, typename L, typename R>
            using binary_expr = expr<Tag, children<L, R>>;

            template<typename Tag, typename ...T>
            using nary_expr = expr<Tag, children<T...>>;

            template<typename T>
            using terminal = expr<tags::terminal, children<T>>;

            template<typename T>
            using unary_plus = expr<tags::unary_plus, children<T>>;

            template<typename T>
            using negate = expr<tags::negate, children<T>>;

            template<typename T>
            using dereference = expr<tags::dereference, children<T>>;

            template<typename T>
            using complement = expr<tags::complement, children<T>>;

            template<typename T>
            using address_of = expr<tags::address_of, children<T>>;

            template<typename T>
            using logical_not = expr<tags::logical_not, children<T>>;

            template<typename T>
            using pre_inc = expr<tags::pre_inc, children<T>>;

            template<typename T>
            using pre_dec = expr<tags::pre_dec, children<T>>;

            template<typename T>
            using post_inc = expr<tags::post_inc, children<T>>;

            template<typename T>
            using post_dec = expr<tags::post_dec, children<T>>;

            template<typename L, typename R>
            using shift_left = expr<tags::shift_left, children<L, R>>;

            template<typename L, typename R>
            using shift_right = expr<tags::shift_right, children<L, R>>;

            template<typename L, typename R>
            using multiplies = expr<tags::multiplies, children<L, R>>;

            template<typename L, typename R>
            using divides = expr<tags::divides, children<L, R>>;

            template<typename L, typename R>
            using modulus = expr<tags::modulus, children<L, R>>;

            template<typename L, typename R>
            using plus = expr<tags::plus, children<L, R>>;

            template<typename L, typename R>
            using minus = expr<tags::minus, children<L, R>>;

            template<typename L, typename R>
            using less = expr<tags::less, children<L, R>>;

            template<typename L, typename R>
            using greater = expr<tags::greater, children<L, R>>;

            template<typename L, typename R>
            using less_equal = expr<tags::less_equal, children<L, R>>;

            template<typename L, typename R>
            using greater_equal = expr<tags::greater_equal, children<L, R>>;

            template<typename L, typename R>
            using equal_to = expr<tags::equal_to, children<L, R>>;

            template<typename L, typename R>
            using not_equal_to = expr<tags::not_equal_to, children<L, R>>;

            template<typename L, typename R>
            using logical_or = expr<tags::logical_or, children<L, R>>;

            template<typename L, typename R>
            using logical_and = expr<tags::logical_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and = expr<tags::bitwise_and, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or = expr<tags::bitwise_or, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor = expr<tags::bitwise_xor, children<L, R>>;

            template<typename L, typename R>
            using comma = expr<tags::comma, children<L, R>>;

            template<typename L, typename R>
            using mem_ptr = expr<tags::mem_ptr, children<L, R>>;

            template<typename L, typename R>
            using assign = expr<tags::assign, children<L, R>>;

            template<typename L, typename R>
            using shift_left_assign = expr<tags::shift_left_assign, children<L, R>>;

            template<typename L, typename R>
            using shift_right_assign = expr<tags::shift_right_assign, children<L, R>>;

            template<typename L, typename R>
            using multiplies_assign = expr<tags::multiplies_assign, children<L, R>>;

            template<typename L, typename R>
            using divides_assign = expr<tags::divides_assign, children<L, R>>;

            template<typename L, typename R>
            using modulus_assign = expr<tags::modulus_assign, children<L, R>>;

            template<typename L, typename R>
            using plus_assign = expr<tags::plus_assign, children<L, R>>;

            template<typename L, typename R>
            using minus_assign = expr<tags::minus_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_and_assign = expr<tags::bitwise_and_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_or_assign = expr<tags::bitwise_or_assign, children<L, R>>;

            template<typename L, typename R>
            using bitwise_xor_assign = expr<tags::bitwise_xor_assign, children<L, R>>;

            template<typename L, typename R>
            using subscript = expr<tags::subscript, children<L, R>>;

            template<typename L, typename R>
            using member = expr<tags::member, children<L, R>>;

            template<typename C, typename T, typename F>
            using if_else_ = expr<tags::if_else_, children<C, T, F>>;

            template<typename ...A>
            using function = expr<tags::function, children<A...>>;
        }

        template<typename T>
        using literal = exprs::terminal<T>;

        template<template<typename...> class Expr, typename Domain = void>
        struct custom;

        namespace functional
        {
            struct deep_copy;
        }
    }
}

#endif
