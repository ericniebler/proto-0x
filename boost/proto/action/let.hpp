///////////////////////////////////////////////////////////////////////////////
// let.hpp
// A BasicAction that creates local variables in a lexical scope
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_LET_HPP_INCLUDED
#define BOOST_PROTO_ACTION_LET_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        // TODO: I think this implementation allows non-local scope variables to be
        // accessed from with the action of other let statements. That would be a bug.
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
                    env_var_tag<Local>() = action<Action>()(static_cast<Args &&>(args)...)
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
                    action<Action>()(
                        static_cast<Expr &&>(expr)
                      , detail::empty_state()
                      , proto::make_env(
                            eval_local<Locals>()(static_cast<Expr &&>(expr))...
                          , tags::has_scope = std::true_type()
                        )
                    )
                )

                template<typename Expr, typename State>
                auto operator()(Expr &&expr, State && state) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Action>()(
                        static_cast<Expr &&>(expr)
                      , static_cast<State &&>(state)
                      , proto::make_env(
                            eval_local<Locals>()(
                                static_cast<Expr &&>(expr)
                              , static_cast<State &&>(state)
                            )...
                          , tags::has_scope = std::true_type()
                        )
                    )
                )

                template<typename Expr, typename State, typename Env, typename ...Rest>
                auto operator()(Expr &&expr, State && state, Env &&env, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Action>()(
                        static_cast<Expr &&>(expr)
                      , static_cast<State &&>(state)
                      , proto::make_env(
                            static_cast<Env &&>(env)
                          , eval_local<Locals>()(
                                static_cast<Expr &&>(expr)
                              , static_cast<State &&>(state)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )...
                          , tags::has_scope = std::true_type()
                        )
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            template<typename Local, typename Env>
            inline void assert_has_scope(Env const &) noexcept
            {
                static_assert(
                    result_of::env_var<Env, tags::has_scope_type>::type::value
                  , "local variable accessed out of a let scope"
                );
            }
        }

        template<typename Local>
        struct _local
          : basic_action<_local<Local>>
          , detail::local_base
        {
            template<typename Expr, typename State, typename Env, typename ...Rest, typename This = Local>
            auto operator()(Expr &&, State &&, Env &&env, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                (
                    detail::assert_has_scope<Local>(env)
                  , static_cast<Env &&>(env)[static_cast<This const &>(*this)]
                )
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

        template<typename ...As>
        struct action<let(As...)>
          : detail::_let<
                typename utility::result_of::back<As...>::type
              , typename utility::pop_back<void(As...)>::type
            >
        {};
    }
}

#endif
