////////////////////////////////////////////////////////////////////////////////////////////////////
// expr.hpp
// Defines how proto::matches works with expression patterns
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_EXPR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_EXPR_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // tag_matches
            template<typename Tag0, typename Tag1>
            struct tag_matches
              : std::false_type
            {};

            template<typename Tag>
            struct tag_matches<Tag, Tag>
              : std::true_type
            {};

            template<typename Tag>
            struct tag_matches<Tag, proto::_>
              : std::true_type
            {};

            template<typename Tag>
            struct tag_matches<Tag, detail::any_terminal>
              : std::integral_constant<bool, Tag::proto_is_terminal::value>
            {};

            template<typename Tag>
            struct tag_matches<Tag, detail::any_non_terminal>
              : std::integral_constant<bool, !Tag::proto_is_terminal::value>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // array_matches
            template<typename T, typename U>
            struct array_matches
              : std::false_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T *>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T const *>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T[proto::N]>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // array_matches
            template<typename T, typename U>
            struct lambda_matches
              : std::false_type
            {};

            template<typename T>
            struct lambda_matches<T, proto::_>
              : std::true_type
            {};

            template<typename T>
            struct lambda_matches<T, T>
              : std::true_type
            {};

            template<typename T, std::size_t M, typename U>
            struct lambda_matches<T[M], U>
              : array_matches<T[M], U>
            {};

            template<typename T, std::size_t M>
            struct lambda_matches<T[M], proto::_>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct lambda_matches<T[M], T[M]>
              : std::true_type
            {};

            template<template<typename...> class T, typename ...Expr0, typename ...Grammar0>
            struct lambda_matches<T<Expr0...>, T<Grammar0...>>
              : utility::and_<lambda_matches<Expr0, Grammar0>...>
            {};

            // How value_matches_<> handles references and cv-qualifiers.
            // The cv and ref matter *only* if the grammar has a top-level ref.
            //
            // Expr       |   Grammar    |  Matches?
            // -------------------------------------
            // T              T             yes
            // T &            T             yes
            // T const &      T             yes
            // T              T &           no
            // T &            T &           yes
            // T const &      T &           no
            // T              T const &     no
            // T &            T const &     no
            // T const &      T const &     yes

            template<typename T, typename U>
            struct is_cv_ref_compatible
              : std::true_type
            {};

            template<typename T, typename U>
            struct is_cv_ref_compatible<T, U &>
              : std::false_type
            {};

            template<typename T, typename U>
            struct is_cv_ref_compatible<T &, U &>
              : std::integral_constant<bool, std::is_const<T>::value == std::is_const<U>::value>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // value_matches_
            template<typename T, typename U>
            struct value_matches_
              : utility::and_<
                    is_cv_ref_compatible<T, U>
                    // TODO: instantiate fewer templates here. Make this lazy?
                  , lambda_matches<
                        typename std::remove_const<typename std::remove_reference<T>::type>::type
                      , typename std::remove_const<typename std::remove_reference<U>::type>::type
                    >
                >
            {};

            template<typename T, std::size_t M>
            struct value_matches_<T(&)[M], T(&)[proto::N]>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct value_matches_<T(&)[M], T *>
              : std::true_type
            {};

            template<typename T>
            struct value_matches_<T, T>
              : std::true_type
            {};

            template<typename T>
            struct value_matches_<T &, T>
              : std::true_type
            {};

            template<typename T>
            struct value_matches_<T const &, T>
              : std::true_type
            {};

            template<typename T>
            struct value_matches_<T, proto::_>
              : std::true_type
            {};

            template<typename T>
            struct value_matches_<T, exact<T>>
              : std::true_type
            {};

            template<typename T, typename U>
            struct value_matches_<T, fuzzy<U>>
              : value_matches_<T, U>
            {};

            template<typename T, typename U>
            struct value_matches_<T, convertible_to<U>>
              : std::is_convertible<T, U>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // vararg_matches
            template<typename Args0, typename Args1>
            struct vararg_matches
              : std::false_type
            {};

            template<typename Head0, typename ...Tail0, typename Head1, typename ...Tail1>
            struct vararg_matches<void(Head0, Tail0...), void(Head1, Tail1...)>
              : utility::and_<
                    matches<Head0, Head1>
                  , vararg_matches<void(Tail0...), void(Tail1...)>
                >
            {};

            template<typename Head0, typename ...Tail0, typename Grammar>
            struct vararg_matches<void(Head0, Tail0...), void(Grammar)>
              : utility::and_<
                    matches<Head0, Grammar>
                  , matches<Tail0, Grammar>...
                >
            {};

            template<typename Grammar>
            struct vararg_matches<void(), void(Grammar)>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_expr_
            template<typename Expr, typename BasicExpr, typename Grammar, typename Enable = void>
            struct matches_expr_
              : std::false_type
            {};

            // Handle non-variadic non-terminal matches
            template<
                typename Expr
              , typename Tag0, typename ...Args0, typename Domain
              , typename Tag1, typename ...Args1
            >
            struct matches_expr_<
                Expr
              , basic_expr<Tag0, children<Args0...>, Domain>
              , Tag1(Args1...)
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) == sizeof...(Args1)
                >::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , matches<Args0, Args1>...
                >
            {};

            // Handle variadic non-terminal matches
            template<
                typename Expr
              , typename Tag0, typename ...Args0, typename Domain
              , typename Tag1, typename ...Args1
            >
            struct matches_expr_<
                Expr
              , basic_expr<Tag0, children<Args0...>, Domain>
              , Tag1(Args1......)
              , typename std::enable_if<!Tag0::proto_is_terminal::value>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , vararg_matches<void(Args0...), void(Args1...)>
                >
            {};

            // Handle terminal matches.
            template<
                typename Expr
              , typename Tag0, typename Value0, typename Domain
              , typename Tag1, typename Value1
            >
            struct matches_expr_<
                Expr
              , basic_expr<Tag0, children<Value0>, Domain>
              , Tag1(Value1)
              , typename std::enable_if<Tag0::proto_is_terminal::value>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , value_matches_<Value0, Value1>
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_nullary_expr_
            template<typename Expr, typename BasicExpr, typename Grammar, typename Enable = void>
            struct matches_nullary_expr_
              : std::false_type
            {};

            template<
                typename Expr
              , typename Tag0, typename Value0, typename Domain
              , typename Tag1, typename Value1
            >
            struct matches_nullary_expr_<
                Expr
              , basic_expr<Tag0, children<Value0>, Domain>
              , tag::nullary_expr(Tag1, Value1)
              , typename std::enable_if<Tag0::proto_is_terminal>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , value_matches_<Value0, Value1>
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_unary_expr_
            template<typename Expr, typename BasicExpr, typename Grammar, typename Enable = void>
            struct matches_unary_expr_
              : std::false_type
            {};

            template<
                typename Expr
              , typename Tag0, typename Child0, typename Domain
              , typename Tag1, typename Child1
            >
            struct matches_unary_expr_<
                Expr
              , basic_expr<Tag0, children<Child0>, Domain>
              , tag::unary_expr(Tag1, Child1)
              , typename std::enable_if<!Tag0::proto_is_terminal::value>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , matches<Child0, Child1>
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_binary_expr_
            template<typename Expr, typename BasicExpr, typename Grammar>
            struct matches_binary_expr_
              : std::false_type
            {};

            template<
                typename Expr
              , typename Tag0, typename Left0, typename Right0, typename Domain
              , typename Tag1, typename Left1, typename Right1
            >
            struct matches_binary_expr_<
                Expr
              , basic_expr<Tag0, children<Left0, Right0>, Domain>
              , tag::binary_expr(Tag1, Left1, Right1)
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , matches<Left0, Left1>
                  , matches<Right0, Right1>
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_nary_expr_
            template<typename Expr, typename BasicExpr, typename Grammar, typename Enable = void>
            struct matches_nary_expr_
              : std::false_type
            {};

            template<
                typename Expr
              , typename Tag0, typename ...Args0, typename Domain
              , typename Tag1, typename ...Args1
            >
            struct matches_nary_expr_<
                Expr
              , basic_expr<Tag0, children<Args0...>, Domain>
              , tag::nary_expr(Tag1, Args1...)
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) == sizeof...(Args1)
                >::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , matches<Args0, Args1>...
                >
            {};

            template<
                typename Expr
              , typename Tag0, typename ...Args0, typename Domain
              , typename Tag1, typename ...Args1
            >
            struct matches_nary_expr_<
                Expr
              , basic_expr<Tag0, children<Args0...>, Domain>
              , tag::nary_expr(Tag1, Args1......)
              , typename std::enable_if<!Tag0::proto_is_terminal::value>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , vararg_matches<void(Args0...), void(Args1...)>
                >
            {};
        }

        template<typename Expr, typename Grammar>
        struct matches<Expr, detail::expr_pattern<Grammar>, void>
          : detail::matches_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , Grammar
            >
        {};

        template<typename Expr, typename Tag, typename Value>
        struct matches<Expr, tag::nullary_expr(Tag, Value), void>
          : detail::matches_nullary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::nullary_expr(Tag, Value)
            >
        {};

        template<typename Expr, typename Tag, typename Arg>
        struct matches<Expr, tag::unary_expr(Tag, Arg), void>
          : detail::matches_unary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::unary_expr(Tag, Arg)
            >
        {};

        template<typename Expr, typename Tag, typename Arg0, typename Arg1>
        struct matches<Expr, tag::binary_expr(Tag, Arg0, Arg1), void>
          : detail::matches_binary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::binary_expr(Tag, Arg0, Arg1)
            >
        {};

        template<typename Expr, typename Tag, typename Arg0>
        struct matches<Expr, tag::binary_expr(Tag, Arg0...), void>
          : detail::matches_binary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::binary_expr(Tag, Arg0, Arg0)
            >
        {};

        template<typename Expr, typename Tag, typename ...Args>
        struct matches<Expr, tag::nary_expr(Tag, Args...), void>
          : detail::matches_nary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::nary_expr(Tag, Args...)
            >
        {};

        template<typename Expr, typename Tag, typename ...Args>
        struct matches<Expr, tag::nary_expr(Tag, Args......), void>
          : detail::matches_nary_expr_<
                Expr
              , typename std::remove_reference<Expr>::type::proto_basic_expr_type
              , tag::nary_expr(Tag, Args......)
            >
        {};
    }
}

#endif
