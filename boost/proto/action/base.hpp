///////////////////////////////////////////////////////////////////////////////
// base.hpp
// Helpers for building Proto actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_BASE_HPP_INCLUDED
#define BOOST_PROTO_ACTION_BASE_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

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
            typedef Action proto_algorithm_type;
        };

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