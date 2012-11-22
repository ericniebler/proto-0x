////////////////////////////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED
#define BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/pass_through.hpp>
#include <boost/proto/action/placeholders.hpp>

namespace boost
{
    namespace proto
    {
        template<typename TerminalTag, typename Value>
        struct action<TerminalTag(Value), typename std::enable_if<detail::is_terminal_tag<TerminalTag>::value>::type>
          : _
        {};

        //template<typename Tag, typename ...Actions>
        //struct action<Tag(Actions...), typename std::enable_if<is_tag<Tag>::value>::type>
        //  : action<pass_through(Actions...)>
        //{};

        //template<typename Tag, typename ...Actions>
        //struct action<Tag(Actions......), typename std::enable_if<is_tag<Tag>::value>::type>
        //  : action<pass_through(Actions......)>
        //{};

        //template<typename ...Actions>
        //struct action<_(Actions...)>
        //  : action<pass_through(Actions...)>
        //{};

        //template<typename ...Actions>
        //struct action<_(Actions......)>
        //  : action<pass_through(Actions......)>
        //{};
    }
}

#endif
