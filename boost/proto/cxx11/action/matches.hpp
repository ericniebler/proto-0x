////////////////////////////////////////////////////////////////////////////////////////////////////
// matches.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_ACTION_MATCHES_HPP_INCLUDED

#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/matches.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // _matches_ basic_action
            template<typename Grammar>
            struct _matches_
              : proto::basic_action<_matches_<Grammar>>
            {
                template<typename Expr, typename... Rest>
                auto operator()(Expr &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::matches<Expr, Grammar>()
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // matches_
        struct matches_
        {};

        namespace extension
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            // action_impl
            template<typename Grammar>
            struct action_impl<matches_(Grammar)>
              : detail::_matches_<Grammar>
            {};
        }
    }
}

#endif
