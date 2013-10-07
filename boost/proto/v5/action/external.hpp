////////////////////////////////////////////////////////////////////////////////////////////////////
// external.hpp
// Contains definition of external actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_EXTERNAL_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_EXTERNAL_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/env.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename RuleName>
                struct _external_
                  : basic_action<_external_<RuleName>>
                {
                    template<typename Expr>
                    constexpr utility::any operator()(Expr &&) const
                    {
                        static_assert(
                            utility::never<Expr>::value
                          , "The proto::external action requires you to pass in an "
                            "environment with mappings from rule names to actions."
                        );
                        return utility::any();
                    }

                    template<typename Expr, typename Env, typename ...Rest, typename Rule = RuleName>
                    constexpr auto operator()(Expr &&e, Env &&env, Rest &&...rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(static_cast<Env &&>(env)(Rule()))(
                            static_cast<Expr &&>(e)
                          , static_cast<Env &&>(env)
                          , static_cast<Rest &&>(rest)...
                        )
                    )
                };
            }

            struct external
            {};

            namespace extension
            {
                template<typename RuleName>
                struct action_impl<case_(RuleName, external)>
                  : detail::_external_<RuleName>
                {};
            }
        }
    }
}

#endif
