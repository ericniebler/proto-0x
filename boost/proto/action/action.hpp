////////////////////////////////////////////////////////////////////////////////////////////////////
// action.hpp
// Helpers for building Proto actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_ACTION_HPP_INCLUDED
#define BOOST_PROTO_ACTION_ACTION_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename BoolAction, typename ...Args>
            struct eval_bool_action_
              : std::integral_constant<
                    bool
                  , static_cast<bool>(
                        std::remove_reference<
                            decltype(proto::action<BoolAction>()(std::declval<Args>()...))
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
        }

        namespace extension
        {
            template<typename Action>
            struct action_impl
            {
                static_assert(
                    utility::never<Action>::value
                  , "Unknown action pattern used in proto::action"
                );
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action_base
        struct action_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // basic_action
        // CRTP base class for all actions
        template<typename Action>
        struct basic_action
          : action_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename ActionOrActiveGrammar>
        struct action
          : ActionOrActiveGrammar
        {};

        template<typename Ret, typename ...Args>
        struct action<Ret(Args...)>
          : action_base
          , grammar<Ret(Args...)>
        {
            template<typename ...Args2, typename A = typename detail::normalize_action_<Ret(Args...)>::type>
            auto operator()(Args2 &&... args2) const
            BOOST_PROTO_AUTO_RETURN(
                BOOST_PROTO_TRY_CALL(extension::action_impl<A>())(static_cast<Args2 &&>(args2)...)
            )
        };

        template<typename Ret, typename ...Args>
        struct action<Ret(Args......)>
          : action_base
          , grammar<Ret(Args......)>
        {
            template<typename ...Args2, typename A = typename detail::normalize_action_<Ret(Args......)>::type>
            auto operator()(Args2 &&... args2) const
            BOOST_PROTO_AUTO_RETURN(
                BOOST_PROTO_TRY_CALL(extension::action_impl<A>())(static_cast<Args2 &&>(args2)...)
            )
        };

        template<typename Ret, typename ...Args>
        struct action<Ret(*)(Args...)>
          : action<Ret(Args...)>
        {};

        template<typename Ret, typename ...Args>
        struct action<Ret(*)(Args......)>
          : action<Ret(Args......)>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_action
        template<typename T>
        struct is_action
          : std::is_base_of<action_base, T>
        {};

        template<typename T>
        struct is_action<T &>
          : std::is_base_of<action_base, T>
        {};

        template<typename T>
        struct is_action<T &&>
          : std::is_base_of<action_base, T>
        {};
    }
}

#endif
