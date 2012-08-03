///////////////////////////////////////////////////////////////////////////////
// integral_constant.hpp
// A simple basic_action that returns a constant
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_CONSTANT_HPP_INCLUDED
#define BOOST_PROTO_ACTION_CONSTANT_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/base.hpp>

namespace boost
{
    namespace proto
    {
        template<typename T, T Value>
        struct _integral_constant
          : basic_action<_integral_constant<T, Value>>
        {
            template<typename ...Args>
            T operator()(Args &&...) const noexcept(noexcept(T(std::declval<T>())))
            {
                return Value;
            }
        };

        typedef _integral_constant<bool, true> _true;
        typedef _integral_constant<bool, false> _false;
    }
}

#endif