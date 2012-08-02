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
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // when
        template<typename Grammar, typename Action>
        struct when
          : basic_action<when<Grammar, Action>>
        {
            //typedef Grammar proto_grammar_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                action<Action>()(static_cast<Args &&>(args)...)
            )
        };
    }
}

#endif
