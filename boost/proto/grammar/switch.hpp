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
#include <boost/proto/action/basic_action.hpp>
#include <boost/proto/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace extension
        {
            // Handle proto::switch_
            template<typename Cases>
            struct grammar_impl<proto::switch_(Cases)>
            {
                template<typename Expr>
                struct apply
                  : matches<
                        Expr
                      , typename Cases::template case_<typename tag_of<Expr>::type>
                    >
                {};
            };

            template<typename Cases, typename Action>
            struct grammar_impl<proto::switch_(Cases, Action)>
            {
                template<typename Expr>
                struct apply
                  : matches<
                        Expr
                      , typename Cases::template case_<decltype(detail::as_action_<Action>()(std::declval<Expr>()))>
                    >
                {};
            };
        }
    }
}

#endif
