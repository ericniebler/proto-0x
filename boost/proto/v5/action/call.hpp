////////////////////////////////////////////////////////////////////////////////////////////////////
// call.hpp
// Helpers for building callable actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_CALL_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_CALL_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/protect.hpp>
#include <boost/proto/v5/action/env.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            // TODO: can the call action do something to automatically prevent the lookup of
            // of out-of-scope local variables? Consider how function calls in C++ automatically
            // create a new scope that hides the local one.

            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // call_2_
                template<bool TryCall, bool NoPad, typename ...Results>
                struct call_2_
                {
                    template<typename Action, typename ...Args>
                    constexpr auto operator()(
                        Action &&act
                      , Results &&... results
                      , utility::first<utility::any, Results>...
                      , Args &&... args
                    ) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL_IF(TryCall)(static_cast<Action &&>(act))(
                            static_cast<Results &&>(results)...
                          , static_cast<Args &&>(args)...
                        )
                    )
                };

                template<bool TryCall, typename ...Results>
                struct call_2_<TryCall, true, Results...>
                {
                    template<typename Action, typename ...Ts>
                    constexpr auto operator()(Action &&act, Results &&... results, Ts &&...) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL_IF(TryCall)(static_cast<Action &&>(act))(
                            static_cast<Results &&>(results)...
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // call_1_
                template<bool TryCall, typename ...Actions>
                struct call_1_
                {
                    // Handle actions
                    template<
                        typename Action
                      , typename ...Args
                      , BOOST_PROTO_ENABLE_IF(is_action<Action>::value)
                    >
                    constexpr auto operator()(Action &&act, Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL_IF(TryCall)(call_2_<
                            TryCall
                          , (sizeof...(Args) <= sizeof...(Actions))
                          , decltype(call_action_<Actions>()(static_cast<Args &&>(args)...))...
                        >())(
                            static_cast<Action &&>(act)
                          , call_action_<Actions>()(static_cast<Args &&>(args)...)...
                          , static_cast<Args &&>(args)...
                        )
                    )

                    // Handle callables
                    template<
                        typename Fun
                      , typename ...Args
                      , BOOST_PROTO_ENABLE_IF(!is_action<Fun>::value)
                    >
                    constexpr auto operator()(Fun &&fun, Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL_IF(TryCall)(static_cast<Fun &&>(fun))(
                            call_action_<Actions>()(static_cast<Args &&>(args)...)...
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _call
                template<typename Ret, typename ...Actions>
                struct _call
                  : basic_action<_call<Ret, Actions...>>
                {
                    template<typename ...Args, typename Fun = Ret>
                    constexpr auto operator()(Args &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        call_1_<true, Actions...>()(Fun(), static_cast<Args &&>(t)...)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // call_or_construct_1_
                template<typename Ret, typename ...Actions>
                struct call_or_construct_1_
                {
                    template<typename ...Args, typename Fun = Ret>
                    constexpr auto operator()(Args &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        call_1_<false, Actions...>()(
                            Fun()
                          , static_cast<Args &&>(t)...
                        )
                    )

                    template<typename ...Args, typename Obj = Ret
                      , BOOST_PROTO_ENABLE_IF(!v5::is_action<Obj>::value)
                    >
                    constexpr auto operator()(Args &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        Obj{call_action_<Actions>()(static_cast<Args &&>(t)...)...}
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _call_or_construct
                template<typename Ret, typename ...Actions>
                struct _call_or_construct
                  : basic_action<_call_or_construct<Ret, Actions...>>
                {
                    template<typename ...Args>
                    constexpr auto operator()(Args &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(call_or_construct_1_<Ret, Actions...>())(
                            static_cast<Args &&>(t)...
                        )
                    )
                };
            }

            struct call
            {};

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // Handle callable actions
                template<typename Ret, typename ...Actions>
                struct action_impl<Ret(Actions...)>
                  : detail::_call_or_construct<Ret, Actions...>
                {};

                template<typename Ret>
                struct action_impl<call(Ret)>
                  : detail::_call<Ret>
                {};

                template<typename Ret, typename ...Actions>
                struct action_impl<call(Ret(*)(Actions...))>
                  : detail::_call<Ret, Actions...>
                {};
            }
        }
    }
}

#endif
