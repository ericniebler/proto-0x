////////////////////////////////////////////////////////////////////////////////////////////////////
// size.hpp
// Proto callables for boost::size()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_RANGE_SIZE_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_RANGE_SIZE_HPP_INCLUDED

#include <boost/range/size.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { inline namespace cxx11 { namespace functional { namespace range
{

    // A PolymorphicFunctionObject that wraps boost::size()
    struct size
    {
        template<typename Rng>
        auto operator()(Rng const &rng) const
        BOOST_PROTO_AUTO_RETURN(
            boost::size(rng)
        )
    };

}}}}}

#endif
