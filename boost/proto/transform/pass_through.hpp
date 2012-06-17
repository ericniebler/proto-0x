///////////////////////////////////////////////////////////////////////////////
// pass_through.hpp
// Rebuild an expression where each child has been transformed according
// to the corresponding transform.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_PASS_THROUGH_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_PASS_THROUGH_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/mpl/identity.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/args.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<std::size_t N, std::size_t M, typename Ints, typename ...Args>
            struct pass_through_1_;

            template<std::size_t N, std::size_t M, typename Ints, typename T, typename ...Args>
            struct pass_through_2_
              : pass_through_2_<N, M+1, Ints, T, Args..., T>
            {
                static_assert(N >= M+1, "Too few children in the expression to use this pass-through trasform");
            };

            template<std::size_t N, typename Ints, typename T, typename ...Args>
            struct pass_through_2_<N, N, Ints, T, Args...>
              : pass_through_1_<N, N, Ints, Args...>
            {};

            template<std::size_t N, std::size_t M, typename Ints, typename ...Args>
            struct pass_through_1_
              : pass_through_2_<N, M, Ints, typename utility::back_type<Args...>::type, Args...>
            {};

            template<std::size_t N, std::size_t... Indices, typename ...Args>
            struct pass_through_1_<N, N, utility::ints<Indices...>, Args...>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::domains::make_expr<typename proto::domain_of<E>::type>(
                        as_transform<Args>()(proto::child<Indices>(static_cast<E &&>(e)), static_cast<T &&>(t)...)...
                    )
                )
            };

            template<bool IsTerminal, std::size_t N, std::size_t M, typename ...Args>
            struct pass_through_0_
              : pass_through_1_<N, M, utility::indices<N>, Args...>
            {};

            template<std::size_t N, std::size_t M, typename ...Args>
            struct pass_through_0_<true, N, M, Args...>
              : utility::identity
            {};
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // pass_through
        template<typename Expr>
        struct pass_through
          : pass_through<typename Expr::proto_basic_expr_type>
        {};

        template<template<typename...> class Expr, typename Tag, typename... Args, typename... Rest>
        struct pass_through<Expr<Tag, args<Args...>, Rest...>>
          : transform<pass_through<Expr<Tag, args<Args...>, Rest...>>>
        {
            typedef Expr<Tag, args<Args...>, Rest...> proto_grammar_type;

            template<typename E, typename ...T>
            auto operator()(E && e, T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                detail::pass_through_0_<
                    std::remove_reference<E>::type::proto_tag_type::proto_is_terminal::value
                  , std::remove_reference<E>::type::proto_args_type::proto_size::value
                  , sizeof...(Args)
                  , Args...
                >()(static_cast<E &&>(e), static_cast<T &&>(t)...)
            )
        };
    }
}

#endif
