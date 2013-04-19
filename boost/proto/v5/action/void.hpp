////////////////////////////////////////////////////////////////////////////////////////////////////
// void.hpp
// A no-op basic action that returns void.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_VOID_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_VOID_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // _void
                struct _void
                  : basic_action<_void>
                {
                    template<typename ...As>
                    void operator()(As &&...) const
                    {}
                };
            }

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // action_impl
                template<>
                struct action_impl<void()>
                  : detail::_void
                {};
            }
        }
    }
}

#endif
