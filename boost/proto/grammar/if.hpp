////////////////////////////////////////////////////////////////////////////////////////////////////
// if.hpp
// Contains the behavior of proto::if_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_IF_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_IF_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::if_ in a matching context
        template<typename Expr, typename If>
        struct matches<Expr, proto::if_(If)>
          : detail::eval_bool_action_<If, Expr>
        {};
    }
}

#endif
