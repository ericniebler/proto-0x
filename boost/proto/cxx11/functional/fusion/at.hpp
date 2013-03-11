////////////////////////////////////////////////////////////////////////////////////////////////////
// at.hpp
// Proto callables Fusion at
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_FUSION_AT_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_FUSION_AT_HPP_INCLUDED

#include <boost/fusion/include/at.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional { namespace fusion
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c fusion::at() accessor on its argument.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c fusion::at() accessor on its argument.
    struct at
    {
        template<typename Seq, typename N>
        auto operator ()(Seq &&seq, N const &) const
        BOOST_PROTO_AUTO_RETURN(
            boost::fusion::at<N>(static_cast<Seq &&>(seq))
        )
    };
}}}}

#endif
