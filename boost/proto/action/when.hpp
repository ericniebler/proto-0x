////////////////////////////////////////////////////////////////////////////////////////////////////
// when.hpp
// Associate a custom basic_action with a grammar rule
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_WHEN_HPP_INCLUDED
#define BOOST_PROTO_ACTION_WHEN_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/and.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // when
        template<typename Grammar, typename ...Action>
        struct action<when(Grammar, Action...)>
          : detail::_and_<Action...>
        {};

        template<typename Grammar, typename Action>
        struct action<when(Grammar, Action)>
          : action<Action>
        {};
    }
}

#endif
