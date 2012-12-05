////////////////////////////////////////////////////////////////////////////////////////////////////
// match.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_MATCH_HPP_INCLUDED
#define BOOST_PROTO_ACTION_MATCH_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/case.hpp>
#include <boost/proto/grammar/grammar.hpp>
#include <boost/proto/grammar/match.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename T>
            struct is_case_stmt_
              : std::false_type
            {};

            template<typename Grammar, typename Action>
            struct is_case_stmt_<case_(*)(Grammar, Action)>
              : std::true_type
            {};

            template<typename... ActiveGrammars>
            struct _match
              : basic_action<_match<ActiveGrammars...>>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && e, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<
                        // This relies on details of how proto::match's grammar behavior is implemented.
                        typename matches<Expr, proto::match(ActiveGrammars...)>::which::proto_grammar_type
                    >()(
                        static_cast<Expr &&>(e)
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename ...ActiveGrammars>
        struct action<match(ActiveGrammars...)>
          : detail::_match<ActiveGrammars...>
          , grammar<match(ActiveGrammars...)>
        {
            static_assert(
                utility::logical_ops::and_(detail::is_case_stmt_<ActiveGrammars>::value...)
              , "Expected case_ statement"
            );
        };
    }
}

#endif // BOOST_PROTO_ACTION_MATCH_HPP_INCLUDED
