////////////////////////////////////////////////////////////////////////////////////////////////////
// utility.hpp
// Proto callables for things found in the std \<utility\> header
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_STD_UTILITY_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_STD_UTILITY_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c std::make_pair() action on its arguments.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c std::make_pair() action on its arguments.
    struct make_pair
    {
        template<typename First, typename Second>
        auto operator()(First &&first, Second &&second) const
        BOOST_PROTO_AUTO_RETURN(
            std::make_pair(std::forward<First>(first), std::forward<Second>(second))
        )
    };

    /// \brief A PolymorphicFunctionObject type that returns
    /// the first element of a std::pair.
    ///
    /// A PolymorphicFunctionObject type that returns
    /// the first element of a std::pair..
    struct first
    {
        template<typename Pair>
        auto operator()(Pair &&pair) const
        BOOST_PROTO_AUTO_RETURN(
            (static_cast<Pair &&>(pair).first) // extra parens are significant!
        )
    };

    /// \brief A PolymorphicFunctionObject type that returns
    /// the second element of a std::pair.
    ///
    /// A PolymorphicFunctionObject type that returns
    /// the second element of a std::pair..
    struct second
    {
        template<typename Pair>
        auto operator()(Pair &&pair) const
        BOOST_PROTO_AUTO_RETURN(
            (static_cast<Pair &&>(pair).second) // extra parens are significant!
        )
    };

}}}

#endif
