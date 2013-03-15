////////////////////////////////////////////////////////////////////////////////////////////////////
// let.hpp
// A BasicAction that creates local variables in a lexical scope
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_LET_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_LET_HPP_INCLUDED

#include <utility>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace detail
            {
                template<typename Local>
                struct eval_local;

                template<typename Local, typename Action>
                struct eval_local<Local(*)(Action)>
                  : basic_action<eval_local<Local(*)(Action)>>
                {
                    template<typename ...Args>
                    auto operator()(Args &&...args) const
                    BOOST_PROTO_AUTO_RETURN(
                        env_var_tag<Local>() = as_action_<Action>()(static_cast<Args &&>(args)...)
                    )
                };

                template<typename Action, typename Locals>
                struct _let;

                template<typename Action, typename ...Locals>
                struct _let<Action, void(Locals...)>
                  : basic_action<_let<Action, void(Locals...)>>
                {
                    template<typename Expr>
                    auto operator()(Expr &&expr) const
                    BOOST_PROTO_AUTO_RETURN(
                        as_action_<Action>()(
                            static_cast<Expr &&>(expr)
                          , proto::cxx11::make_env(eval_local<Locals>()(static_cast<Expr &&>(expr))...)
                        )
                    )

                    template<typename Expr, typename Env, typename ...Rest>
                    auto operator()(Expr &&expr, Env &&env, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        as_action_<Action>()(
                            static_cast<Expr &&>(expr)
                          , proto::cxx11::make_env(
                                static_cast<Env &&>(env)
                              , eval_local<Locals>()(
                                    static_cast<Expr &&>(expr)
                                  , static_cast<Env &&>(env)
                                  , static_cast<Rest &&>(rest)...
                                )...
                            )
                          , static_cast<Rest &&>(rest)...
                        )
                    )
                };
            }

            template<typename Local>
            struct _local
              : basic_action<_local<Local>>
            {
                template<typename Expr, typename Env, typename ...Rest, typename This = Local>
                auto operator()(Expr &&, Env &&env, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Env &&>(env)[static_cast<This const &>(*this)]
                )
            };

            struct _a
              : _local<_a>
            {};

            struct _b
              : _local<_b>
            {};

            struct let
            {};

            namespace extension
            {
                template<typename ...As>
                struct action_impl<let(As...)>
                  : detail::_let<
                        typename utility::result_of::back<As...>::type
                      , typename utility::pop_back<void(As...)>::type
                    >
                {};
            }
        }
    }
}

#endif