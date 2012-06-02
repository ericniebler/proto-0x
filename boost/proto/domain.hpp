///////////////////////////////////////////////////////////////////////////////
/// \file domain.hpp
/// Contains definition of domain\<\> class template and helpers for
/// defining domains with a generator and a grammar for controlling
/// operator overloading.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_DOMAIN_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct not_a_grammar
            {};

            struct not_a_domain
            {};

            template<template<typename...> class Expr, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_RETURN(
                Expr<Tag, args<T...>>(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
            )
        }

        namespace domains
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            // make_expr (with domain)
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_RETURN(
                typename Domain::make_expr()(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////////
            // as_expr (with domain)
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_RETURN(
                typename Domain::make_expr()(
                    proto::tag::terminal()
                  , exprs::make_args(typename Domain::store_value()(static_cast<T &&>(t)))
                )
            )

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_RETURN(
                typename Domain::store_child()(static_cast<T &&>(t))
            )

            /// INTERNAL ONLY total hack
            template<typename Domain, typename T>
            inline constexpr auto as_expr(args<T> &&t)
            BOOST_PROTO_RETURN(
                (static_cast<args<T> &&>(t).proto_child0)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // domain
            template<typename Domain, typename Grammar, typename SuperDomain>
            struct domain
            {
                typedef Domain          proto_domain_type;
                typedef Grammar         proto_grammar_type;
                typedef SuperDomain     proto_super_domain_type;

                // Define this in your derived domain class to control how
                // objects are stored within the terminals of your domain.
                struct store_value
                {
                    template<typename T>
                    inline constexpr T && operator()(T &&t) const noexcept
                    {
                        return static_cast<T &&>(t);
                    }
                };

                // Define this in your derived domain class to control how intermediate
                // nodes are stored within your expressions.
                struct store_child
                {
                    template<typename T>
                    inline constexpr T && operator()(T &&t) const noexcept
                    {
                        return static_cast<T &&>(t);
                    }
                };

                // Define this in your derived domain class to control expressions are
                // assembled.
                struct make_expr
                {
                    template<typename Tag, typename ...T>
                    inline constexpr auto operator()(Tag tag, T &&... t) const
                    BOOST_PROTO_RETURN(
                        detail::make_expr<proto::expr>(
                            static_cast<Tag &&>(tag)
                          , proto::domains::as_expr<default_domain>(static_cast<T &&>(t))...
                        )
                    )
                };
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // default_domain
            struct default_domain
              : domain<default_domain>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // custom_expr
            //template<template<typename Tag, typename Args> class Expr, typename Domain>
            template<template<typename...> class Expr, typename Domain>
            struct custom_expr
            {
                template<typename Tag, typename ...T>
                inline constexpr auto operator()(Tag tag, T &&... t) const
                BOOST_PROTO_RETURN(
                    detail::make_expr<Expr>(
                        static_cast<Tag &&>(tag)
                      , proto::domains::as_expr<Domain>(static_cast<T &&>(t))...
                    )
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_expr (no domain)
        template<typename Tag, typename ...T>
        inline constexpr auto make_expr(Tag tag, T &&...t)
        BOOST_PROTO_RETURN(
            proto::domains::make_expr<default_domain>(
                static_cast<Tag &&>(tag)
              , static_cast<T &&>(t)...
            )
        )

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr (no domain)
        template<typename T>
        inline constexpr auto as_expr(T &&t)
        BOOST_PROTO_RETURN(
            proto::domains::as_expr<default_domain>(static_cast<T &&>(t))
        )
    }
}

#endif
