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
#include <boost/proto/generator.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct not_a_generator
            {};

            struct not_a_grammar
            {};

            struct not_a_domain
            {};
        }

        template<typename Expr, typename U = void>
        struct domain_of
        {
            typedef typename Expr::proto_domain type;
        };

        namespace domainns
        {
            template<typename Generator, typename Grammar, typename Super>
            struct domain
            {
                typedef Generator proto_generator;
                typedef Grammar proto_grammar;
                typedef Super proto_super_domain;
                typedef domain proto_base_domain;

                struct as_expr
                {
                    template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
                    inline constexpr auto operator()(T &&t) const
                    BOOST_PROTO_RETURN(
                        expr<tag::terminal, term<detail::as_arg<T>>>(static_cast<T &&>(t))
                    )

                    template<typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
                    inline constexpr T && operator()(T &&t) const noexcept
                    {
                        return static_cast<T &&>(t);
                    }
                };

                template<typename Tag>
                struct make_expr
                {
                };
            };

            struct default_domain
              : domain<>
            {};
        }
    }
}

#endif
