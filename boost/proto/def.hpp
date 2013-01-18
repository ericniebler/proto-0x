////////////////////////////////////////////////////////////////////////////////////////////////////
// def.hpp
// Helpers for building Proto grammars and actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DEF_HPP_INCLUDED
#define BOOST_PROTO_DEF_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/grammar/basic_grammar.hpp>
#include <boost/proto/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        struct def
          : detail::as_grammar_<T>
          , detail::as_action_<T>
        {
            static_assert(
                std::is_function<T>::value
              , "Only use proto::def with function types."
            );
        };
    }
}

#endif
