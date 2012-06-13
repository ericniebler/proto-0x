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

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // get_basic_expr
            template<typename Expr>
            typename Expr::proto_basic_expr_type get_basic_expr(Expr const &);

            ////////////////////////////////////////////////////////////////////////////////////////
            // or_c_impl
            template<bool Head, typename ...Tail>
            struct or_c_impl
              : std::false_type
            {};

            template<typename ...Tail>
            struct or_c_impl<true, Tail...>
              : std::true_type
            {};

            template<typename Head, typename ...Tail>
            struct or_c_impl<false, Head, Tail...>
              : or_c_impl<Head::value, Tail...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // or_c_impl
            template<typename ...T>
            struct or_c
              : std::false_type
            {};

            template<typename Head, typename ...T>
            struct or_c<Head, T...>
              : or_c_impl<Head::value, T...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_c_impl
            template<bool Head, typename ...Tail>
            struct and_c_impl
              : std::true_type
            {};

            template<typename ...Tail>
            struct and_c_impl<false, Tail...>
              : std::false_type
            {};

            template<typename Head, typename ...Tail>
            struct and_c_impl<true, Head, Tail...>
              : and_c_impl<Head::value, Tail...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_c_impl
            template<typename ...T>
            struct and_c
              : std::false_type
            {};

            template<typename Head, typename ...T>
            struct and_c<Head, T...>
              : and_c_impl<Head::value, T...>
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
            // terminal_matches
            template<typename T, typename U>
            struct terminal_matches
              : std::false_type
            {};

            template<typename T>
            struct terminal_matches<T, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, proto::_>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_impl
            template<typename Expr, typename BasicExpr, typename Grammar, typename EnableIf = void>
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
                    tag_matches<Tag0, Tag1>
                  , matches_impl<Args0, Args0, Args1>...
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
                    tag_matches<Tag0, Tag1>
                  , terminal_matches<Value0, Value1>
                >
            {};
        }

        template<typename Expr, typename Grammar>
        struct matches
          : detail::matches_impl<Expr, Expr, Grammar>
        {};
    }
}

#endif // BOOST_PROTO_MATCHES_HPP_INCLUDED
