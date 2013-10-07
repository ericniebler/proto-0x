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
#include <boost/proto/v5/env.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
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
