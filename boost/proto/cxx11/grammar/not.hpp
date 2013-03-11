////////////////////////////////////////////////////////////////////////////////////////////////////
// not.hpp
// Contains the behavior of proto::not_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_GRAMMAR_NOT_HPP_INCLUDED
#define BOOST_PROTO_CXX11_GRAMMAR_NOT_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/grammar/basic_grammar.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace extension
            {
                // Handle proto::not_
                template<typename BoolAction>
                struct grammar_impl<proto::cxx11::not_(BoolAction)>
                {
                    template<typename Expr>
                    struct apply
                      : std::integral_constant<
                            bool
                          , !detail::eval_bool_action_<BoolAction, Expr>::value
                        >
                    {};
                };
            }
        }
    }
}

#endif
