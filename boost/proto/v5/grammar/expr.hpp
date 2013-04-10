////////////////////////////////////////////////////////////////////////////////////////////////////
// expr.hpp
// Defines how proto::matches works with expression patterns
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_GRAMMAR_EXPR_HPP_INCLUDED
#define BOOST_PROTO_CXX11_GRAMMAR_EXPR_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
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
                struct tag_matches<Tag, proto::v5::_>
                  : std::true_type
                {};

                ////////////////////////////////////////////////////////////////////////////////////
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
                struct array_matches<T[M], T[proto::v5::N]>
                  : std::true_type
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // array_matches
                template<typename T, typename U>
                struct lambda_matches
                  : std::false_type
                {};

                template<typename T>
                struct lambda_matches<T, proto::v5::_>
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
                struct lambda_matches<T[M], proto::v5::_>
                  : std::true_type
                {};

                template<typename T, std::size_t M>
                struct lambda_matches<T[M], T[M]>
                  : std::true_type
                {};

                template<typename T, typename U>
                struct lambda_matches<T, fuzzy<U>>
                  : lambda_matches<T, U>
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

                ////////////////////////////////////////////////////////////////////////////////////
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
                struct value_matches_<T(&)[M], T(&)[proto::v5::N]>
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
                struct value_matches_<T, proto::v5::_>
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

                ////////////////////////////////////////////////////////////////////////////////////
                // vararg_matches
                template<typename Child0, typename Child1>
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

                ////////////////////////////////////////////////////////////////////////////////////
                // matches_expr_
                template<typename BasicExpr, typename Grammar, typename Enable = void>
                struct matches_expr_
                  : std::false_type
                {};

                // Handle non-variadic non-terminal matches
                template<
                    typename Tag0, typename ...Child0
                  , typename Tag1, typename ...Child1
                >
                struct matches_expr_<
                    Tag0(Child0...)
                  , Tag1(Child1...)
                  , typename std::enable_if<
                        !Tag0::proto_is_terminal_type::value &&
                        sizeof...(Child0) == sizeof...(Child1)
                    >::type
                >
                  : utility::and_<
                        tag_matches<Tag0, Tag1>
                      , matches<Child0, Child1>...
                    >
                {};

                // Handle variadic non-terminal matches
                template<
                    typename Tag0, typename ...Child0
                  , typename Tag1, typename ...Child1
                >
                struct matches_expr_<
                    Tag0(Child0...)
                  , Tag1(Child1......)
                  , typename std::enable_if<!Tag0::proto_is_terminal_type::value>::type
                >
                  : utility::and_<
                        tag_matches<Tag0, Tag1>
                      , vararg_matches<void(Child0...), void(Child1...)>
                    >
                {};

                // Handle terminal matches.
                template<
                    typename Tag0, typename Value0
                  , typename Tag1, typename Value1
                >
                struct matches_expr_<
                    Tag0(Value0)
                  , Tag1(Value1)
                  , typename std::enable_if<Tag0::proto_is_terminal_type::value>::type
                >
                  : utility::and_<
                        tag_matches<Tag0, Tag1>
                      , value_matches_<Value0, Value1>
                    >
                {};
            }

            namespace extension
            {
                template<typename Tag, typename Arity, typename Action, typename ...Grammars>
                struct grammar_impl<expr_tag<Tag, Arity, Action>(Grammars...)>
                {
                    template<typename Expr>
                    struct apply
                      : detail::matches_expr_<
                            typename std::remove_reference<Expr>::type::proto_expr_descriptor_type
                          , Tag(Grammars...)
                        >
                    {};
                };

                template<typename Tag, typename Arity, typename Action, typename ...Grammars>
                struct grammar_impl<expr_tag<Tag, Arity, Action>(Grammars......)>
                {
                    template<typename Expr>
                    struct apply
                      : detail::matches_expr_<
                            typename std::remove_reference<Expr>::type::proto_expr_descriptor_type
                          , Tag(Grammars......)
                        >
                    {};
                };

                template<typename ...Children>
                struct grammar_impl<_(Children...)>
                {
                    template<typename Expr>
                    struct apply
                      : detail::matches_expr_<
                            typename std::remove_reference<Expr>::type::proto_expr_descriptor_type
                          , _(Children...)
                        >
                    {};
                };

                template<typename ...Children>
                struct grammar_impl<_(Children......)>
                {
                    template<typename Expr>
                    struct apply
                      : detail::matches_expr_<
                            typename std::remove_reference<Expr>::type::proto_expr_descriptor_type
                          , _(Children......)
                        >
                    {};
                };
            }
        }
    }
}

#endif
