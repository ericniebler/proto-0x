////////////////////////////////////////////////////////////////////////////////////////////////////
// empty.hpp
// Proto callables for boost::empty()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_FUNCTIONAL_RANGE_EMPTY_HPP_INCLUDED
#define BOOST_PROTO_CXX11_FUNCTIONAL_RANGE_EMPTY_HPP_INCLUDED

#include <boost/range/empty.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { inline namespace cxx11 { namespace functional { namespace range
{

    // A PolymorphicFunctionObject that wraps boost::empty()
    struct empty
    {
        template<typename Rng>
        bool operator()(Rng const &rng) const noexcept(noexcept(boost::empty(rng)))
        {
            return boost::empty(rng);
        }
    };

}}}}}

#endif
