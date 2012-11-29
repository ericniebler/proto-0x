////////////////////////////////////////////////////////////////////////////////////////////////////
// and.hpp
// Contains the behavior of proto::and_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_AND_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_AND_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        // Handle proto::and_
        template<typename Expr, typename ...BoolActions>
        struct matches<Expr, proto::and_(BoolActions...)>
          : utility::and_<detail::eval_bool_action_<BoolActions, Expr>...>
        {};
    }
}

#endif
