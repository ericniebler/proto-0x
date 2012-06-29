////////////////////////////////////////////////////////////////////////////////////////////////////
// fold.hpp
// Contains definition of the fold<> and reverse_fold<> transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_FOLD_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_FOLD_HPP_INCLUDED

#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/prior.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            struct proto_detail_fold_adl_
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // fold_2_
            template<typename Cur, typename End, typename State, typename Fun, typename ...Rest
              , BOOST_PROTO_ENABLE_IF(!(fusion::result_of::equal_to<Cur, End>::value))>
            auto fold_2_(proto_detail_fold_adl_, Cur const &cur, End const &end, State const &state, Fun fun, Rest &&...rest)
            BOOST_PROTO_AUTO_RETURN(
                fold_2_(
                    proto_detail_fold_adl_()
                  , fusion::next(cur)
                  , end
                  , fun(fusion::deref(cur), state, static_cast<Rest &&>(rest)...)
                  , fun
                  , static_cast<Rest &&>(rest)...
                )
            )

            template<typename Cur, typename End, typename State, typename Fun, typename ...Rest
              , BOOST_PROTO_ENABLE_IF((fusion::result_of::equal_to<Cur, End>::value))>
            State fold_2_(proto_detail_fold_adl_, Cur const &, End const &, State const &state, Fun, Rest &&...)
            {
                return state;
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // fold_1_
            template<typename Sequence, typename State0, typename Fun, typename ...Rest>
            auto fold_1_(Sequence && seq, State0 const &state0, Fun fun, Rest &&... rest)
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_2_(
                    proto_detail_fold_adl_()
                  , fusion::begin(static_cast<Sequence &&>(seq))
                  , fusion::end(static_cast<Sequence &&>(seq))
                  , state0
                  , fun
                  , static_cast<Rest &&>(rest)...
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // reverse_fold_2_
            template<typename Cur, typename End, typename State, typename Fun, typename ...Rest
              , BOOST_PROTO_ENABLE_IF(!(fusion::result_of::equal_to<Cur, End>::value))>
            auto reverse_fold_2_(proto_detail_fold_adl_, Cur const &cur, End const &end, State const &state, Fun fun, Rest &&...rest)
            BOOST_PROTO_AUTO_RETURN(
                fold_2_(
                    proto_detail_fold_adl_()
                  , cur
                  , fusion::prior(end)
                  , fun(fusion::deref(fusion::prior(end)), state, static_cast<Rest &&>(rest)...)
                  , fun
                  , static_cast<Rest &&>(rest)...
                )
            )

            template<typename Cur, typename End, typename State, typename Fun, typename ...Rest
              , BOOST_PROTO_ENABLE_IF((fusion::result_of::equal_to<Cur, End>::value))>
            State reverse_fold_2_(proto_detail_fold_adl_, Cur const &, End const &, State const &state, Fun, Rest &&...)
            {
                return state;
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // reverse_fold_1_
            template<typename Sequence, typename State0, typename Fun, typename ...Rest>
            auto reverse_fold_1_(Sequence && seq, State0 const &state0, Fun fun, Rest &&... rest)
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_2_(
                    proto_detail_fold_adl_()
                  , fusion::begin(static_cast<Sequence &&>(seq))
                  , fusion::end(static_cast<Sequence &&>(seq))
                  , state0
                  , fun
                  , static_cast<Rest &&>(rest)...
                )
            )
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // fold
        // \brief A PrimitiveTransform that invokes the <tt>fusion::fold\<\></tt>
        // algorithm to accumulate
        template<typename Sequence, typename State0, typename Fun>
        struct fold
          : transform<fold<Sequence, State0, Fun>>
        {
            template<typename E>
            auto operator()(E && e) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_1_(
                    as_transform<Sequence>()(static_cast<E &&>(e))
                  , as_transform<State0>()(static_cast<E &&>(e))
                  , as_transform<Fun>()
                )
            )

            template<typename E, typename S, typename ...Rest>
            auto operator()(E && e, S && s, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                detail::fold_1_(
                    as_transform<Sequence>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , as_transform<State0>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , as_transform<Fun>()
                  , static_cast<Rest &&>(rest)...
                )
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // reverse_fold
        // A PrimitiveTransform that invokes the <tt>fusion::fold\<\></tt>
        // algorithm to accumulate
        template<typename Sequence, typename State0, typename Fun>
        struct reverse_fold
          : transform<reverse_fold<Sequence, State0, Fun>>
        {
            template<typename E>
            auto operator()(E && e) const
            BOOST_PROTO_AUTO_RETURN(
                detail::reverse_fold_1_(
                    as_transform<Sequence>()(static_cast<E &&>(e))
                  , as_transform<State0>()(static_cast<E &&>(e))
                  , as_transform<Fun>()
                )
            )

            template<typename E, typename S, typename ...Rest>
            auto operator()(E && e, S && s, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                detail::reverse_fold_1_(
                    as_transform<Sequence>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , as_transform<State0>()(static_cast<E &&>(e), static_cast<S &&>(s), static_cast<Rest &&>(rest)...)
                  , as_transform<Fun>()
                  , static_cast<Rest &&>(rest)...
                )
            )
        };
    }
}

#endif
