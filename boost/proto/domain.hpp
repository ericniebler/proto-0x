///////////////////////////////////////////////////////////////////////////////
// domain.hpp
// Contains definition of domain\<\> class template and helpers for
// defining domains with a generator and a grammar for controlling
// operator overloading.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_DOMAIN_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/make_expr.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // not_a_grammar
            struct not_a_grammar
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // not_a_domain
            struct not_a_domain
            {};
        }

        namespace domains
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // make_custom_expr
            template<template<typename...> class Expr, typename Domain>
            struct make_custom_expr
            {
                template<typename Tag, typename ...T>
                inline constexpr auto operator()(Tag tag, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::make_expr<Expr, Domain>(
                        static_cast<Tag &&>(tag)
                      , proto::domains::as_expr<Domain>(static_cast<T &&>(t))...
                    )
                )
            };

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
                  : utility::identity
                {};

                // Define this in your derived domain class to control how intermediate
                // nodes are stored within your expressions.
                struct store_child
                  : utility::identity
                {};

                // Define this in your derived domain class to control expressions are
                // assembled.
                struct make_expr
                  : make_custom_expr<expr, Domain>
                {};
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // default_domain
            struct default_domain
              : domain<default_domain>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // deduce_domain
            struct deduce_domain
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // domain_of
            template<typename Expr>
            struct domain_of
            {
                typedef typename Expr::proto_domain_type type;
            };

            template<typename Expr>
            struct domain_of<Expr &>
            {
                typedef typename Expr::proto_domain_type type;
            };
        }
    }
}

#endif
