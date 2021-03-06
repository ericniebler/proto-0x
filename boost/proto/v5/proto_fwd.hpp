////////////////////////////////////////////////////////////////////////////////////////////////////
// proto_fwd.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_FWD_HPP_INCLUDED
#define BOOST_PROTO_V5_FWD_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <type_traits>
//#include <limits>
#include <climits>
#include <boost/preprocessor/cat.hpp>

// Usage: auto fun(T t) BOOST_PROTO_AUTO_RETURN( some-expression )
//   noexcept clause from Dave Abrahams, tweaked by me.
#define BOOST_PROTO_AUTO_RETURN(...)                                                                \
    noexcept(noexcept(                                                                              \
        decltype(__VA_ARGS__)(boost::proto::v5::utility::move_declval<decltype(__VA_ARGS__)>())     \
    )) -> decltype(__VA_ARGS__)                                                                     \
    {                                                                                               \
        return (__VA_ARGS__);                                                                       \
    }                                                                                               \
    /**/

#define BOOST_PROTO_RETURN(...)                                                                     \
    noexcept(noexcept(                                                                              \
        decltype(__VA_ARGS__)(boost::proto::v5::utility::move_declval<decltype(__VA_ARGS__)>())     \
    ))                                                                                              \
    {                                                                                               \
        return (__VA_ARGS__);                                                                       \
    }                                                                                               \
    /**/

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF(...)                                                                  \
    typename std::enable_if<static_cast<bool>(__VA_ARGS__)>::type *const & =                        \
        boost::proto::v5::utility::static_const<void *>::value

