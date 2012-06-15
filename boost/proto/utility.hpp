///////////////////////////////////////////////////////////////////////////////
// utility.hpp
// Generally useful stuff.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_UTILITY_HPP_INCLUDED
#define BOOST_PROTO_UTILITY_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // any
            struct any
            {
                template<typename T>
                constexpr any(T const &) noexcept
                {}
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // static_const
            template<typename T>
            struct static_const
            {
                static constexpr T value = T();
            };

            template<typename T>
            constexpr T static_const<T>::value;

            ////////////////////////////////////////////////////////////////////////////////////////
            // logical_and
            inline constexpr bool logical_and()
            {
                return true;
            }

            template<typename ...Bool>
            inline constexpr bool logical_and(bool b0, Bool... rest)
            {
                return b0 && utility::logical_and(rest...);
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // logical_or
            inline constexpr bool logical_or()
            {
                return false;
            }

            template<typename ...Bool>
            inline constexpr bool logical_or(bool b0, Bool... rest)
            {
                return b0 || utility::logical_or(rest...);
            }
        }
    }
}

#endif
