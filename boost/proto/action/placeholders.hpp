////////////////////////////////////////////////////////////////////////////////////////////////////
// placeholders.hpp
// Basic actions for accessing the arguments to the action.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_PLACEHOLDERS_HPP_INCLUDED
#define BOOST_PROTO_ACTION_PLACEHOLDERS_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/placeholders.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/env.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // _expr
        struct _expr
          : basic_action<_expr>
        {
            template<typename E, typename ...Rest>
            auto operator()(E && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<E &&>(e)
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _state
        struct _state
          : basic_action<_state>
        {
            template<typename E, typename Env, typename S, typename ...Rest>
            auto operator()(E &&, Env &&, S && s, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<S &&>(s)
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _data
        struct _data
          : _env_var<data_type>
        {};
    }
}

#endif
