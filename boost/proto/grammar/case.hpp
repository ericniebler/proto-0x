////////////////////////////////////////////////////////////////////////////////////////////////////
// case.hpp
// Contains the behavior of proto::when when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_CASE_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_CASE_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>

namespace boost
{
    namespace proto
    {
        template<typename Expr, typename Grammar, typename ...Actions>
        struct matches<Expr, proto::case_(Grammar, Actions...)>
          : matches<Expr, Grammar>
        {};
    }
}

#endif
