////////////////////////////////////////////////////////////////////////////////////////////////////
// pop_front.hpp
// Proto callables Fusion pop_front
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_FUSION_POP_FRONT_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_FUSION_POP_FRONT_HPP_INCLUDED

#include <type_traits>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { inline namespace cxx11 { namespace functional { namespace fusion
{
    /// \brief A PolymorphicFunctionObject type that invokes the
    /// \c fusion::pop_front() action on its argument.
    ///
    /// A PolymorphicFunctionObject type that invokes the
    /// \c fusion::pop_front() action on its argument. This is
    /// useful for defining a CallableAction like \c pop_front(_)
    /// which removes the first child from a Proto expression node.
    /// Such a basic_action might be used as the first argument to the
    /// \c proto::fold\<\> basic_action; that is, fold all but
    /// the first child.
    struct pop_front
    {
        template<typename Seq>
        auto operator ()(Seq &&seq) const
        BOOST_PROTO_AUTO_RETURN(
            typename boost::fusion::result_of::pop_front<
                typename ::std::remove_reference<Seq>::type
            >::type(
                boost::fusion::next(boost::fusion::begin(static_cast<Seq &&>(seq)))
              , boost::fusion::end(static_cast<Seq &&>(seq))
            )
        )
    };
}}}}}

#endif
