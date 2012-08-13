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
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
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
            typedef typename T::proto_action_type proto_action_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                BOOST_PROTO_TRY_CALL(proto_action_type())(static_cast<Args &&>(args)...)
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
