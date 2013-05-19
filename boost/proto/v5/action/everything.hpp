////////////////////////////////////////////////////////////////////////////////////////////////////
// everything.hpp
//
// Copyright 2013 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_EVERYTHING_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_EVERYTHING_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/def.hpp>
#include <boost/proto/v5/action/match.hpp>
#include <boost/proto/v5/action/fold.hpp>
#include <boost/proto/v5/grammar/expr.hpp>
#include <boost/proto/v5/grammar/placeholders.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // reduce_
                // Evaluate BinFun with 2 arguments, or return the second argument if the first
                // argument is "nothing".
                template<typename BinFun>
                struct reduce_
                {
                    template<typename Value>
                    constexpr auto operator()(nothing const &, Value && value) const
                    BOOST_PROTO_AUTO_RETURN(
                        Value(static_cast<Value &&>(value))
                    )

                    template<typename Value0, typename Value1>
                    constexpr auto operator()(Value0 && v0, Value1 && v1) const
                    BOOST_PROTO_AUTO_RETURN(
                        BinFun()(
                            static_cast<Value0 &&>(v0)
                          , static_cast<Value1 &&>(v1)
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _reduce_if_
                template<typename BinOp, typename ...Cases>
                struct _reduce_if_
                  : def<
                        match(
                            case_(*...Case)(
                                as_grammar_<Cases>
                              , reduce_<BinOp>(_state, as_action_<Cases>)
                            )
                          , default_(
                                return_(_state) // avoid returning an rvalue ref to a temporary
                            )
                        )
                    >
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // _everything_
                template<typename BinOp, typename...Cases>
                struct _everything_
                  : def<
                        match(
                            case_(
                                terminal(_)
                              , _reduce_if_<BinOp, Cases...>
                            )
                          , default_(
                                _reduce_if_<BinOp, Cases...>(
                                    _
                                  , _env
                                  , fold(_, _state, _everything_<BinOp, Cases...>)
                                )
                            )
                        )
                    >
                {};
            }

            struct everything
            {};

            struct nothing
            {};

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // e.g. everything(plus, case_(X, Y), case_(A, B))
                template<typename BinOp, typename...Cases>
                struct action_impl<everything(BinOp, Cases...)>
                  : detail::as_action_<detail::_everything_<BinOp, Cases...>(_, _env, nothing())>
                {};
            }
        }
    }
}

#endif
