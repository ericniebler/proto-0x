////////////////////////////////////////////////////////////////////////////////////////////////////
// switch.hpp
// Contains the behavior of proto::switch_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_GRAMMAR_SWITCH_HPP_INCLUDED
#define BOOST_PROTO_V5_GRAMMAR_SWITCH_HPP_INCLUDED

#include <utility>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace extension
            {
                // Handle proto::switch_
                template<typename Cases>
                struct grammar_impl<proto::v5::switch_(Cases)>
                {
                    template<typename Expr>
                    struct apply
                      : matches<
                            Expr
                          , typename Cases::template case_<typename result_of::tag_of<Expr>::type>
                        >
                    {};
                };

                template<typename Cases, typename Action>
                struct grammar_impl<proto::v5::switch_(Cases, Action)>
                {
                    template<typename Expr>
                    struct apply
                      : matches<
                            Expr
                          , typename Cases::template case_<decltype(detail::call_action_<Action>()(std::declval<Expr>()))>
                        >
                    {};
                };
            }
        }
    }
}

#endif
