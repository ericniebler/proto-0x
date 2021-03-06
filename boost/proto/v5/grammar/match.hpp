////////////////////////////////////////////////////////////////////////////////////////////////////
// match.hpp
// Contains the behavior of proto::match when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_GRAMMAR_MATCH_HPP_INCLUDED
#define BOOST_PROTO_V5_GRAMMAR_MATCH_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/grammar/basic_grammar.hpp>
#include <boost/proto/v5/grammar/case.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // matches_wrap_
                template<typename Expr, typename Grammar>
                struct matches_wrap_
                  : result_of::matches<Expr, Grammar>
                {
                    using proto_grammar_type = Grammar;
                };
            }

            namespace extension
            {
                // Handle proto::match
                template<typename ...Grammars>
                struct grammar_impl<proto::v5::match(Grammars...)>
                {
                    template<typename Expr>
                    struct apply
                      : utility::or_<detail::matches_wrap_<Expr, Grammars>...>
                    {};
                };
            }
        }
    }
}

#endif
