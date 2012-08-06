////////////////////////////////////////////////////////////////////////////////////////////////////
// not.hpp
// Contains the behavior of proto::not_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_NOT_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_NOT_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::not_
        template<typename Expr, typename Grammar>
        struct matches<Expr, proto::not_(Grammar)>
          : std::integral_constant<bool, !matches<Expr, Grammar>::value>
        {};
    }
}

#endif