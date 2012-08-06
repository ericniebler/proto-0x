////////////////////////////////////////////////////////////////////////////////////////////////////
// switch.hpp
// Contains the behavior of proto::switch_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_SWITCH_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_SWITCH_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::switch_
        template<typename Expr, typename Cases>
        struct matches<Expr, proto::switch_(Cases)>
          : matches<
                Expr                
              , typename Cases::template case_<typename tag_of<Expr>::type>
            >
        {};

        template<typename Expr, typename Cases, typename Action>
        struct matches<Expr, proto::switch_(Cases, Action)>
          : matches<
                Expr
              , typename Cases::template case_<decltype(action<Action>()(std::declval<Expr>()))>
            >
        {};
    }
}

#endif
