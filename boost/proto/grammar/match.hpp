////////////////////////////////////////////////////////////////////////////////////////////////////
// match.hpp
// Contains the behavior of proto::match when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_MATCH_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_MATCH_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/grammar/grammar.hpp>
#include <boost/proto/grammar/case.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_wrap_
            template<typename Expr, typename Grammar>
            struct matches_wrap_
              : matches<Expr, Grammar>
            {
                typedef Grammar proto_grammar_type;
            };
        }

        // Handle proto::match
        template<typename Expr, typename ...Grammars>
        struct matches<Expr, proto::match(Grammars...)>
          : utility::or_<detail::matches_wrap_<Expr, Grammars>...>
        {};
    }
}

#endif
