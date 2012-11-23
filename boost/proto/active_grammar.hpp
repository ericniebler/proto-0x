////////////////////////////////////////////////////////////////////////////////////////////////////
// active_grammar.hpp
// Both a grammar and an action.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTIVE_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_ACTIVE_GRAMMAR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename T>
            struct is_case_stmt
              : std::false_type
            {};

            template<typename Grammar, typename Action>
            struct is_case_stmt<case_(Grammar, Action)>
              : std::true_type
            {};

            template<typename Case>
            struct ensure_case_stmt
            {
                using type = Case;
                static_assert(is_case_stmt<Case>::value, "Expecting case_(G, A) statement in match<>");
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar
        template<typename ActiveGrammar>
        struct active_grammar
          : action<ActiveGrammar>
          , grammar<ActiveGrammar>
        {};

        template<typename... Cases>
        struct match
          : active_grammar<proto::or_(typename detail::ensure_case_stmt<Cases>::type...)>
        {};
    }
}

#endif
