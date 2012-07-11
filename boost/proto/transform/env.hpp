///////////////////////////////////////////////////////////////////////////////
// env.hpp
// Helpers for producing and consuming tranform environment variables.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_ENV_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_ENV_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>

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
            T at(utility::any, T && t) const noexcept(noexcept(T(static_cast<T &&>(t))))
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
              , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<environment, V>::value))>
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

        namespace detail
        {
            struct make_env_impl_
            {
                template<typename T>
                static T make_env(T && t)
                {
                    static_assert(
                        utility::is_base_of<environment_base, T>::value
                      , "make_env used in non-environment context"
                    );
                    return t;
                }

                template<typename T, typename U, typename ...V, typename Impl = make_env_impl_>
                static auto make_env(T && t, U && u, V &&... v)
                BOOST_PROTO_AUTO_RETURN(
                    Impl::make_env((static_cast<T &&>(t), static_cast<U &&>(u)), static_cast<V &&>(v)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_env
        template<typename ...T>
        inline auto make_env(T &&... t)
        BOOST_PROTO_AUTO_RETURN(
            detail::make_env_impl_::make_env(static_cast<T &&>(t)...)
        )

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _env
        template<typename Tag>
        struct _env
          : transform<_env<Tag>>
        {
            template<typename E, typename S, typename Env, typename ...Rest>
            auto operator()(E &&, S &&, Env && env, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Env &&>(env)[Tag()]
            )
        };

        template<>
        struct _env<void>
          : transform<_env<void>>
        {
            template<typename E, typename S, typename Env, typename ...Rest>
            auto operator()(E &&, S &&, Env && env, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Env &&>(env)
            )
        };
    }
}

#endif
