////////////////////////////////////////////////////////////////////////////////////////////////////
// matches.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_MATCHES_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // _matches_ basic_action
                template<typename Grammar>
                struct _matches_
                  : proto::v5::basic_action<_matches_<Grammar>>
                {
                    template<typename Expr, typename... Rest>
                    constexpr auto operator()(Expr &&, Rest &&...) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::matches<Expr, Grammar>()
                    )
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_
            struct matches_
            {};

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // action_impl
                template<typename Grammar>
                struct action_impl<matches_(Grammar)>
                  : detail::_matches_<Grammar>
                {};
            }
        }
    }
}

#endif
