////////////////////////////////////////////////////////////////////////////////////////////////////
// cast.hpp
// Function objects for performing the various C++-style casts
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_FUNCTIONAL_CXX_CAST_HPP_INCLUDED
#define BOOST_PROTO_CXX11_FUNCTIONAL_CXX_CAST_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace functional
            {
                namespace cxx
                {
                    ////////////////////////////////////////////////////////////////////////////////////
                    // static_cast_
                    template<typename T>
                    struct static_cast_
                    {
                        template<typename U>
                        constexpr auto operator()(U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T>(static_cast<U &&>(u))
                        )
                    };

                    ////////////////////////////////////////////////////////////////////////////////////
                    // const_cast_
                    template<typename T>
                    struct const_cast_
                    {
                        template<typename U>
                        constexpr auto operator()(U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            const_cast<T>(static_cast<U &&>(u))
                        )
                    };

                    ////////////////////////////////////////////////////////////////////////////////////
                    // dynamic_cast_
                    template<typename T>
                    struct dynamic_cast_
                    {
                        template<typename U>
                        constexpr auto operator()(U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            dynamic_cast<T>(static_cast<U &&>(u))
                        )
                    };

                    ////////////////////////////////////////////////////////////////////////////////////
                    // reinterpret_cast_
                    template<typename T>
                    struct reinterpret_cast_
                    {
                        template<typename U>
                        constexpr auto operator()(U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            reinterpret_cast<T>(static_cast<U &&>(u))
                        )
                    };
                }
            }
        }
    }
}

#endif
