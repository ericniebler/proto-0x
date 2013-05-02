////////////////////////////////////////////////////////////////////////////////////////////////////
// end.hpp
// Proto callables for boost::end()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_FUNCTIONAL_RANGE_END_HPP_INCLUDED
#define BOOST_PROTO_V5_FUNCTIONAL_RANGE_END_HPP_INCLUDED

#include <boost/range/end.hpp>
#include <boost/proto/v5/proto_fwd.hpp>

namespace boost { namespace proto { inline namespace v5 { namespace functional { namespace range
{

    // A PolymorphicFunctionObject that wraps boost::end()
    struct end
    {
        template<typename Rng>
        auto operator()(Rng &&rng) const
        BOOST_PROTO_AUTO_RETURN(
            boost::end(static_cast<Rng &&>(rng))
        )
    };

}}}}}

#endif
