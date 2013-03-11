////////////////////////////////////////////////////////////////////////////////////////////////////
// begin.hpp
// Proto callables for boost::begin()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_RANGE_BEGIN_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_RANGE_BEGIN_HPP_INCLUDED

#include <boost/range/begin.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional { namespace range
{

    // A PolymorphicFunctionObject that wraps boost::begin()
    struct begin
    {
        template<typename Rng>
        auto operator()(Rng &&rng) const
        BOOST_PROTO_AUTO_RETURN(
            boost::begin(static_cast<Rng &&>(rng))
        )
    };

}}}}

#endif
