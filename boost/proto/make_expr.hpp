///////////////////////////////////////////////////////////////////////////////
// make_expr.hpp
// Definitions for make_expr and as_expr.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MAKE_EXPR_HPP_INCLUDED
#define BOOST_PROTO_MAKE_EXPR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/args.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/detail/common_domain.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // get_domain
            template<typename T>
            typename T::proto_domain_type get_domain(T const &);

            default_domain get_domain(utility::any);

            ////////////////////////////////////////////////////////////////////////////////////////
            // Misc domain helpers
            template<typename Domain, typename ...T>
            inline constexpr auto value_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::store_value{}
            )

            template<typename ...T>
            inline constexpr auto value_storer(deduce_domain const &, T const &... t)
            BOOST_PROTO_AUTO_RETURN(
                typename common_domain<decltype(get_domain(t))...>::type::store_value{}
            )

            template<typename Domain, typename ...T>
            inline constexpr auto child_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::store_child{}
            )

            template<typename ...T>
            inline constexpr auto child_storer(deduce_domain const &, T const &... t)
            BOOST_PROTO_AUTO_RETURN(
                typename common_domain<decltype(get_domain(t))...>::type::store_child{}
            )

            template<typename Domain, typename ...T>
            inline constexpr auto expr_maker(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::make_expr{}
            )

            template<typename ...T>
            inline constexpr auto expr_maker(deduce_domain const &, T const &... t)
            BOOST_PROTO_AUTO_RETURN(
                typename common_domain<decltype(get_domain(t))...>::type::make_expr{}
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // detail::make_expr
            template<template<typename, typename> class Expr, typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                Expr<Tag, args<T...>>{static_cast<Tag &&>(tag), static_cast<T &&>(t)...}
            )

            template<template<typename, typename, typename> class Expr, typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                Expr<Tag, args<T...>, Domain>{static_cast<Tag &&>(tag), static_cast<T &&>(t)...}
            )

            template<typename T, typename Domain, bool IsExpr = is_expr<T>::value>
            struct as_expr
            {
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    child_storer(Domain(), t)(static_cast<T &&>(t))
                )
            };

            template<typename T, typename Domain>
            struct as_expr<T, Domain, false>
            {
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    expr_maker(Domain(), t)(
                        proto::tag::terminal()
                      , exprs::make_args(typename Domain::store_value()(static_cast<T &&>(t))) // HACK
                    )
                )
            };

            /// INTERNAL ONLY total HACK
            template<typename T, typename Domain>
            struct as_expr<args<T>, Domain, false>
            {
                inline constexpr auto operator()(args<T> &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    (static_cast<args<T> &&>(t).proto_child0)
                )
            };
        }

        namespace domains
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            // make_expr (with domain)
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t...)(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////////
            // as_expr (with domain)
        #if 1
            template<typename Domain, typename T>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::as_expr<T, Domain>()(static_cast<T &&>(t))
            )
        #else // clang wierdness
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::make_expr()(
                    proto::tag::terminal()
                  , exprs::make_args(typename Domain::store_value()(static_cast<T &&>(t))) // HACK
                )
            )

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::store_child()(static_cast<T &&>(t))
            )

            /// INTERNAL ONLY total HACK
            template<typename Domain, typename T>
            inline constexpr auto as_expr(args<T> &&t)
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<args<T> &&>(t).proto_child0)
            )
        #endif

            namespace result_of
            {
                template<typename Domain, typename Tag, typename ...T>
                struct make_expr
                {
                    typedef decltype(domains::make_expr<Domain>(Tag(), std::declval<T>()...)) type;
                };

                template<typename Domain, typename T>
                struct as_expr
                {
                    typedef decltype(domains::as_expr<Domain>(std::declval<T>())) type;
                };
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_expr (no domain)
        template<typename Tag, typename ...T>
        inline constexpr auto make_expr(Tag tag, T &&...t)
        BOOST_PROTO_AUTO_RETURN(
            proto::domains::make_expr<default_domain>(
                static_cast<Tag &&>(tag)
              , static_cast<T &&>(t)...
            )
        )

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr (no domain)
        template<typename T>
        inline constexpr auto as_expr(T &&t)
        BOOST_PROTO_AUTO_RETURN(
            proto::domains::as_expr<default_domain>(static_cast<T &&>(t))
        )

        namespace result_of
        {
            template<typename Tag, typename ...T>
            struct make_expr
            {
                typedef decltype(proto::make_expr(Tag(), std::declval<T>()...)) type;
            };

            template<typename T>
            struct as_expr
            {
                typedef decltype(proto::as_expr(std::declval<T>())) type;
            };
        }
    }
}

#endif
