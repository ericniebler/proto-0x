////////////////////////////////////////////////////////////////////////////////////////////////////
// rend.hpp
// Proto callables for boost::rend()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_RANGE_REND_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_RANGE_REND_HPP_INCLUDED

#include <boost/range/rend.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional
{

    // A PolymorphicFunctionObject that wraps boost::rend()
    struct rend
    {
        template<typename Rng>
        auto operator()(Rng &&rng) const
        BOOST_PROTO_AUTO_RETURN(
            boost::rend(static_cast<Rng &&>(rng))
        )
    };

}}}

#endif
