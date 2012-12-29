////////////////////////////////////////////////////////////////////////////////////////////////////
// or.hpp
// Contains the behavior of proto::or_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_OR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_OR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace extension
        {
            // Handle proto::or_
            template<typename ...BoolActions>
            struct grammar_impl<proto::or_(BoolActions...)>
            {
                template<typename Expr>
                struct apply
                  : utility::or_<detail::eval_bool_action_<BoolActions, Expr>...>
                {};
            };
        }
    }
}

#endif
