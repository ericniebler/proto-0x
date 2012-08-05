///////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED
#define BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/pass_through.hpp>

namespace boost
{
    namespace proto
    {
        template<typename Tag, typename ...Actions>
        struct action<Tag(Actions...), typename std::enable_if<is_tag<Tag>::value>::type>
          : action<pass_through(Actions...)>
        {};

        template<typename Tag, typename ...Actions>
        struct action<Tag(Actions......), typename std::enable_if<is_tag<Tag>::value>::type>
          : action<pass_through(Actions......)>
        {};

        template<typename Tag, typename Value>
        struct action<tag::nullary_expr(Tag, Value)>
          : action<pass_through(Value)>
        {};

        template<typename Tag, typename Action>
        struct action<tag::unary_expr(Tag, Action)>
          : action<pass_through(Action)>
        {};

        template<typename Tag, typename Tfx0, typename Tfx1>
        struct action<tag::binary_expr(Tag, Tfx0, Tfx1)>
          : action<pass_through(Tfx0, Tfx1)>
        {};

        template<typename Tag, typename ...Actions>
        struct action<tag::nary_expr(Tag, Actions...)>
          : action<pass_through(Actions...)>
        {};
    }
}

#endif
