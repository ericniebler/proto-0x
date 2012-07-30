///////////////////////////////////////////////////////////////////////////////
// unpack.hpp
// Make as_transform work with unpacking patterns.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_UNPACK_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_UNPACK_HPP_INCLUDED

#include <cstddef>
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
            // build_result_
            template<typename Ret, typename List, typename... Rest>
            struct build_result_;

            template<typename Ret, typename ...Tfxs, typename... Rest>
            struct build_result_<Ret, utility::list<Tfxs...>, Rest...>
            {
                typedef Ret type(Tfxs..., Rest...);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_2_
            template<std::size_t I, typename PrimitiveTransform>
            struct expand_pattern_2_
            {
                typedef PrimitiveTransform type;
            };

            template<std::size_t I, typename T>
            struct expand_pattern_2_<I, pack(T)>
            {
                typedef proto::_child<I> type(T);
            };

            template<typename T>
            struct expand_pattern_2_<(std::size_t)-1, pack(T)>
            {
                typedef proto::_value type(T);
            };

            template<std::size_t I, typename Ret, typename ...Tfxs>
            struct expand_pattern_2_<I, Ret(Tfxs...)>
            {
                typedef Ret type(typename expand_pattern_2_<I, Tfxs>::type...);
            };

            template<std::size_t I, typename Ret, typename ...Tfxs>
            struct expand_pattern_2_<I, Ret(*)(Tfxs...)>
              : expand_pattern_2_<I, Ret(Tfxs...)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_1_
            template<typename Indices, typename PrimitiveTransform>
            struct expand_pattern_1_;

            template<std::size_t ...I, typename Ret, typename ...Tfxs>
            struct expand_pattern_1_<utility::indices<I...>, Ret(Tfxs......)>
              : build_result_<
                    Ret
                  , utility::pop_back<Tfxs...>
                  , typename expand_pattern_2_<
                        I
                      , typename utility::result_of::back<Tfxs...>::type
                    >::type...
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // collect_pack_transforms_
            template<typename UnpackingPattern, typename Transforms = void()>
            struct collect_pack_transforms_
            {
                typedef Transforms type;
            };

            // Note: Do *NOT* recurse into vararg functions. Those are nested pack transforms,
            // which should not be considered together with this one.
            template<typename Ret, typename Head, typename ...Tail, typename Transforms>
            struct collect_pack_transforms_<Ret(Head, Tail...), Transforms>
              : collect_pack_transforms_<
                    Ret(Tail...)
                  , typename collect_pack_transforms_<Head, Transforms>::type
                >
            {};

            template<typename Ret, typename Head, typename ...Tail, typename Transforms>
            struct collect_pack_transforms_<Ret(*)(Head, Tail...), Transforms>
              : collect_pack_transforms_<Ret(Head, Tail...), Transforms>
            {};

            template<typename Tfx, typename ...Transforms>
            struct collect_pack_transforms_<pack(Tfx), void(Transforms...)>
            {
                typedef void type(Transforms..., as_transform<Tfx>);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_2_
            template<std::size_t Arity0, std::size_t Arity1 = Arity0>
            struct compute_indices_2_
              : std::conditional<
                    Arity0 == 0
                  , utility::indices<(std::size_t)-1>
                  , utility::make_indices<0, Arity0>
                >
            {
                static_assert(
                    Arity0 == Arity1
                  , "Two pack expressions in unpacking pattern have different arities"
                );
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_1_
            template<typename Transforms, typename ...Args>
            struct compute_indices_1_
            {
                static_assert(
                    utility::never<Transforms>::value
                  , "No pack expression found in unpacking pattern. Use proto::pack(<transform>) "
                    "to designate a transform that returns the expression you want to unpack; "
                    "e.g., proto::pack(_) unpacks the current expression."
                );
            };

            template<typename Head, typename...Tail, typename ...Args>
            struct compute_indices_1_<void(Head, Tail...), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef compute_indices_1_<void(Tail...), Args...> tail;
                typedef typename compute_indices_2_<arity::value, tail::arity::value>::type type;
            };

            template<typename Head, typename ...Args>
            struct compute_indices_1_<void(Head), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef typename compute_indices_2_<arity::value>::type type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _unpack
            template<typename Ret, typename ...Tfxs>
            struct _unpack
              : transform<_unpack<Ret, Tfxs...>>
            {
                typedef
                    typename collect_pack_transforms_<
                        typename utility::result_of::back<Tfxs...>::type
                    >::type
                transforms_type;

                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    as_transform<
                        typename expand_pattern_1_<
                            typename compute_indices_1_<transforms_type, Args...>::type
                          , Ret(Tfxs......)
                        >::type
                    >()(static_cast<Args &&>(args)...)
                )
            };
        }

        // Usage: construct(wrap<proto::_value>(proto::_value))
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(*)(Tfxs......), I>
          : as_transform<Ret(Tfxs......), I>
        {};

        // Handle transforms with pack expansions
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(Tfxs......), I>
          : detail::_unpack<Ret, Tfxs...>
        {};
    }
}

#endif
