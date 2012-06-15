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
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // or_c
            struct none
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // or_c
            template<bool Head, typename ...List>
            struct or_c
              : std::false_type
            {
                typedef none which;
            };

            template<typename Head, typename ...Tail>
            struct or_c<true, Head, Tail...>
              : std::true_type
            {
                typedef Head which; // which predicate succeeded?
            };

            template<typename Head, typename Next, typename ...Tail>
            struct or_c<false, Head, Next, Tail...>
              : or_c<Next::value, Next, Tail...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_c
            template<bool Head, typename ...List>
            struct and_c
              : std::true_type
            {
                typedef none which;
            };

            template<typename Head, typename ...Tail>
            struct and_c<false, Head, Tail...>
              : std::false_type
            {
                typedef Head which; // which predicate failed?
            };

            template<typename Head, typename Next, typename ...Tail>
            struct and_c<true, Head, Next, Tail...>
              : and_c<Next::value, Next, Tail...>
            {};
        }

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
            // or_ (like mpl::or_, but variadic)
            template<typename ...T>
            struct or_
              : std::false_type
            {
                typedef detail::none which;
            };

            template<typename Head, typename ...T>
            struct or_<Head, T...>
              : detail::or_c<Head::value, Head, T...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // and_  (like mpl::and_, but variadic)
            template<typename ...T>
            struct and_
              : std::true_type
            {
                typedef detail::none which;
            };

            template<typename Head, typename ...T>
            struct and_<Head, T...>
              : detail::and_c<Head::value, Head, T...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // back - fetch the last element of a variadic template pack
            template<typename T0>
            auto back(T0 && t0)
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T0 &&>(t0)
            )

            template<typename T0, typename T1>
            auto back(T0 &&, T1 && t1)
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T1 &&>(t1)
            )

            template<typename T0, typename T1, typename T2>
            auto back(T0 &&, T1 &&, T2 && t2)
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T2 &&>(t2)
            )

            template<typename T0, typename T1, typename T2, typename ...Rest>
            auto back(T0 &&, T1 &&, T2 &&, Rest &&... rest)
            BOOST_PROTO_AUTO_RETURN(
                utility::back(static_cast<Rest &&>(rest)...)
            )

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
