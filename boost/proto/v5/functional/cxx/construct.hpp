////////////////////////////////////////////////////////////////////////////////////////////////////
// construct.hpp
// A function object for constructing objects of a specified type.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_FUNCTIONAL_CXX_CONSTRUCT_HPP_INCLUDED
#define BOOST_PROTO_V5_FUNCTIONAL_CXX_CONSTRUCT_HPP_INCLUDED

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
                    // construct
                    template<typename Type>
                    struct construct
                    {
                        template<typename ...Args>
                        constexpr auto operator()(Args &&... args) const
                        BOOST_PROTO_AUTO_RETURN(
                            Type{static_cast<Args &&>(args)...}
                        )
                    };
                }
            }
        }
    }
}

#endif
