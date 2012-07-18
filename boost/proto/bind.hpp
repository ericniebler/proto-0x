///////////////////////////////////////////////////////////////////////////////
// bind.hpp
// Treat the first argument as a callable and the others as arguments to the
// callable
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_BIND_HPP_INCLUDED
#define BOOST_PROTO_BIND_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        struct bind
        {
            template<typename Fun, typename ...Ts>
            auto operator()(Fun && fun, Ts &&... ts) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Fun &&>(fun)(static_cast<Ts &&>(ts)...)
            )
        };
    }
}

#endif
