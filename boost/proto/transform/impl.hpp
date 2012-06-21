///////////////////////////////////////////////////////////////////////////////
// impl.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_IMPL_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_IMPL_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // evironment_base
        struct environment_base
        {
            void operator[](struct not_a_valid_tag) const noexcept;

            template<typename T>
            T && at(utility::any, T && t) const noexcept
            {
                return static_cast<T &&>(t);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // evironment
        // A transform environment is a slot-based storage mechanism, accessible by tag.
        template<typename Tag, typename Value, typename Base /*= environment_base*/>
        struct environment
          : Base
        {
            BOOST_PROTO_REGULAR_TRIVIAL_CLASS(environment);

            Value value_;

            template<typename V, typename B = Base
              , BOOST_PROTO_ENABLE_IF(!BOOST_PROTO_IS_CONVERTIBLE(V, environment))>
            explicit environment(V && v, B && b = B())
              : Base(static_cast<B &&>(b))
              , value_(static_cast<V &&>(v))
            {}

            // For key-based lookups not intended to fail
            using Base::operator[];
            auto operator[](Tag) const
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<environment const &>(*this).value_)
            )

            // For key-based lookups that can fail, use the default if key not found.
            using Base::at;
            template<typename T>
            auto at(Tag, T &&) const
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<environment const &>(*this).value_)
            )

            template<typename T, typename V>
            friend environment<T, V, environment> operator,(environment tail, environment<T, V> head)
            {
                return environment<T, V, environment>{
                    static_cast<environment<T, V> &&>(head).value_
                  , static_cast<environment &&>(tail)
                };
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // callable
        struct callable
        {};

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // is_transform
            std::true_type is_transform_(transform_base const &);
            std::false_type is_transform_(utility::any const &);

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_transform
            std::true_type is_callable_(callable const &);
            std::false_type is_callable_(utility::any const &);

            template<typename T>
            struct is_callable_2_
              : decltype(detail::is_callable_(std::declval<T>()))
            {};

            template<typename T>
            struct is_callable_1_
              : is_callable_2_<T>
            {};

            template<template<typename...> class T, typename... Args>
            struct is_callable_1_<T<Args...>>
              : std::is_same<typename utility::back_type<Args...>::type, callable>
            {};
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_callable
        template<typename T>
        struct is_callable
          : detail::is_callable_1_<T>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform_base
        struct transform_base
          : callable
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
          : decltype(detail::is_transform_(std::declval<T>()))
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename T>
        struct as_transform
          : T::proto_transform_type
        {};

        template<typename Ret, typename ...Args>
        struct as_transform<Ret(Args...)>
          : transform<as_transform<Ret(Args...)>>
        {
            template<typename ...T, typename R = Ret>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                typename std::conditional<
                    is_callable<R>::value
                  , call<R(Args...)>
                  , make<R(Args...)>
                >::type()(static_cast<T &&>(t)...)
            )
        };

        template<typename Ret, typename ...Args>
        struct as_transform<Ret(*)(Args...)>
          : as_transform<Ret(Args...)>
        {};
    }
}

#endif
