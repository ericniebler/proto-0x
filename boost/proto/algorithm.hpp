///////////////////////////////////////////////////////////////////////////////
// algorithm.hpp
// Both a grammar and an action.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ALGORITHM_HPP_INCLUDED
#define BOOST_PROTO_ALGORITHM_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar
        template<typename Grammar>
        struct algorithm
          : action<Grammar>
        {
            typedef Grammar proto_grammar_type;
        };
    }
}

#endif
