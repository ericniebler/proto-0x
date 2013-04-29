////////////////////////////////////////////////////////////////////////////////////////////////////
// domain.hpp
// Contains definition of domain\<\> class template and helpers for
// defining domains with a generator and a grammar for controlling
// operator overloading.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_CXX11_DOMAIN_HPP_INCLUDED

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

                ////////////////////////////////////////////////////////////////////////////////////
                // detail::make_custom_expr
                template<template<typename> class Expr, typename Domain, typename Tag, typename ...T>
                inline constexpr auto make_custom_expr(Tag && tag, T &&...t)
                BOOST_PROTO_AUTO_RETURN(
                    Expr<utility::uncvref<Tag>(T...)>{static_cast<Tag &&>(tag), static_cast<T &&>(t)...}
                )

                template<template<typename, typename> class Expr, typename Domain, typename Tag, typename ...T>
                inline constexpr auto make_custom_expr(Tag && tag, T &&...t)
                BOOST_PROTO_AUTO_RETURN(
                    Expr<utility::uncvref<Tag>(T...), Domain>{static_cast<Tag &&>(tag), static_cast<T &&>(t)...}
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // detail::template_arity
                template<template<typename> class T>
                char (&template_arity())[1];

                template<template<typename, typename> class T>
                char (&template_arity())[2];
            }

            namespace domains
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // make_custom_expr
                template<template<typename...A> class Expr, typename Domain>
                struct make_custom_expr
                {
                private:
                    static constexpr std::size_t template_arity =
                        sizeof(detail::template_arity<Expr>());

                    static_assert(
                        template_arity == 1 || template_arity == 2
                      , "expected template of arity 1 or 2"
                    );

                    static_assert(
                        template_arity == 1 || !std::is_same<Domain, void>::value
                      , "for custom expression types that accept a domain parameter, you must"
                        " specify what the domain should be"
                    );

                public:
                    template<typename Tag, typename ...T>
                    inline constexpr auto operator()(Tag && tag, T &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        detail::make_custom_expr<Expr, Domain>(
                            static_cast<Tag &&>(tag)
                          , static_cast<T &&>(t)...
                        )
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
                    using make_expr = make_custom_expr<exprs::expr, Domain>;

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
                    using make_expr = make_custom_expr<exprs::basic_expr, BaseDomain>;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // default_grammar
                struct default_grammar
                  : detail::as_grammar_<not_(matches_(address_of(_)))>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // default_domain
                struct default_domain
                  : domain<default_domain, default_grammar>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // basic_default_domain
                struct basic_default_domain
                  : domain<basic_default_domain, default_grammar>
                {
                    using make_expr = make_custom_expr<exprs::basic_expr, basic_default_domain>;
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
                    using type = typename Expr::proto_domain_type;
                };

                template<typename Expr>
                struct domain_of<Expr &>
                {
                    using type = typename Expr::proto_domain_type;
                };

                template<typename Expr>
                struct domain_of<Expr &&>
                {
                    using type = typename Expr::proto_domain_type;
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _domain_of
            struct _domain_of
              : proto::v5::basic_action<_domain_of>
            {
                template<typename E, typename ...Rest>
                auto operator()(E &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    typename result_of::domain_of<E>::type()
                )
            };
        }
    }
}

#endif
