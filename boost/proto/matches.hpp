///////////////////////////////////////////////////////////////////////////////
// matches.hpp
// Contains the definition of the matches\<\> Boolean metafunction and the
// facilities for building Proto grammars.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_MATCHES_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_impl
            template<typename Expr, typename BasicExpr, typename Grammar, typename EnableIf = void>
            struct matches_impl;

            ////////////////////////////////////////////////////////////////////////////////////////
            // get_basic_expr
            template<typename Expr>
            typename Expr::proto_basic_expr_type get_basic_expr(Expr const &);

            ////////////////////////////////////////////////////////////////////////////////////////
            // or_c
            template<bool Head, typename ...Tail>
            struct or_c
              : std::false_type
            {};

            template<typename ...Tail>
            struct or_c<true, Tail...>
              : std::true_type
            {};

            template<typename Head, typename ...Tail>
            struct or_c<false, Head, Tail...>
              : or_c<Head::value, Tail...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // or_ (like mpl::or_, but variadic)
            template<typename ...T>
            struct or_
              : std::true_type
            {};

            template<typename Head, typename ...T>
            struct or_<Head, T...>
              : or_c<Head::value, T...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_c
            template<bool Head, typename ...Tail>
            struct and_c
              : std::true_type
            {};

            template<typename ...Tail>
            struct and_c<false, Tail...>
              : std::false_type
            {};

            template<typename Head, typename ...Tail>
            struct and_c<true, Head, Tail...>
              : and_c<Head::value, Tail...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_  (like mpl::and_, but variadic)
            template<typename ...T>
            struct and_
              : std::true_type
            {};

            template<typename Head, typename ...T>
            struct and_<Head, T...>
              : and_c<Head::value, T...>
            {};

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
              : detail::and_<lambda_matches<Expr0, Grammar0>...>
            {};

            // How terminal_matches<> handles references and cv-qualifiers.
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
            // terminal_matches
            template<typename T, typename U>
            struct terminal_matches
              : detail::and_c<
                    is_cv_ref_compatible<T, U>::value
                    // TODO: instantiate fewer templates here. Make this lazy?
                  , lambda_matches<
                        typename std::remove_const<typename std::remove_reference<T>::type>::type
                      , typename std::remove_const<typename std::remove_reference<U>::type>::type
                    >
                >
            {};

            template<typename T, std::size_t M>
            struct terminal_matches<T(&)[M], T(&)[proto::N]>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct terminal_matches<T(&)[M], T *>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T &, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T const &, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, proto::_>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, exact<T> >
              : std::true_type
            {};

            template<typename T, typename U>
            struct terminal_matches<T, proto::convertible_to<U> >
              : std::is_convertible<T, U>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_vararg
            template<typename ...T>
            struct is_vararg
              : std::false_type
            {};

            template<typename Grammar>
            struct is_vararg<vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename Grammar>
            struct is_vararg<T0, vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename T1, typename Grammar>
            struct is_vararg<T0, T1, vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename T1, typename T2, typename ...Rest>
            struct is_vararg<T0, T1, T2, Rest...>
              : is_vararg<Rest...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // vararg_matches
            template<typename Args0, typename Args1>
            struct vararg_matches
              : std::false_type
            {};

            template<typename Head0, typename ...Tail0, typename Head1, typename ...Tail1>
            struct vararg_matches<args<Head0, Tail0...>, args<Head1, Tail1...>>
              : detail::and_c<
                    matches_impl<Head0, Head0, Head1>::value
                  , vararg_matches<args<Tail0...>, args<Tail1...>>
                >
            {};

            template<typename Head0, typename ...Tail0, typename Grammar>
            struct vararg_matches<args<Head0, Tail0...>, args<vararg<Grammar>>>
              : detail::and_c<
                    matches_impl<Head0, Head0, Grammar>::value
                  , matches_impl<Tail0, Tail0, Grammar>...
                >
            {};

            template<typename Grammar>
            struct vararg_matches<args<>, args<vararg<Grammar>>>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_impl
            template<typename Expr, typename BasicExpr, typename Grammar, typename EnableIf>
            struct matches_impl
              : matches_impl<
                    Expr
                  , decltype(detail::get_basic_expr(std::declval<BasicExpr>()))
                  , Grammar
                >
            {};

            template<typename Expr, typename BasicExpr>
            struct matches_impl<Expr, BasicExpr, proto::_, void>
              : std::true_type
            {};

            template<
                typename Expr
              , template<typename, typename, typename...> class Expr0
              , template<typename, typename, typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_impl<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) == sizeof...(Args1)
                >::type
            >
              : detail::and_c<
                    tag_matches<Tag0, Tag1>::value
                  , matches_impl<Args0, Args0, Args1>...
                >
            {};

            template<
                typename Expr
              , template<typename, typename, typename...> class Expr0
              , template<typename, typename, typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_impl<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) != sizeof...(Args1)
                >::type
            >
              : detail::and_c<
                    tag_matches<Tag0, Tag1>::value
                  , is_vararg<Args1...>
                  , vararg_matches<args<Args0...>, args<Args1...>>
                >
            {};

            template<
                typename Expr
              , template<typename, typename, typename...> class Expr0
              , template<typename, typename, typename...> class Expr1
              , typename Tag0, typename Value0, typename ...Rest0
              , typename Tag1, typename Value1, typename ...Rest1
            >
            struct matches_impl<
                Expr
              , Expr0<Tag0, args<Value0>, Rest0...>
              , Expr1<Tag1, args<Value1>, Rest1...>
              , typename std::enable_if<Tag0::proto_is_terminal::value>::type
            >
              : detail::and_c<
                    tag_matches<Tag0, Tag1>::value
                  , terminal_matches<Value0, Value1>
                >
            {};

            template<
                typename Expr
              , template<typename, typename, typename...> class Expr0
              , template<typename, typename, typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_impl<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<Tag0::proto_is_terminal::value>::type
            >
              : std::false_type
            {};
        }

        template<typename Expr, typename Grammar>
        struct matches
          : detail::matches_impl<Expr, Expr, Grammar>
        {};
    }
}

#endif // BOOST_PROTO_MATCHES_HPP_INCLUDED
