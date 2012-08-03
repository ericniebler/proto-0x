///////////////////////////////////////////////////////////////////////////////
// call.hpp
// Helpers for building callable actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_CALL_HPP_INCLUDED
#define BOOST_PROTO_ACTION_CALL_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/mpl/identity.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/protect.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // call_2_
            template<bool NoPad, typename ...Results>
            struct call_2_
            {
                template<typename Action, typename ...Args>
                auto operator()(
                    Action &&tfx
                  , Results &&... results
                  , utility::first<utility::any, Results>...
                  , Args &&... args
                ) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(tfx)(
                        static_cast<Results &&>(results)...
                      , static_cast<Args &&>(args)...
                    )
                )
            };

            template<typename ...Results>
            struct call_2_<true, Results...>
            {
                template<typename Action, typename ...Ts>
                auto operator()(Action &&tfx, Results &&... results, Ts &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Action &&>(tfx)(static_cast<Results &&>(results)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_1_
            template<typename ...Actions>
            struct call_1_
            {
                // Handle actions
                template<
                    typename Action
                  , typename ...Args
                  , BOOST_PROTO_ENABLE_IF(is_action<Action>::value)
                >
                auto operator()(Action &&tfx, Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    call_2_<
                        (sizeof...(Args) <= sizeof...(Actions))
                      , decltype(action<Actions>()(static_cast<Args &&>(args)...))...
                    >()(
                        static_cast<Action &&>(tfx)
                      , action<Actions>()(static_cast<Args &&>(args)...)...
                      , static_cast<Args &&>(args)...
                    )
                )

                // Handle callables
                template<
                    typename Fun
                  , typename ...Args
                  , BOOST_PROTO_ENABLE_IF(!is_action<Fun>::value)
                >
                auto operator()(Fun &&fun, Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Fun &&>(fun)(action<Actions>()(static_cast<Args &&>(args)...)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _call
            template<typename Ret, typename ...Actions>
            struct _call
              : basic_action<_call<Ret, Actions...>>
            {
                template<typename ...Args, typename Fun = Ret>
                auto operator()(Args &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    call_1_<Actions...>()(Fun(), static_cast<Args &&>(t)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename Ret, typename ...Actions, int I>
        struct action<Ret(*)(Actions...), I>
          : action<Ret(Actions...), I>
        {};

        // Handle callable actions
        template<typename Ret, typename ...Actions, int I>
        struct action<Ret(Actions...), I>
          : detail::_call<Ret, Actions...>
        {};
    }
}

#endif