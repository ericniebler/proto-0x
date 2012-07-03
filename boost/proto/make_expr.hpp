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
            typename T::proto_domain_type get_domain(int);

            template<typename T>
            default_domain get_domain(...);

            ////////////////////////////////////////////////////////////////////////////////////////
            // get_domain
            template<typename Domain, typename ...T>
            struct get_common_domain_impl
            {
                typedef Domain type;
            };

            template<typename ...T>
            struct get_common_domain_impl<deduce_domain, T...>
              : common_domain<decltype(detail::get_domain<T>(1))...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // Misc domain helpers
            template<typename Domain, typename ...T>
            inline constexpr auto get_common_domain(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type{}
            )

            template<typename Domain, typename ...T>
            inline constexpr auto value_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::store_value{}
            )

            template<typename Domain, typename ...T>
            inline constexpr auto child_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::store_child{}
            )

            template<typename Domain, typename ...T>
            inline constexpr auto expr_maker(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::make_expr{}
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

            ////////////////////////////////////////////////////////////////////////////////////////
            // detail::normalize_expr
            template<typename This, typename Value, typename Domain>
            auto normalize_expr(virtual_member<This, Value, Domain> && e)
            BOOST_PROTO_AUTO_RETURN(
                // Must build a real binary expression from virtual members before the left hand
                // side (actually the member's enclosing object) dies and we lose it forever.
                typename Domain::make_expr{}(
                    tag::member()
                  , proto::left(static_cast<virtual_member<This, Value, Domain> &&>(e))
                  , proto::right(static_cast<virtual_member<This, Value, Domain> &&>(e))
                )
            )

            template<typename T>
            T && normalize_expr(T && t) noexcept
            {
                return static_cast<T &&>(t);
            }

            template<typename T, typename Domain, bool IsExpr = is_expr<T>::value>
            struct as_expr
            {
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    child_storer(Domain(), t)(detail::normalize_expr(static_cast<T &&>(t)))
                )
            };

            template<typename T, typename Domain>
            struct as_expr<T, Domain, false>
            {
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    expr_maker(Domain(), t)(
                        proto::tag::terminal()
                      , exprs::make_args(value_storer(Domain(), t)(static_cast<T &&>(t))) // HACK
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
        
            ////////////////////////////////////////////////////////////////////////////////////////
            // as_basic_expr
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            auto as_basic_expr(T &&t) ->
                decltype(typename Domain::store_child{}(t));

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            auto as_basic_expr(T &&t) ->
                basic_expr<
                    tag::terminal
                  , args<decltype(typename Domain::store_value{}(t))>
                  , Domain
                >;

            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_maker_if_2_
            template<typename Tag, typename Args, typename Domain
              , typename E = basic_expr<Tag, Args, Domain>
              , typename G = typename Domain::proto_grammar_type
              , BOOST_PROTO_ENABLE_IF(matches<E, G>::value)
            >
            inline constexpr auto expr_maker_if_2_()
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::make_expr{}
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr_if_
            template<typename Domain>
            struct make_expr_if_
            {
                template<typename Tag, typename ...T>
                constexpr auto operator()(Tag tag, T &&...t) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::expr_maker_if_2_<
                        Tag
                      , args<decltype(detail::as_basic_expr<Domain>(static_cast<T &&>(t)))...>
                      , Domain
                    >()(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_maker_if_1_
            template<typename Domain, typename Tag, typename ...T, typename ...U>
            inline constexpr auto expr_maker_if_1_(U const &...u)
            BOOST_PROTO_AUTO_RETURN(
                make_expr_if_<decltype(detail::get_common_domain(Domain(), u...))>{}
            )
        }

        namespace domains
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr (with domain)
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t...)(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr_if (with domain) - refuse to create expressions that are non-grammatical
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr_if(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker_if_1_<Domain, Tag, T...>(t...)(
                    static_cast<Tag &&>(tag)
                  , static_cast<T &&>(t)...
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // as_expr (with domain)
            template<typename Domain, typename T>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::as_expr<T, Domain>()(static_cast<T &&>(t))
            )

            namespace result_of
            {
                // BUGBUG make_expr<xxx, tag::terminal ...> should behave like as_expr?
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
