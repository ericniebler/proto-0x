///////////////////////////////////////////////////////////////////////////////
// grammar.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        template<typename Grammar>
        struct grammar
          : action<Grammar>
        {
            typedef Grammar proto_grammar_type;
        };
    }
}

#endif
