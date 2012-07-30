///////////////////////////////////////////////////////////////////////////////
// base.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/mpl/identity.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // call_2_
            template<bool NoPad, typename ...Results>
            struct call_2_
            {
                template<typename Transform, typename ...Ts>
                auto operator()(Transform &&tfx, Results &&... results, utility::first<utility::any, Results>..., Ts &&... ts) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Transform &&>(tfx)(static_cast<Results &&>(results)..., static_cast<Ts &&>(ts)...)
                )
            };

            template<typename ...Results>
            struct call_2_<true, Results...>
            {
                template<typename Transform, typename ...Ts>
                auto operator()(Transform &&tfx, Results &&... results, Ts &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Transform &&>(tfx)(static_cast<Results &&>(results)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_1_
            template<bool IsTransform, typename ...Args>
            struct call_1_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle transforms.
            template<typename ...Args>
            struct call_1_<true, Args...>
            {
                template<typename Transform, typename ...T>
                auto operator()(Transform &&tfx, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    call_2_<
                        (sizeof...(T) <= sizeof...(Args))
                      , decltype(as_transform<Args>()(static_cast<T &&>(t)...))...
                    >()(static_cast<Transform &&>(tfx), as_transform<Args>()(static_cast<T &&>(t)...)..., static_cast<T &&>(t)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle callables and object constructions.
            template<typename ...Args>
            struct call_1_<false, Args...>
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // Handle callables
                template<typename Fun, typename ...T>
                auto operator()(Fun &&fun, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Fun &&>(fun)(as_transform<Args>()(static_cast<T &&>(t)...)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // build_result_
            template<typename R, typename List, typename... Rest>
            struct build_result_;

            template<typename R, typename ...As, typename... Rest>
            struct build_result_<R, utility::list<As...>, Rest...>
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
            struct expand_pattern_1_;

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(As......)>
              : build_result_<
                    R
                  , utility::pop_back<As...>
                  , typename expand_pattern_2_<
                        I
                      , typename utility::result_of::back<As...>::type
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

            template<typename Transform, typename ...Transforms>
            struct collect_pack_transforms_<pack(Transform), void(Transforms...)>
            {
                typedef void type(Transforms..., as_transform<Transform>);
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
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform_base
        struct transform_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform
        // CRTP base class for all transforms
        template<typename Transform>
        struct transform
          : transform_base
        {
            typedef Transform proto_transform_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_transform
        template<typename T>
        struct is_transform
          : std::is_base_of<transform_base, T>
        {};

        template<typename T>
        struct is_transform<T &>
          : std::is_base_of<transform_base, T>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////
        // _void
        struct _void
          : transform<_void>
        {
            template<typename ...T>
            utility::void_type const &operator()(T &&...) const noexcept
            {
                return utility::void_;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename T, int>
        struct as_transform
          : T::proto_transform_type
        {};

        template<typename T, int I>
        struct as_transform<as_transform<T>, I>
          : as_transform<T>
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(*)(Args...), I>
          : as_transform<Ret(Args...), I>
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(*)(Args......), I>
          : as_transform<Ret(Args......), I>
        {};

        // Handle callable transforms
        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(Args...), I>
          : transform<as_transform<Ret(Args...), I>>
        {
            template<typename ...T, typename X = Ret>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                detail::call_1_<is_transform<X>::value, Args...>()(X(), static_cast<T &&>(t)...)
            )
        };

        // Handle transforms with pack expansions
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(Tfxs......), I>
          : transform<as_transform<Ret(Tfxs......), I>>
        {
            typedef
                typename detail::collect_pack_transforms_<
                    typename utility::result_of::back<Tfxs...>::type
                >::type
            transforms_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                as_transform<
                    typename detail::expand_pattern_1_<
                        typename detail::compute_indices_1_<transforms_type, Args...>::type
                      , Ret(Tfxs......)
                    >::type
                >()(static_cast<Args &&>(args)...)
            )
        };
    }
}

#endif
