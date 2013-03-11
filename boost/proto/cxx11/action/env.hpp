////////////////////////////////////////////////////////////////////////////////////////////////////
// env.hpp
// Helpers for producing and consuming tranform env variables.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_ENV_HPP_INCLUDED
#define BOOST_PROTO_ACTION_ENV_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>
#include <boost/proto/cxx11/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace envs
            {
                struct key_not_found
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // empty_env
                struct empty_env
                {
                    key_not_found operator[](utility::any) const noexcept
                    {
                        return key_not_found();
                    }

                    template<typename T>
                    T at(utility::any, T && t) const noexcept(noexcept(T(static_cast<T &&>(t))))
                    {
                        return static_cast<T &&>(t);
                    }
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // env
                // A basic_action env is a slot-based storage mechanism, accessible by Key.
                template<typename Key, typename Value, typename Env /*= empty_env*/>
                struct env
                  : private Env
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(env);

                    Value value_;

                    template<typename V, typename B = Env
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<env, V>::value))>
                    explicit env(V && v, B && b = B())
                      : Env(static_cast<B &&>(b))
                      , value_(static_cast<V &&>(v))
                    {}

                    // For key-based lookups not intended to fail
                    using Env::operator[];
                    Value operator[](Key) const
                    BOOST_PROTO_RETURN(
                        this->value_
                    )

                    // For key-based lookups that can fail, use the default if key not found.
                    using Env::at;
                    template<typename T>
                    Value at(Key, T &&) const
                    BOOST_PROTO_RETURN(
                        this->value_
                    )

                    template<typename T, typename V>
                    friend env<T, V, env> operator,(env tail, env<T, V> head)
                    {
                        return env<T, V, env>(
                            static_cast<env<T, V> &&>(head).value_
                          , static_cast<env &&>(tail)
                        );
                    }
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_env
            template<typename T>
            struct is_env
              : std::is_base_of<empty_env, T>
            {};

            template<typename T>
            struct is_env<T &>
              : std::is_base_of<empty_env, T>
            {};

            template<typename T>
            struct is_env<T &&>
              : std::is_base_of<empty_env, T>
            {};

            namespace detail
            {
                struct make_env_impl_
                {
                    template<typename T>
                    static T make_env(T && t)
                    {
                        static_assert(is_env<T>::value, "make_env used in non-env context");
                        return t;
                    }

                    template<typename T, typename U, typename ...V, typename Impl = make_env_impl_>
                    static auto make_env(T && t, U && u, V &&... v)
                    BOOST_PROTO_AUTO_RETURN(
                        Impl::make_env((static_cast<T &&>(t), static_cast<U &&>(u)), static_cast<V &&>(v)...)
                    )
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_env
            template<typename ...T>
            inline auto make_env(T &&... t)
            BOOST_PROTO_AUTO_RETURN(
                detail::make_env_impl_::make_env(static_cast<T &&>(t)...)
            )

            namespace functional
            {
                template<typename Key>
                struct env_var
                {
                    template<typename Env>
                    auto operator()(Env &&env) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Env &&>(env)[Key()]
                    )
                };
            }

            namespace result_of
            {
                template<typename Env, typename Key>
                struct env_var
                {
                    using type = decltype(functional::env_var<Key>()(std::declval<Env>()));
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _env_var
            template<typename Key>
            struct _env_var
              : basic_action<_env_var<Key>>
            {
                template<typename E, typename Env, typename ...Rest>
                auto operator()(E &&, Env && env, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Env &&>(env)[Key()]
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _env
            struct _env
              : basic_action<_env>
            {
                template<typename E, typename Env, typename ...Rest>
                auto operator()(E &&, Env && env, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Env &&>(env)
                )
            };
        }
    }
}

#endif
