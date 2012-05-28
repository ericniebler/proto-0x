///////////////////////////////////////////////////////////////////////////////
/// \file generator.hpp
/// Contains definition of generator\<\> class template, which end users can
/// specialize for generating domain-specific expression wrappers.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GENERATOR_HPP_INCLUDED
#define BOOST_PROTO_GENERATOR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        template<typename Fun>
        struct static_cast_with
        {
            template<typename T>
            constexpr auto operator()(T & t) const
            BOOST_PROTO_RETURN(
                static_cast<decltype((Fun()(t))) &>(t)
            )

            template<typename T>
            constexpr auto operator()(T const & t) const
            BOOST_PROTO_RETURN(
                static_cast<decltype((Fun()(t))) const &>(t)
            )

            template<typename T>
            constexpr auto operator()(T && t) const
            BOOST_PROTO_RETURN(
                static_cast<decltype((Fun()(static_cast<T &&>(t)))) &&>(static_cast<T &&>(t))
            )
        };

        struct identity
        {
            template<typename Expr>
            constexpr Expr && operator()(Expr && e) const
            {
                return static_cast<Expr &&>(e);
            }
        };

        template<template<class Expr> class Extends>
        struct expr_wrap
        {
            template<typename Expr>
            Extends<Expr> operator()(Expr & e) const
            {
                return Extends<Expr>(e);
            }

            template<typename Expr>
            Extends<Expr> operator()(Expr const & e) const
            {
                return Extends<Expr>(e);
            }

            template<typename Expr>
            Extends<Expr> operator()(Expr && e) const
            {
                return Extends<Expr>(static_cast<Expr &&>(e));
            }
        };

        template<typename ConvertFun, typename CastFun>
        struct generator
          : ConvertFun
        {
            typedef CastFun proto_downcaster;
        };

        struct default_generator
          : generator<>
        {};
    }
}

#endif
