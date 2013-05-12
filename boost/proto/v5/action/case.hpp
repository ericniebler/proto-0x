////////////////////////////////////////////////////////////////////////////////////////////////////
// case.hpp
// Associate a custom basic_action with a grammar rule
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_CASE_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_CASE_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/block.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // case_
                template<typename Grammar, typename ...Actions>
                struct action_impl<case_(Grammar, Actions...)>
                  : detail::_block<Actions...>
                {};

                template<typename Grammar, typename Action>
                struct action_impl<case_(Grammar, Action)>
                  : detail::as_action_<Action>
                {};

                // These two are defined in pass.hpp
                template<typename Tag, typename ...ActiveGrammars>
                struct action_impl<case_(Tag(ActiveGrammars...), pass)>;

                template<typename Tag, typename ...ActiveGrammars>
                struct action_impl<case_(Tag(ActiveGrammars......), pass)>;

                ////////////////////////////////////////////////////////////////////////////////////
                // default_
                template<typename ...Actions>
                struct action_impl<default_(Actions...)>
                  : detail::_block<Actions...>
                {};

                template<typename Action>
                struct action_impl<default_(Action)>
                  : detail::as_action_<Action>
                {};
            }
        }
    }
}

#endif
