////////////////////////////////////////////////////////////////////////////////////////////////////
// apply.hpp
// Treat the first argument as a callable and the others as arguments to the
// callable
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_APPLY_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_APPLY_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/call.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename Fun, typename ...Actions>
                struct _apply
                  : basic_action<_apply<Fun, Actions...>>
                {
                    template<typename ...Args>
                    constexpr auto operator()(Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(detail::call_1_<true, Actions...>())(
                            call_action_<Fun>()(static_cast<Args &&>(args)...)
                          , static_cast<Args &&>(args)...
                        )
                    )
                };
            }

            struct apply
            {};

            namespace extension
            {
                template<typename Fun, typename ...Actions>
                struct action_impl<apply(Fun, Actions...)>
                  : detail::_apply<Fun, Actions...>
                {};
            }
        }
    }
}

#endif
