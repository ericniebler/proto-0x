////////////////////////////////////////////////////////////////////////////////////////////////////
// or.hpp
// Contains the behavior of proto::or_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_GRAMMAR_OR_HPP_INCLUDED
#define BOOST_PROTO_CXX11_GRAMMAR_OR_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace extension
            {
                // Handle proto::or_
                template<typename ...BoolActions>
                struct grammar_impl<proto::v5::or_(BoolActions...)>
                {
                    template<typename Expr>
                    struct apply
                      : utility::or_<detail::eval_bool_action_<BoolActions, Expr>...>
                    {};
                };
            }
        }
    }
}

#endif
