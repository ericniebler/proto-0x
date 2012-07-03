////////////////////////////////////////////////////////////////////////////////////////////////////
// push_back.hpp
// Proto callables Fusion push_back
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_FUSION_PUSH_BACK_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_FUSION_PUSH_BACK_HPP_INCLUDED

#include <boost/fusion/include/push_back.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c fusion::push_back() algorithm on its argument.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c fusion::push_back() algorithm on its argument.
    struct push_back
    {
        template<typename Seq, typename T>
        auto operator ()(Seq &&seq, T &&t) const
        BOOST_PROTO_AUTO_RETURN(
            fusion::push_back(static_cast<Seq &&>(seq), static_cast<T &&>(t))
        )
    };
}}}

#endif
