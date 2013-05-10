////////////////////////////////////////////////////////////////////////////////////////////////////
// domain.hpp
// Contains definition of domain\<\> class template and helpers for
// defining domains with a generator and a grammar for controlling
// operator overloading.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_V5_DOMAIN_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/make_expr.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/grammar/basic_grammar.hpp>
#include <boost/proto/v5/grammar/not.hpp>
#include <boost/proto/v5/grammar/expr.hpp>
#include <boost/proto/v5/action/matches.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // not_a_domain
                struct not_a_domain
                {
                    using proto_grammar_type = default_grammar; ///< INTERNAL ONLY
                    using store_value = utility::identity;      ///< INTERNAL ONLY
                    using store_child = utility::identity;      ///< INTERNAL ONLY
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // make_expr_raw_
                template<typename Domain>
                struct make_expr_raw_
                {
                    // If Tag does not represents a terminal, first pass the argument(s) through as_expr
                    template<typename Tag, typename ...T
                      , BOOST_PROTO_ENABLE_IF(!Tag::proto_is_terminal_type::value)
                    >
                    inline constexpr auto operator()(Tag && tag, T &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        typename Domain::make_expr()(
                            static_cast<Tag &&>(tag)
                          , proto::v5::as_expr<Domain>(static_cast<T &&>(t))...
                        )
                    )

                    // If Tag represents a terminal, don't pass the argument(s) through as_expr
                    template<typename Tag, typename T
                      , BOOST_PROTO_ENABLE_IF(Tag::proto_is_terminal_type::value)
                    >
                    inline constexpr auto operator()(Tag && tag, T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        typename Domain::make_expr()(
                            static_cast<Tag &&>(tag)
                          , typename Domain::store_value()(static_cast<T &&>(t))
                        )
                    )
                };
            }

            namespace domains
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // make_custom_expr
                template<template<typename...A> class Expr, typename ...Rest>
                struct make_custom_expr<Expr<_, Rest...>>
                {
                    template<typename Tag, typename ...T>
                    inline constexpr auto operator()(Tag && tag, T &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        Expr<utility::uncvref<Tag>(T...), Rest...>{
                            static_cast<Tag &&>(tag)
                          , static_cast<T &&>(t)...
                        }
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // domain_base
                struct domain_base
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // domain
                template<typename Domain, typename Grammar, typename SuperDomain>
                struct domain
                  : domain_base
                {
                    using proto_domain_type = Domain;
                    using proto_grammar_type = Grammar;
                    using proto_super_domain_type = SuperDomain;

                    // Define this in your derived domain class to control how
                    // objects are stored within the terminals of your domain.
                    using store_value = typename SuperDomain::store_value;

                    // Define this in your derived domain class to control how intermediate
                    // nodes are stored within your expressions.
                    using store_child = typename SuperDomain::store_child;

                    // Define this in your derived domain class to control how expressions are
                    // assembled.
                    using make_expr = make_custom_expr<exprs::expr<_, Domain>>;

                    // Define this in your derived domain class to /really/ control how expressions are
                    // assembled. But really, you shouldn't be messing with this. Mess with make_expr
                    // instead.
                    using make_expr_raw = detail::make_expr_raw_<Domain>;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // safe_domain_adaptor
                template<typename BaseDomain>
                struct safe_domain_adaptor
                  : domain<
                        safe_domain_adaptor<BaseDomain>
                      , typename BaseDomain::proto_grammar_type
                      , BaseDomain
                    >
                {
                    using store_value = utility::identity;
                    using store_child = utility::identity;
                    using make_expr = typename BaseDomain::make_expr;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // basic_expr_domain_adaptor
                template<typename BaseDomain>
                struct basic_expr_domain_adaptor
                  : domain<basic_expr_domain_adaptor<BaseDomain>, proto::_, BaseDomain>
                {
                    using make_expr = make_custom_expr<exprs::basic_expr<_, BaseDomain>>;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // default_grammar
                struct default_grammar
                  : detail::as_grammar_<not_(matches_(address_of(_)))>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // default_domain
                struct default_domain
                  : domain<default_domain>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // basic_default_domain
                struct basic_default_domain
                  : domain<basic_default_domain, default_grammar, default_domain>
                {
                    using make_expr = make_custom_expr<exprs::basic_expr<_, basic_default_domain>>;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // auto_domain
                template<
                    typename Grammar
                  , typename SuperDomain
                  , template<typename...> class CustomExpr
                  , typename ...Rest
                >
                struct auto_domain<Grammar, SuperDomain, CustomExpr<_, Rest...>>
                  : domain<
                        auto_domain<Grammar, SuperDomain, CustomExpr<_, Rest...>>
                      , Grammar
                      , SuperDomain
                    >
                {
                    using make_expr = make_custom_expr<CustomExpr<_, Rest...>>;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // deduce_domain
                struct deduce_domain
                {
                    using proto_grammar_type = void;    ///< INTERNAL ONLY
                    using store_value = void;           ///< INTERNAL ONLY
                    using store_child = void;           ///< INTERNAL ONLY
                    using make_expr = void;             ///< INTERNAL ONLY
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_domain
            template<typename T>
            struct is_domain
              : std::is_base_of<domains::domain_base, T>
            {};

            namespace result_of
            {
                ////////////////////////////////////////////////////////////////////////////////////////
                // domain_of
                template<typename Expr>
                struct domain_of
                {
                    using type = decltype(detail::get_domain<Expr>(1));
                };

                template<typename Expr>
                struct domain_of<Expr &>
                {
                    using type = decltype(detail::get_domain<Expr>(1));
                };

                template<typename Expr>
                struct domain_of<Expr &&>
                {
                    using type = decltype(detail::get_domain<Expr>(1));
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _domain_of
            struct _domain_of
              : proto::v5::basic_action<_domain_of>
            {
                template<typename E, typename ...Rest>
                constexpr auto operator()(E &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    typename result_of::domain_of<E>::type()
                )
            };
        }
    }
}

#endif
