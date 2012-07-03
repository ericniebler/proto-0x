////////////////////////////////////////////////////////////////////////////////////////////////////
// pop_back.hpp
// Proto callables Fusion pop_back
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_FUSION_POP_BACK_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_FUSION_POP_BACK_HPP_INCLUDED

#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/prior.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c fusion::pop_back() algorithm on its argument.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c fusion::pop_back() algorithm on its argument.
    struct pop_back
    {
        template<typename Seq>
        auto operator ()(Seq &&seq) const
        BOOST_PROTO_AUTO_RETURN(
            typename fusion::result_of::pop_back<typename std::remove_reference<Seq>::type>::type(
                fusion::begin(static_cast<Seq &&>(seq))
              , fusion::prior(fusion::end(static_cast<Seq &&>(seq)))
            )
        )
    };
}}}

#endif
