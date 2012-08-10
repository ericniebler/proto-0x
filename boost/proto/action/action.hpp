///////////////////////////////////////////////////////////////////////////////
// action.hpp
// Helpers for building Proto actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_ACTION_HPP_INCLUDED
#define BOOST_PROTO_ACTION_ACTION_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // local_base
            struct local_base
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_local_
            template<typename Action>
            struct is_local_
              : std::is_base_of<local_base, Action>
            {};

            template<typename Action>
            struct is_local_<Action &>
              : std::is_base_of<local_base, Action>
            {};

            template<typename Action>
            struct is_local_<Action &&>
              : std::is_base_of<local_base, Action>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // hide_scope_
            template<
                typename Env
              , typename HasScope = tags::has_scope_type
              , BOOST_PROTO_ENABLE_IF(!result_of::env_var<Env, HasScope>::type::value)
            >
            inline auto hide_scope_(Env &&env)
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Env &&>(env)
            )

            template<
                typename Env
              , typename HasScope = tags::has_scope_type
              , BOOST_PROTO_ENABLE_IF(result_of::env_var<Env, HasScope>::type::value)
            >
            inline auto hide_scope_(Env &&env)
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<Env &&>(env), HasScope() = std::false_type())
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // do_action_
            template<bool IsLocal>
            struct do_action_
            {
                template<typename Action, typename Expr>
                auto operator()(Action &&act, Expr &&expr) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(act)(
                        static_cast<Expr &&>(expr)
                    )
                )

                template<typename Action, typename Expr, typename State>
                auto operator()(Action &&act, Expr &&expr, State &&state) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(act)(
                        static_cast<Expr &&>(expr)
                      , static_cast<State &&>(state)
                    )
                )

                template<typename Action, typename Expr, typename State, typename Env, typename ...Rest>
                auto operator()(Action &&act, Expr &&expr, State &&state, Env &&env, Rest &&...rest) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(act)(
                        static_cast<Expr &&>(expr)
                      , static_cast<State &&>(state)
                      , detail::hide_scope_(static_cast<Env &&>(env))
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            template<>
            struct do_action_<true>
            {
                template<typename Action, typename ...Args>
                auto operator()(Action &&act, Args &&...args) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(act)(static_cast<Args &&>(args)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action_base
        struct action_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // basic_action
        // CRTP base class for all actions
        template<typename Action>
        struct basic_action
          : action_base
        {
            typedef Action proto_action_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename T, typename Enable>
        struct action
        {
            typedef action proto_action_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                detail::do_action_<detail::is_local_<T>::value>()(
                    typename T::proto_action_type()
                  , static_cast<Args &&>(args)...
                )
            )
        };

        template<typename T>
        struct action<action<T>>
          : action<T>
        {};

        template<typename Ret, typename ...Args>
        struct action<Ret(*)(Args...)>
          : action<Ret(Args...)>
        {};

        template<typename Ret, typename ...Args>
        struct action<Ret(*)(Args......)>
          : action<Ret(Args......)>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_action
        template<typename T>
        struct is_action
          : std::is_base_of<action_base, T>
        {};

        template<typename T>
        struct is_action<T &>
          : std::is_base_of<action_base, T>
        {};

        template<typename T>
        struct is_action<T &&>
          : std::is_base_of<action_base, T>
        {};
    }
}

#endif
