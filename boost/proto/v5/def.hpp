////////////////////////////////////////////////////////////////////////////////////////////////////
// def.hpp
// Helpers for building Proto grammars and actions.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_DEF_HPP_INCLUDED
#define BOOST_PROTO_V5_DEF_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/grammar/basic_grammar.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                struct def_base
                {};
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct def
              : detail::def_base
              , detail::as_grammar_<T>
              , detail::as_action_<T>
            {
                static_assert(
                    std::is_function<T>::value
                  , "Only use proto::def with function types."
                );
            };
        }
    }
}

#endif
