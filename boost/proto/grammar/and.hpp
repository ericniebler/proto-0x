////////////////////////////////////////////////////////////////////////////////////////////////////
// and.hpp
// Contains the behavior of proto::and_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_AND_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_AND_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::and_
        template<typename Expr, typename ...Grammars>
        struct matches<Expr, proto::and_(Grammars...)>
          : utility::and_<matches<Expr, Grammars>...>
        {};
    }
}

#endif
