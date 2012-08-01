///////////////////////////////////////////////////////////////////////////////
// construct.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_CONSTRUCT_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_CONSTRUCT_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>
#include <boost/proto/transform/protect.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename R, typename ...Args>
            struct make_1_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // nested_type_
            template<typename T>
            typename T::type nested_type_(int);

            template<typename T>
            T nested_type_(long);

            ////////////////////////////////////////////////////////////////////////////////////////
            // invoke_if_
            template<typename T, bool Applied>
            struct invoke_if_
            {
                typedef decltype(detail::nested_type_<T>(1)) type;
                typedef std::true_type applied;
            };

            template<typename T>
            struct invoke_if_<T, false>
            {
                typedef T type;
                typedef std::false_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_3_
            template<bool IsTransform, typename R, typename ...Args>
            struct make_3_
            {
                typedef decltype(utility::by_val()(R()(std::declval<Args>()...))) type;
                typedef std::true_type applied;
            };

            template<typename R, typename ...Args>
            struct make_3_<false, R, Args...>
            {
                typedef R type;
                typedef std::false_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_2_
            template<typename R, typename ...Args>
            struct make_2_
              : make_1_<R, Args...>
            {
                static_assert(!std::is_pointer<R>::value, "ptr to function?");
            };

            template<typename R, typename ...A, typename ...Args>
            struct make_2_<R(A...), Args...>
            {
                typedef decltype(utility::by_val()(as_transform<R(A...)>()(std::declval<Args>()...))) type;
                typedef std::true_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_1_
            template<typename R, typename ...Args>
            struct make_1_
              : make_3_<is_transform<R>::value, R, Args...>
            {};

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_1_<R<A...>, Args...>
              : invoke_if_<
                    R<typename make_2_<A, Args...>::type...>
                  , utility::logical_ops::or_(make_2_<A, Args...>::applied::value...)
                >
            {};

            template<typename R, int I, typename ...Args>
            struct make_1_<_protect<R, I>, Args...>
            {
                typedef _protect<R, I> type;
                typedef std::false_type applied;
            };

            template<typename R, typename ...Args>
            struct make_1_<noinvoke<R>, Args...>
            {
                typedef R type;
                typedef std::false_type applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_1_<noinvoke<R<A...>>, Args...>
            {
                typedef R<typename make_2_<A, Args...>::type...> type;
                typedef
                    std::integral_constant<
                        bool
                      , utility::logical_ops::or_(make_2_<A, Args...>::applied::value...)
                    >
                applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // construct_
            template<typename Type>
            struct construct_
            {
                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    Type{static_cast<Args &&>(args)...}
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _construct
            template<typename Type, typename ...Tfxs>
            struct _construct
              : transform<_construct<Type, Tfxs...>>
            {
                template<
                    typename ...Args
                  , typename X = typename detail::make_1_<Type, Args...>::type
                >
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    as_transform<detail::construct_<X>(Tfxs...)>()(static_cast<Args &&>(args)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _construct_unpack
            template<typename Type, typename ...Tfxs>
            struct _construct_unpack
              : transform<_construct_unpack<Type, Tfxs...>>
            {
                template<
                    typename ...Args
                  , typename X = typename detail::make_1_<Type, Args...>::type
                >
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    as_transform<detail::construct_<X>(Tfxs......)>()(static_cast<Args &&>(args)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename Type, typename ...Tfxs, int I>
        struct as_transform<construct(Type(*)(Tfxs...)), I>
          : detail::_construct<Type, Tfxs...>
        {};

        template<typename Type, typename ...Tfxs, int I>
        struct as_transform<construct(Type(*)(Tfxs......)), I>
          : detail::_construct_unpack<Type, Tfxs...>
        {};
    }
}

#endif
