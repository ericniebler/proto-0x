////////////////////////////////////////////////////////////////////////////////////////////////////
// if.hpp
// Contains the behavior of proto::if_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_IF_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_IF_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

#define BOOST_PROTO_BOOL_ACTION(If, Expr)                                                           \
    static_cast<bool>(                                                                              \
        std::remove_reference<                                                                      \
            decltype(proto::action<If>()(std::declval<Expr>()))                                     \
        >::type::value                                                                              \
    )                                                                                               \
    /**/

namespace boost
{
    namespace proto
    {
        // Handle proto::if_
        template<typename Expr, typename If>
        struct matches<Expr, proto::if_(If)>
          : std::integral_constant<bool, BOOST_PROTO_BOOL_ACTION(If, Expr)>
        {};

        template<typename Expr, typename If, typename Then>
        struct matches<Expr, proto::if_(If, Then)>
          : std::conditional<
                BOOST_PROTO_BOOL_ACTION(If, Expr)
              , matches<Expr, Then>
              , std::false_type
            >::type
        {};

        template<typename Expr, typename If, typename Then, typename Else>
        struct matches<Expr, proto::if_(If, Then, Else)>
          : std::conditional<
                BOOST_PROTO_BOOL_ACTION(If, Expr)
              , matches<Expr, Then>
              , matches<Expr, Else>
            >::type
        {};
    }
}

#undef BOOST_PROTO_BOOL_ACTION

#endif
