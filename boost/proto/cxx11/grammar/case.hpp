////////////////////////////////////////////////////////////////////////////////////////////////////
// case.hpp
// Contains the behavior of proto::case when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_GRAMMAR_CASE_HPP_INCLUDED
#define BOOST_PROTO_CXX11_GRAMMAR_CASE_HPP_INCLUDED

#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/tags.hpp>
#include <boost/proto/cxx11/matches.hpp>
#include <boost/proto/cxx11/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace extension
            {
                template<typename Grammar, typename ...Actions>
                struct grammar_impl<proto::cxx11::case_(Grammar, Actions...)>
                {
                    template<typename Expr>
                    struct apply
                      : matches<Expr, Grammar>
                    {};
                };
            }
        }
    }
}

#endif
