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
        template<typename Grammar, typename ...Algo>
        struct action<when(Grammar, Algo...)>
          : detail::_and_<Algo...>
        {};

        template<typename Grammar, typename Algo>
        struct action<when(Grammar, Algo)>
          : action<Algo>
        {};
    }
}

#endif
