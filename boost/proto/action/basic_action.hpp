////////////////////////////////////////////////////////////////////////////////////////////////////
// action.hpp
// Helpers for building Proto actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_BASIC_ACTION_HPP_INCLUDED
#define BOOST_PROTO_ACTION_BASIC_ACTION_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            struct not_an_action
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            template<typename BoolAction, typename ...Args>
            struct eval_bool_action_
              : std::integral_constant<
                    bool
                  , static_cast<bool>(
                        std::remove_reference<
                            decltype(as_action_<BoolAction>()(std::declval<Args>()...))
                        >::type::value
                    )
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // normalize_action_
            template<typename T, typename Enable = void>
            struct normalize_action_
            {
                using type = T;
            };

            template<typename Ret, typename ...Args>
            struct normalize_action_<Ret(Args...), utility::always_void<typename Ret::proto_action_type>>
            {
                using type = typename Ret::proto_action_type(Args...);
            };

            template<typename Ret, typename ...Args>
            struct normalize_action_<Ret(Args......), utility::always_void<typename Ret::proto_action_type>>
            {
                using type = typename Ret::proto_action_type(Args......);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // as_action_impl_
            template<typename T>
            struct as_action_impl_
            {
                using type = T;
            };

            template<typename Ret, typename ...Args>
            struct as_action_impl_<Ret(Args...)>
            {
                using type =
                    extension::action_impl<typename normalize_action_<Ret(Args...)>::type>;
            };

            template<typename Ret, typename ...Args>
            struct as_action_impl_<Ret(Args......)>
            {
                using type =
                    extension::action_impl<typename normalize_action_<Ret(Args......)>::type>;
            };

            template<typename Ret, typename ...Args>
            struct as_action_impl_<Ret(*)(Args...)>
              : as_action_impl_<Ret(Args...)>
            {};

            template<typename Ret, typename ...Args>
            struct as_action_impl_<Ret(*)(Args......)>
              : as_action_impl_<Ret(Args......)>
            {};
        }

        namespace extension
        {
            template<typename T>
            struct not_an_action
              : detail::not_an_action
            {
                template<typename ...Args>
                utility::any operator()(Args &&...) const noexcept
                {
                    static_assert(
                        utility::never<T>::value
                      , "Unknown action pattern used in proto::action"
                    );
                    return utility::any();
                }
            };

            template<typename Action, typename Enable>
            struct action_impl
              : not_an_action<Action>
            {};
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action_base
        struct action_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // basic_action
        // CRTP base class for all primitive actions
        template<typename Action>
        struct basic_action
          : action_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_action
        template<typename T>
        struct is_action
          : std::integral_constant<
                bool
              , std::is_base_of<action_base, T>::value ||
                (
                    std::is_base_of<detail::def_base, T>::value &&
                   !std::is_base_of<detail::not_an_action, T>::value
                )
            >
        {};

        template<typename T>
        struct is_action<T &>
          : is_action<T>
        {};

        template<typename T>
        struct is_action<T &&>
          : is_action<T>
        {};

        template<typename Ret, typename ...Args>
        struct is_action<Ret(Args...)>
          : std::integral_constant<
                bool
              , !std::is_base_of<detail::not_an_action, detail::as_action_<Ret(Args...)>>::value
            >
        {};

        template<typename Ret, typename ...Args>
        struct is_action<Ret(Args......)>
          : std::integral_constant<
                bool
              , !std::is_base_of<detail::not_an_action, detail::as_action_<Ret(Args......)>>::value
            >
        {};

        template<typename Ret, typename ...Args>
        struct is_action<Ret(*)(Args...)>
          : is_action<Ret(Args...)>
        {};

        template<typename Ret, typename ...Args>
        struct is_action<Ret(*)(Args......)>
          : is_action<Ret(Args......)>
        {};
    }
}

#endif
