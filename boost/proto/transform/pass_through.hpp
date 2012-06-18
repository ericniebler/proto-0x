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
            template<std::size_t N, std::size_t M>
            struct pass_through_fail_
            {
                static_assert(N == M, "Wrong number of arguments to pass-through transform");
            };

            template<std::size_t N, std::size_t M, typename Ints, typename ...Args>
            struct pass_through_1_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // pass_through_4_
            template<std::size_t N, std::size_t M, typename Ints, typename T, typename ...Args>
            struct pass_through_4_
              : pass_through_fail_<N, M>
            {};

            template<std::size_t N, std::size_t M, typename Ints, typename T, typename ...Args>
            struct pass_through_4_<N, M, Ints, proto::vararg<T>, Args...>
              : pass_through_4_<N, M + 1, Ints, proto::vararg<T>, Args..., proto::vararg<T>>
            {};

            template<std::size_t N, typename Ints, typename T, typename ...Args>
            struct pass_through_4_<N, N, Ints, proto::vararg<T>, Args...>
              : pass_through_1_<N, N, Ints, Args...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // pass_through_3_
            template<std::size_t N, typename Ints, typename Args>
            struct pass_through_3_;

            template<std::size_t N, typename Ints, typename ...Args>
            struct pass_through_3_<N, Ints, utility::list<Args...>>
              : pass_through_1_<N, N, Ints, Args...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // pass_through_2_
            template<std::size_t N, typename Ints, typename T, typename ...Args>
            struct pass_through_2_
              : pass_through_fail_<N, N + 1>
            {};

            template<std::size_t N, typename Ints, typename T, typename ...Args>
            struct pass_through_2_<N, Ints, proto::vararg<T>, Args...>
              : pass_through_3_<N, Ints, utility::pop_back<Args...>>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // pass_through_1_
            template<std::size_t N, std::size_t M, typename Ints, typename ...Args>
            struct pass_through_1_
              : std::conditional<
                    (N >= M)
                  , pass_through_4_<N, M, Ints, typename utility::back_type<Args...>::type, Args...>
                  , utility::lazy_conditional<
                        (N == M - 1)
                      , pass_through_2_<N, Ints, typename utility::back_type<Args...>::type, Args...>
                      , pass_through_fail_<N, M>
                    >
                >::type
            {};

            template<std::size_t N, std::size_t... Indices, typename ...Args>
            struct pass_through_1_<N, N, utility::ints<Indices...>, Args...>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::domains::make_expr<decltype(e.proto_domain())>(
                        e.proto_tag()
                      , as_transform<Args>()(proto::child<Indices>(static_cast<E &&>(e)), static_cast<T &&>(t)...)...
                    )
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // pass_through_0_
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
