////////////////////////////////////////////////////////////////////////////////////////////////////
// unpack.hpp
// A transform that allows children to be unpacked into a function call
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_UNPACK_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_UNPACK_HPP_INCLUDED

#include <utility>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // build_result
            template<typename R, typename List, typename... Rest>
            struct build_result;

            template<typename R, typename ...As, typename... Rest>
            struct build_result<R, utility::list<As...>, Rest...>
            {
                typedef R type(As..., Rest...);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_2_
            template<std::size_t I, typename PrimitiveTransform>
            struct expand_pattern_2_
            {
                typedef PrimitiveTransform type;
            };

            template<std::size_t I>
            struct expand_pattern_2_<I, _>
            {
                typedef proto::_child<I> type;
            };

            template<>
            struct expand_pattern_2_<(std::size_t)-1, _>
            {
                typedef proto::_value type;
            };

            template<std::size_t I, typename R, typename ...As>
            struct expand_pattern_2_<I, R(As...)>
            {
                typedef R type(typename expand_pattern_2_<I, As>::type...);
            };

            template<std::size_t I, typename R, typename ...As>
            struct expand_pattern_2_<I, R(*)(As...)>
              : expand_pattern_2_<I, R(As...)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_1_
            template<typename Indices, typename PrimitiveTransform>
            struct expand_pattern_1_
            {
                typedef PrimitiveTransform type;
            };

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(As...)>
            {
                typedef R type(typename expand_pattern_1_<utility::indices<I...>, As>::type...);
            };

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(As..., ...)>
              : build_result<
                    R
                  , utility::pop_back<As...>
                  , typename expand_pattern_2_<
                        I
                      , typename utility::result_of::back<As...>::type
                    >::type...
                >
            {};

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(*)(As...)>
              : expand_pattern_1_<utility::indices<I...>, R(As...)>
            {};

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(*)(As..., ...)>
              : expand_pattern_1_<utility::indices<I...>, R(As..., ...)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////////
            // _unpack_impl
            template<typename Fun>
            struct _unpack_impl
              : proto::transform<_unpack_impl<Fun>>
            {
                template<
                    typename E
                  , typename ...Rest
                  , typename Indices = typename std::conditional<
                        proto::is_terminal<E>::value
                      , utility::indices<(std::size_t)-1>
                      , utility::make_indices<0, proto::arity_of<E>::value>
                    >::type
                  , typename Tfx = as_transform<
                        typename detail::expand_pattern_1_<Indices, Fun>::type
                    >
                >
                auto operator()(E && e, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    Tfx()(static_cast<E &&>(e), static_cast<Rest &&>(rest)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _unpack
        template<typename R, typename ...As>
        struct _unpack<R(As...)>
          : detail::_unpack_impl<R(As...)>
        {};

        template<typename R, typename ...As>
        struct _unpack<R(As..., ...)>
          : detail::_unpack_impl<R(As..., ...)>
        {};
    }
}

#endif
