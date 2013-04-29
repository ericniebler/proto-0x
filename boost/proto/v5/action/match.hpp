////////////////////////////////////////////////////////////////////////////////////////////////////
// match.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_MATCH_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_MATCH_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/case.hpp>
#include <boost/proto/v5/grammar/match.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename T>
                struct is_case_stmt_
                  : std::false_type
                {};

                template<typename Grammar, typename ActionHead, typename ...ActionTail>
                struct is_case_stmt_<case_(*)(Grammar, ActionHead, ActionTail...)>
                  : std::true_type
                {};

                template<typename... ActiveGrammars>
                struct _match
                  : basic_action<_match<ActiveGrammars...>>
                {
                    template<typename Expr, typename ...Rest>
                    constexpr auto operator()(Expr && e, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        as_action_<
                            // This relies on details of how proto::match's grammar behavior is implemented.
                            typename matches<Expr, proto::v5::match(ActiveGrammars...)>::which::proto_grammar_type
                        >()(
                            static_cast<Expr &&>(e)
                          , static_cast<Rest &&>(rest)...
                        )
                    )
                };
            }

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////////
                // action_impl
                template<typename ...ActiveGrammars>
                struct action_impl<match(ActiveGrammars...)>
                  : std::conditional<
                        utility::logical_ops::and_(detail::is_case_stmt_<ActiveGrammars>::value...)
                      , detail::_match<ActiveGrammars...>
                      , not_an_action<match(ActiveGrammars...)>
                    >::type
                {};
            }
        }
    }
}

#endif // BOOST_PROTO_CXX11_ACTION_MATCH_HPP_INCLUDED
