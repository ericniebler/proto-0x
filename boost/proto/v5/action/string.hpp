////////////////////////////////////////////////////////////////////////////////////////////////////
// integral_constant.hpp
// A simple basic_action that returns a constant
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_STRING_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_STRING_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/mpl/string.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            template<int ...Is>
            struct _string
              : basic_action<_string<Is...>>
            {
                template<typename ...Args>
                constexpr char const *operator()(Args &&...) const noexcept
                {
                    return mpl::c_str<mpl::string<Is...>>::value;
                }
            };
        }
    }
}

#endif
