///////////////////////////////////////////////////////////////////////////////
// apply.hpp
// Treat the first argument as a callable and the others as arguments to the
// callable
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_APPLY_HPP_INCLUDED
#define BOOST_PROTO_ACTION_APPLY_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/call.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Fun, typename ...Actions>
            struct _apply
              : basic_action<_apply<Fun, Actions...>>
            {
                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::call_1_<Actions...>()(
                        action<Fun>()(static_cast<Args &&>(args)...)
                      , static_cast<Args &&>(args)...
                    )
                )
            };
        }

        template<typename Fun, typename ...Actions, int I>
        struct action<apply(Fun, Actions...), I>
          : detail::_apply<Fun, Actions...>
        {};
    }
}

#endif
