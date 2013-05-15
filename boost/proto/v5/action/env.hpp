////////////////////////////////////////////////////////////////////////////////////////////////////
// env.hpp
// Helpers for producing and consuming tranform env variables.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_ENV_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_ENV_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace envs
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // key_not_found
                struct key_not_found
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // empty_env
                struct empty_env
                {
                    constexpr key_not_found operator()(utility::any) const noexcept
                    {
                        return key_not_found();
                    }

                    template<typename T, typename V>
                    friend constexpr env<T, V> operator,(empty_env, env<T, V> head)
                    {
                        return head;
                    }
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // env
                // An env is a slot-based storage mechanism, accessible by Key.
                template<typename Key, typename Value, typename Env /*= empty_env*/>
                struct env
                  : private Env
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(env);

                    Value value_;

                    template<typename V, typename B = Env
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<env, V>::value))>
                    constexpr explicit env(V && v, B && b = B())
                      : Env(static_cast<B &&>(b))
                      , value_(v) // copy, not move
                    {}

                    using Env::operator();

                    constexpr auto operator()(Key) const
                    BOOST_PROTO_AUTO_RETURN(
                        Value(this->value_)
                    )

                    template<typename V>
                    auto operator()(Key, V &&v)
                    BOOST_PROTO_AUTO_RETURN(
                        this->value_ = v // copy, don't move
                    )

                    template<typename T, typename V>
                    friend constexpr env<T, V, env> operator,(env tail, env<T, V> head)
                    {
                        return env<T, V, env>(
                            static_cast<env<T, V> &&>(head).value_
                          , static_cast<env &&>(tail)
                        );
                    }
                };

                template<typename Key, typename Value>
                using key_value = env<Key, Value>;

                template<typename Key, typename Value>
                constexpr key_value<Key, Value> make_key_value(Key, Value &&value)
                {
                    return key_value<Key, Value>(static_cast<Value &&>(value));
                }
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
                struct make_env_
                {
                    template<typename T>
                    constexpr T operator()(T && t) const
                    {
                        static_assert(is_env<T>::value, "make_env used in non-env context");
                        return t;
                    }

                    template<typename T, typename U, typename ...V, typename Impl = make_env_>
                    constexpr auto operator()(T && t, U && u, V &&... v) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(Impl())(
                            (static_cast<T &&>(t), static_cast<U &&>(u))
                          , static_cast<V &&>(v)...
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _has_env_
                template<typename Key>
                struct _has_env_
                  : basic_action<_has_env_<Key>>
                {
                    template<typename E>
                    constexpr std::false_type operator()(E &&) const noexcept
                    {
                        return std::false_type();
                    }

                    template<typename E, typename Env, typename ...Rest>
                    constexpr auto operator()(E &&e, Env &&env, Rest &&...rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        std::integral_constant<
                            bool
                          , !::std::is_same<
                                decltype(
                                    static_cast<Env &&>(env)(
                                        call_action_<Key>()(
                                            static_cast<E &&>(e)
                                          , static_cast<Env &&>(env)
                                          , static_cast<Rest &&>(rest)...
                                        )
                                    )
                                )
                              , key_not_found
                            >::value
                        >()
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////////
                // _get_env_
                template<typename Key>
                struct _get_env_
                  : basic_action<_get_env_<Key>>
                {
                    template<typename E>
                    constexpr key_not_found operator()(E &&) const noexcept
                    {
                        return key_not_found();
                    }

                    template<typename E, typename Env, typename ...Rest>
                    constexpr auto operator()(E &&e, Env &&env, Rest &&...rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Env &&>(env)(
                            call_action_<Key>()(
                                static_cast<E &&>(e)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////////
                // _push_env_
                template<typename Key, typename Value>
                struct _push_env_
                  : basic_action<_push_env_<Key, Value>>
                {
                    template<typename E>
                    constexpr auto operator()(E &&e) const
                    BOOST_PROTO_AUTO_RETURN(
                        envs::make_key_value(
                            call_action_<Key>()(static_cast<E &&>(e))
                          , call_action_<Value>()(static_cast<E &&>(e))
                        )
                    )

                    template<typename E, typename Env, typename ...Rest>
                    constexpr auto operator()(E &&e, Env &&env, Rest &&...rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Env &&>(env)
                      , envs::make_key_value(
                            call_action_<Key>()(
                                static_cast<E &&>(e)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )
                          , call_action_<Value>()(
                                static_cast<E &&>(e)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////////
                // _set_env_
                template<typename Key, typename Value>
                struct _set_env_
                  : basic_action<_set_env_<Key, Value>>
                {
                    template<typename E>
                    constexpr auto operator()(E &&e) const
                    BOOST_PROTO_AUTO_RETURN(
                        envs::make_key_value(
                            call_action_<Key>()(static_cast<E &&>(e))
                          , call_action_<Value>()(static_cast<E &&>(e))
                        )
                    )

                    template<typename E, typename Env, typename ...Rest>
                    constexpr auto operator()(E &&e, Env &&env, Rest &&...rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        utility::functional::back()(
                            BOOST_PROTO_TRY_CALL(static_cast<Env &&>(env))(
                                call_action_<Key>()(
                                    static_cast<E &&>(e)
                                  , static_cast<Env &&>(env)
                                  , static_cast<Rest &&>(rest)...
                                )
                              , call_action_<Value>()(
                                    static_cast<E &&>(e)
                                  , static_cast<Env &&>(env)
                                  , static_cast<Rest &&>(rest)...
                                )
                            )
                          , static_cast<Env &&>(env)
                        )
                    )

                    template<typename E, typename Env, typename ...Rest>
                    constexpr auto operator()(E &&e, Env &&env, Rest &&...rest) const volatile
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Env &&>(env)
                      , envs::make_key_value(
                            call_action_<Key>()(
                                static_cast<E &&>(e)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )
                          , call_action_<Value>()(
                                static_cast<E &&>(e)
                              , static_cast<Env &&>(env)
                              , static_cast<Rest &&>(rest)...
                            )
                        )
                    )
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_env
            template<typename ...T>
            constexpr auto make_env(T &&... t)
            BOOST_PROTO_AUTO_RETURN(
                detail::make_env_()(static_cast<T &&>(t)...)
            )

            struct has_env
            {};

            struct get_env
            {};

            struct push_env
            {};

            struct set_env
            {};

            namespace extension
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // has_env
                template<typename Key>
                struct action_impl<has_env(Key)>
                  : detail::_has_env_<Key>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // get_env
                template<typename Key>
                struct action_impl<get_env(Key)>
                  : detail::_get_env_<Key>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // set_env
                template<typename Key, typename Value>
                struct action_impl<set_env(Key, Value)>
                  : detail::_set_env_<Key, Value>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // push_env
                template<typename Key, typename Value>
                struct action_impl<push_env(Key, Value)>
                  : detail::_push_env_<Key, Value>
                {};
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _env
            struct _env
              : basic_action<_env>
            {
                template<typename E>
                constexpr auto operator()(E &&) const
                BOOST_PROTO_AUTO_RETURN(
                    empty_env()
                )

                template<typename E, typename Env, typename ...Rest>
                constexpr auto operator()(E &&, Env && env, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Env &&>(env)
                )
            };
        }
    }
}

#endif
