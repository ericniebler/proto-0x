////////////////////////////////////////////////////////////////////////////////////////////////////
// not.hpp
// Contains the behavior of proto::not_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_NOT_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_NOT_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/grammar/grammar.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::not_
        template<typename Expr, typename BoolAction>
        struct matches<Expr, proto::not_(BoolAction)>
          : std::integral_constant<
                bool
              , !detail::eval_bool_action_<BoolAction, Expr>::value
            >
        {};
    }
}

#endif
