////////////////////////////////////////////////////////////////////////////////////////////////////
// or.hpp
// Contains the behavior of proto::or_ when used as a grammar element.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_OR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_OR_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_wrap
            template<typename Expr, typename Grammar>
            struct matches_wrap
              : matches<Expr, Grammar>
            {
                typedef Grammar proto_grammar_type;
            };
        }

        // Handle proto::or_
        template<typename Expr, typename ...Grammars>
        struct matches<Expr, proto::or_(Grammars...)>
          : utility::or_<detail::matches_wrap<Expr, Grammars>...>
        {};
    }
}

#endif
