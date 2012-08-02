////////////////////////////////////////////////////////////////////////////////////////////////////
// fold.hpp
// Contains definition of the _fold<> and _reverse_fold<> actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_FOLD_HPP_INCLUDED
#define BOOST_PROTO_ACTION_FOLD_HPP_INCLUDED

#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/prior.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Fun>
            struct fold_impl_
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // fold_2_
                template<typename Cur, typename End, typename State, typename ...Rest
                  , typename Impl = fold_impl_<Fun>
                  , BOOST_PROTO_ENABLE_IF(!(fusion::result_of::equal_to<Cur, End>::value))>
                static auto fold_2_(Cur const &cur, End const &end, State const &state, Rest &&...rest)
                BOOST_PROTO_AUTO_RETURN(
                    Impl::fold_2_(
                        fusion::next(cur)
                      , end
                      , action<Fun>()(
                            fusion::deref(cur)
                          , state
                          , static_cast<Rest &&>(rest)...
                        )
                      , static_cast<Rest &&>(rest)...
                    )
                )

                template<typename Cur, typename End, typename State, typename ...Rest
                  , BOOST_PROTO_ENABLE_IF((fusion::result_of::equal_to<Cur, End>::value))>
                static State fold_2_(Cur const &, End const &, State const &state, Rest &&...)
                {
                    return state;
                }

                ////////////////////////////////////////////////////////////////////////////////////
                // fold_1_
                // TODO: Optimize when Sequence == args<T...>
                template<typename Sequence, typename State0, typename ...Rest>
                static auto fold_1_(Sequence && seq, State0 const &state0, Rest &&... rest)
                BOOST_PROTO_AUTO_RETURN(
                    fold_impl_<Fun>::fold_2_(
                        fusion::begin(static_cast<Sequence &&>(seq))
                      , fusion::end(static_cast<Sequence &&>(seq))
                      , state0
                      , static_cast<Rest &&>(rest)...
                    )
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // reverse_fold_2_
                template<typename Cur, typename End, typename State, typename ...Rest
                  , typename Impl = fold_impl_<Fun>
                  , BOOST_PROTO_ENABLE_IF(!(fusion::result_of::equal_to<Cur, End>::value))>
                static auto reverse_fold_2_(Cur const &cur, End const &end, State const &state, Rest &&...rest)
                BOOST_PROTO_AUTO_RETURN(
                    Impl::reverse_fold_2_(
                        cur
                      , fusion::prior(end)
                      , action<Fun>()(
                            fusion::deref(fusion::prior(end))
                          , state
                          , static_cast<Rest &&>(rest)...
                        )
                      , static_cast<Rest &&>(rest)...
                    )
                )

                template<typename Cur, typename End, typename State, typename ...Rest
                  , BOOST_PROTO_ENABLE_IF((fusion::result_of::equal_to<Cur, End>::value))>
                static State reverse_fold_2_(Cur const &, End const &, State const &state, Rest &&...)
                {
                    return state;
                }

                ////////////////////////////////////////////////////////////////////////////////////
                // reverse_fold_1_
                // TODO: Optimize when Sequence == args<T...>
                template<typename Sequence, typename State0, typename ...Rest>
                static auto reverse_fold_1_(Sequence && seq, State0 const &state0, Rest &&... rest)
                BOOST_PROTO_AUTO_RETURN(
                    fold_impl_<Fun>::reverse_fold_2_(
                        fusion::begin(static_cast<Sequence &&>(seq))
                      , fusion::end(static_cast<Sequence &&>(seq))
                      , state0
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////
        // _fold
        // \brief A BasicAction that invokes the <tt>fusion::_fold\<\></tt>
        // action to accumulate
        template<typename Sequence, typename State0, typename Fun>
        struct _fold
          : basic_action<_fold<Sequence, State0, Fun>>
        {
            template<typename E>
            auto operator()(E && e) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_impl_<Fun>::fold_1_(
                    action<Sequence>()(static_cast<E &&>(e))
                  , action<State0>()(static_cast<E &&>(e))
                )
            )

            template<typename E, typename S, typename ...Rest>
            auto operator()(E && e, S && s, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_impl_<Fun>::fold_1_(
                    action<Sequence>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , action<State0>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , static_cast<Rest &&>(rest)...
                )
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////
        // _reverse_fold
        // A BasicAction that invokes the <tt>fusion::fold\<\></tt>
        // action to accumulate
        template<typename Sequence, typename State0, typename Fun>
        struct _reverse_fold
          : basic_action<_reverse_fold<Sequence, State0, Fun>>
        {
            template<typename E>
            auto operator()(E && e) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_impl_<Fun>::reverse_fold_1_(
                    action<Sequence>()(static_cast<E &&>(e))
                  , action<State0>()(static_cast<E &&>(e))
                )
            )

            template<typename E, typename S, typename ...Rest>
            auto operator()(E && e, S && s, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_impl_<Fun>::reverse_fold_1_(
                    action<Sequence>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , action<State0>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , static_cast<Rest &&>(rest)...
                )
            )
        };
    }
}

#endif