// New-style enable_if from Matt Calabrese
#define BOOST_PROTO_ENABLE_IF_VALID_EXPR(...)                                                       \
    decltype(static_cast<void>(__VA_ARGS__)) *const & =                                             \
        boost::proto::v5::utility::static_const<void *>::value

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
    } BOOST_PP_CAT(boost_proto_ignore_unused_obj_, __LINE__)                                        \
    {                                                                                               \
        (boost::proto::v5::utility::ignore(__VA_ARGS__), 0)                                         \
    }                                                                                               \
    /**/

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            struct _;

            namespace detail
            {
                struct not_a_grammar;
                struct not_a_domain;

                struct _eval;

                struct empty
                {};

                struct empty_state
                {};

                struct empty_data
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // rvalue_type
                template<typename T>
                struct rvalue_type
                {
                    using type = T &&;
                };

                template<>
                struct rvalue_type<void>
                {
                    using type = void;
                };

                template<typename Ignored = decltype(nullptr)>
                struct any;

                template<typename Expr>
                struct flat_view;

                template<typename T>
                struct as_grammar_impl_;

                template<typename T>
                using as_grammar_ = typename as_grammar_impl_<T>::type;

                template<typename T>
                struct as_action_impl_;

                template<typename T>
                using as_action_ = typename as_action_impl_<T>::type;

                template<typename Expr, typename Domain>
                struct as_basic_expr_;

                template<typename Expr, typename Domain>
                struct as_expr_;

                struct def_base;

                template<typename First, typename Second>
                struct compressed_pair;

                template<typename>
                struct default_expr;

                template<typename T>
                struct name_of_;

                template<template<typename...> class T>
                struct template_name_;
            }

            namespace utility
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // is_base_of
                template<typename T, typename U>
                struct is_base_of;

                ////////////////////////////////////////////////////////////////////////////////////
                // any
                using any = detail::any<>;

                ////////////////////////////////////////////////////////////////////////////////////
                // static_const
                template<typename T>
                struct static_const
                {
                    static constexpr T value{};
                };

                template<typename T>
                constexpr T static_const<T>::value;

                constexpr void *const &enabler = static_const<void *>::value;

                ////////////////////////////////////////////////////////////////////////////////////
                // a declval+move that allows void
                template<typename T>
                typename detail::rvalue_type<T>::type move_declval() noexcept;

                ////////////////////////////////////////////////////////////////////////////////////
                // sized_type
                template<int N>
                using sized_type = char(&)[N];

                ////////////////////////////////////////////////////////////////////////////////////
                // identity
                struct identity;

                ////////////////////////////////////////////////////////////////////////////////////
                // substitution_failure
                template<typename Sig>
                struct substitution_failure;
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // Operator tags
            namespace tags
            {
                struct nullary;
                struct unary;
                struct binary;
                struct ternary;
                struct nary;

                struct expr_tag_tag;
                struct expr_tag_base;

                template<typename Tag, typename Arity, typename Action = detail::empty>
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

                // Fusion tags
                template<typename Tag, typename Domain> struct proto_expr;
                template<typename Tag, typename Domain> struct proto_expr_iterator;
                template<typename Tag, typename Domain> struct proto_flat_view;
            }

            using namespace tags;

            template<typename Tag>
            struct env_tag;

            // Action environment tags
            struct data_tag;

            struct or_;
            struct not_;
            struct and_;
            struct if_;
            struct switch_;
            struct case_;
            struct default_;
            struct match;
            struct matches_;
            struct passthru;
            struct block;

            using then = block;
            using else_ = block;

            namespace result_of
            {
                template<typename Expr>
                struct tag_of;

                template<typename Expr>
                struct arity_of;

                template<typename Expr>
                struct domain_of;
            }

            template<typename T>
            struct is_tag;

            template<typename T>
            struct is_domain;

            namespace domains
            {
                using no_super_domain = detail::not_a_domain;

                struct deduce_domain;

                struct default_grammar;

                template<
                    typename SubDomain
                  , typename Grammar        = default_grammar
                  , typename SuperDomain    = no_super_domain
                >
                struct domain;

                template<typename BaseDomain>
                struct safe_domain_adaptor;

                template<typename BaseDomain>
                struct basic_expr_domain_adaptor;

                template<typename BaseDomain, typename CustomExpr>
                struct custom_expr_domain_adaptor;

                struct basic_default_domain;

                struct default_domain;

                template<typename Expr>
                struct make_custom_expr;

                using default_make_expr = make_custom_expr<detail::default_expr<_>>;
            }

            using domains::no_super_domain;
            using domains::deduce_domain;
            using domains::default_grammar;
            using domains::domain;
            using domains::default_domain;
            using domains::basic_default_domain;
            using domains::make_custom_expr;
            using domains::default_make_expr;

            namespace functional
            {
                template<typename Domain = default_domain>
                struct as_expr;

                template<typename TagOrDomain = void, typename DomainOrVoid = void>
                struct make_expr;

                template<typename TagOrDomain = void, typename DomainOrVoid = void>
                struct unpack_expr;
            }

            namespace result_of
            {
                template<typename T, typename Domain = default_domain>
                struct as_expr;

                template<typename ExprDesc, typename Domain = deduce_domain>
                struct make_expr;

                template<typename Tag, typename Seq, typename Domain = deduce_domain>
                struct unpack_expr;
            }

            namespace exprs
            {
                template<typename ...T>
                struct children;

                template<std::size_t I, typename Children>
                struct children_element;

                template<typename Expr>
                struct expr_assign;

                template<typename Expr>
                struct expr_subscript;

                template<typename Expr>
                struct expr_function;

                struct expr_base;

                template<typename ExprDesc, typename Domain = basic_default_domain>
                struct basic_expr;

                template<typename ExprDesc, typename Domain = default_domain>
                struct expr;

                template<typename ExprDesc>
                struct virtual_member_;

                template<typename Expr>
                struct virtual_;

                template<typename Tag, typename ...Children, typename Domain>
                constexpr Tag &tag_of(basic_expr<Tag(Children...), Domain> &that) noexcept;

                template<typename Tag, typename ...Children, typename Domain>
                constexpr Tag const &tag_of(basic_expr<Tag(Children...), Domain> const &that) noexcept;

                template<typename Tag, typename ...Children, typename Domain>
                constexpr Tag &&tag_of(basic_expr<Tag(Children...), Domain> &&that) noexcept;
            }

            using exprs::expr_assign;
            using exprs::expr_subscript;
            using exprs::expr_function;
            using exprs::expr_base;
            using exprs::tag_of;

            template<typename ExprDesc, typename Domain = basic_default_domain>
            using basic_expr = typename detail::as_basic_expr_<ExprDesc, Domain>::type;

            template<typename ExprDesc, typename Domain = default_domain>
            using expr = typename detail::as_expr_<ExprDesc, Domain>::type;

            template<typename This, typename Value>
            using virtual_member =
                exprs::virtual_member_<member(exprs::virtual_<This>, exprs::expr<terminal(Value)>)>;

            ////////////////////////////////////////////////////////////////////////////////////////
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

            struct _env;

            struct _data;

            struct _value;

            template<std::size_t N>
            struct _child;

            using _left   = _child<0>;
            using _right  = _child<1>;

            using _child0 = _child<0>;
            using _child1 = _child<1>;
            using _child2 = _child<2>;
            using _child3 = _child<3>;
            using _child4 = _child<4>;
            using _child5 = _child<5>;
            using _child6 = _child<6>;
            using _child7 = _child<7>;
            using _child8 = _child<8>;
            using _child9 = _child<9>;

            template<typename T>
            struct is_action;

            template<typename T, T Value>
            struct _integral_constant;

            template<int I>
            using _int = _integral_constant<int, I>;

            template<std::size_t N>
            using _size_t = _integral_constant<std::size_t, N>;

            struct _eval;

            struct eval_with;

            template<typename T>
            struct noinvoke;

            struct _deep_copy;

            struct _tag_of;

            struct _arity_of;

            struct _domain_of;

            template<typename Local>
            struct _local;

            struct _noop;

            struct pack;

            struct apply;

            struct make;

            struct call;

            struct let;

            struct has_env;
            struct get_env;
            struct push_env;
            struct set_env;

            template<int ...Is>
            struct _string;

            struct fold;
            struct reverse_fold;
            struct recursive_fold;
            struct reverse_recursive_fold;

            struct everywhere;
            struct everything;
            struct nothing;

            struct trace;

            struct external;

            ////////////////////////////////////////////////////////////////////////////////////////
            // Misc. traits
            template<typename T>
            struct is_expr;

            template<typename Expr>
            struct is_terminal;

            ////////////////////////////////////////////////////////////////////////////////////////
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

            template<typename T>
            struct def;

            struct grammar_base;

            template<typename Grammar>
            struct basic_grammar;

            template<typename Grammar>
            struct is_grammar;

            namespace extension
            {
                template<typename Grammar, typename Enable = void>
                struct grammar_impl;

                template<typename Grammar, typename Enable = void>
                struct action_impl;
            }

            namespace result_of
            {
                template<typename Expr, typename Grammar>
                struct matches;
            }

            template<typename Expr, typename Grammar>
            constexpr bool matches() noexcept;

            namespace exprs
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // Handy expression template aliases
                template<typename T>
                using terminal = expr<tags::terminal(T)>;

                template<typename T>
                using unary_plus = expr<tags::unary_plus(T)>;

                template<typename T>
                using negate = expr<tags::negate(T)>;

                template<typename T>
                using dereference = expr<tags::dereference(T)>;

                template<typename T>
                using complement = expr<tags::complement(T)>;

                template<typename T>
                using address_of = expr<tags::address_of(T)>;

                template<typename T>
                using logical_not = expr<tags::logical_not(T)>;

                template<typename T>
                using pre_inc = expr<tags::pre_inc(T)>;

                template<typename T>
                using pre_dec = expr<tags::pre_dec(T)>;

                template<typename T>
                using post_inc = expr<tags::post_inc(T)>;

                template<typename T>
                using post_dec = expr<tags::post_dec(T)>;

                template<typename L, typename R>
                using shift_left = expr<tags::shift_left(L, R)>;

                template<typename L, typename R>
                using shift_right = expr<tags::shift_right(L, R)>;

                template<typename L, typename R>
                using multiplies = expr<tags::multiplies(L, R)>;

                template<typename L, typename R>
                using divides = expr<tags::divides(L, R)>;

                template<typename L, typename R>
                using modulus = expr<tags::modulus(L, R)>;

                template<typename L, typename R>
                using plus = expr<tags::plus(L, R)>;

                template<typename L, typename R>
                using minus = expr<tags::minus(L, R)>;

                template<typename L, typename R>
                using less = expr<tags::less(L, R)>;

                template<typename L, typename R>
                using greater = expr<tags::greater(L, R)>;

                template<typename L, typename R>
                using less_equal = expr<tags::less_equal(L, R)>;

                template<typename L, typename R>
                using greater_equal = expr<tags::greater_equal(L, R)>;

                template<typename L, typename R>
                using equal_to = expr<tags::equal_to(L, R)>;

                template<typename L, typename R>
                using not_equal_to = expr<tags::not_equal_to(L, R)>;

                template<typename L, typename R>
                using logical_or = expr<tags::logical_or(L, R)>;

                template<typename L, typename R>
                using logical_and = expr<tags::logical_and(L, R)>;

                template<typename L, typename R>
                using bitwise_and = expr<tags::bitwise_and(L, R)>;

                template<typename L, typename R>
                using bitwise_or = expr<tags::bitwise_or(L, R)>;

                template<typename L, typename R>
                using bitwise_xor = expr<tags::bitwise_xor(L, R)>;

                template<typename L, typename R>
                using comma = expr<tags::comma(L, R)>;

                template<typename L, typename R>
                using mem_ptr = expr<tags::mem_ptr(L, R)>;

                template<typename L, typename R>
                using assign = expr<tags::assign(L, R)>;

                template<typename L, typename R>
                using shift_left_assign = expr<tags::shift_left_assign(L, R)>;

                template<typename L, typename R>
                using shift_right_assign = expr<tags::shift_right_assign(L, R)>;

                template<typename L, typename R>
                using multiplies_assign = expr<tags::multiplies_assign(L, R)>;

                template<typename L, typename R>
                using divides_assign = expr<tags::divides_assign(L, R)>;

                template<typename L, typename R>
                using modulus_assign = expr<tags::modulus_assign(L, R)>;

                template<typename L, typename R>
                using plus_assign = expr<tags::plus_assign(L, R)>;

                template<typename L, typename R>
                using minus_assign = expr<tags::minus_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_and_assign = expr<tags::bitwise_and_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_or_assign = expr<tags::bitwise_or_assign(L, R)>;

                template<typename L, typename R>
                using bitwise_xor_assign = expr<tags::bitwise_xor_assign(L, R)>;

                template<typename L, typename R>
                using subscript = expr<tags::subscript(L, R)>;

                template<typename L, typename R>
                using member = expr<tags::member(L, R)>;

                template<typename C, typename T, typename F>
                using if_else_ = expr<tags::if_else_(C, T, F)>;

                template<typename ...A>
                using function = expr<tags::function(A...)>;
            }

            template<typename T>
            using literal = exprs::terminal<T>;

            template<typename Expr>
            struct custom;

            namespace functional
            {
                struct deep_copy;

                struct display_expr;
            }
        }
    }
}

#endif
