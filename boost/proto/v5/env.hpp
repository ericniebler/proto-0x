////////////////////////////////////////////////////////////////////////////////////////////////////
// env.hpp
// Contains definition of the \c env<> class and the \c make_env() helpers
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ENV_HPP_INCLUDED
#define BOOST_PROTO_V5_ENV_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
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
                constexpr auto make_env(Key, Value &&value)
                BOOST_PROTO_AUTO_RETURN(
                    env<Key, Value>(static_cast<Value &&>(value))
                )

                template<typename Key, typename Value, typename Env>
                constexpr auto make_env(Key, Value &&value, Env &&en)
                BOOST_PROTO_AUTO_RETURN(
                    env<Key, Value, utility::uncvref<Env>>(static_cast<Value &&>(value), static_cast<Env &&>(en))
                )

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
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_env
            template<typename ...T>
            constexpr auto make_env(T &&... t)
            BOOST_PROTO_AUTO_RETURN(
                detail::make_env_()(static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////
            // env_tag
            template<typename Tag>
            struct env_tag
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(env_tag);

                // So that tag objects of type (derived from) env_tag can be used
                // to create basic_action environments like (data=x, myvar=y),
                // where "data" and "myvar" are tags.
                template<
                    typename V
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<env_tag, V>::value))
                >
                constexpr env<Tag, V> operator=(V && v) const
                {
                    return env<Tag, V>(static_cast<V &&>(v));
                }
            };

            /// Tag type for the data parameter in the basic_action environment
            struct data_tag
              : env_tag<data_tag>
            {
                using env_tag<data_tag>::operator=;
            };

            namespace
            {
                ////////////////////////////////////////////////////////////////////////////////
                // data
                constexpr auto const & data = utility::static_const<data_tag>::value;
            }

            BOOST_PROTO_IGNORE_UNUSED(data);
        }
    }
}

#endif
