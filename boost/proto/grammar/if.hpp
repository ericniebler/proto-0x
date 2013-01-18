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
#include <boost/proto/action/basic_action.hpp>
#include <boost/proto/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace extension
        {
            // Handle proto::if_ in a matching context
            template<typename If>
            struct grammar_impl<proto::if_(If)>
            {
                template<typename Expr>
                struct apply
                  : detail::eval_bool_action_<If, Expr>
                {};
            };
        }
    }
}

#endif
