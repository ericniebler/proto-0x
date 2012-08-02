////////////////////////////////////////////////////////////////////////////////////////////////////
// reverse.hpp
// Proto callables Fusion reverse
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_FUSION_REVERSE_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_FUSION_REVERSE_HPP_INCLUDED

#include <type_traits>
#include <boost/fusion/include/reverse.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c fusion::reverse() action on its argument.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c fusion::reverse() action on its argument. This is
    /// useful for defining a CallableAction like \c reverse(_)
    /// which reverses the order of the children of a Proto
    /// expression node.
    struct reverse
    {
        template<typename Seq>
        auto operator ()(Seq &&seq) const
        BOOST_PROTO_AUTO_RETURN(
            typename fusion::result_of::reverse<typename std::remove_reference<Seq>::type>::type(
                static_cast<Seq &&>(seq)
            )
        )
    };
}}}

#endif
