////////////////////////////////////////////////////////////////////////////////////////////////////
// switch.hpp
// Contains the behavior of proto::cxx11::switch_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_SWITCH_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_SWITCH_HPP_INCLUDED

#include <utility>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/tags.hpp>
#include <boost/proto/cxx11/matches.hpp>
#include <boost/proto/cxx11/utility.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>
#include <boost/proto/cxx11/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace extension
            {
                // Handle proto::cxx11::switch_
                template<typename Cases>
                struct grammar_impl<proto::cxx11::switch_(Cases)>
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
                struct grammar_impl<proto::cxx11::switch_(Cases, Action)>
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
}

#endif
