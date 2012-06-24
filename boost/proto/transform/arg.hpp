///////////////////////////////////////////////////////////////////////////////
// arg.hpp
// Basic transforms for accessing the arguments to the transform.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_ARG_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_ARG_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // _expr
        struct _expr
          : transform<_expr>
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
          : transform<_state>
        {
            template<typename E, typename S, typename ...Rest>
            auto operator()(E &&, S && s, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<S &&>(s)
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _env
        template<typename Tag>
        struct _env
          : transform<_env<Tag>>
        {
            template<typename E, typename S, typename Env, typename ...Rest>
            auto operator()(E &&, S &&, Env && env, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Env &&>(env)[Tag()]
            )
        };

        template<>
        struct _env<void>
          : transform<_env<void>>
        {
            template<typename E, typename S, typename Env, typename ...Rest>
            auto operator()(E &&, S &&, Env && env, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Env &&>(env)
            )
        };
    }
}

#endif
