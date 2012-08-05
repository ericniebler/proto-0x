///////////////////////////////////////////////////////////////////////////////
// matches.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_ACTION_MATCHES_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // _matches basic_action
            template<typename Gram>
            struct _matches
              : proto::basic_action<_matches<Gram>>
            {
                template<typename Expr, typename... Rest>
                auto operator()(Expr &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::matches<Expr, Gram>()
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename Expr>
        struct action<matches_(Expr)>
          : detail::_matches<Expr>
        {};
    }
}

#endif
