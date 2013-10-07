////////////////////////////////////////////////////////////////////////////////////////////////////
// throw.hpp
// Function object for throwing objects
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_FUNCTIONAL_CXX_THROW_HPP_INCLUDED
#define BOOST_PROTO_V5_FUNCTIONAL_CXX_THROW_HPP_INCLUDED

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
                    // throw_
                    struct throw_
                    {
                        template<typename U>
                        [[noreturn]] void operator()(U && u) const
                        {
                            throw static_cast<U &&>(u);
                        }
                    };
                }
            }
        }
    }
}

#endif
