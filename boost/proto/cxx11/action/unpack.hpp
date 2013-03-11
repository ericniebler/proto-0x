////////////////////////////////////////////////////////////////////////////////////////////////////
// unpack.hpp
// Make action work with unpacking patterns.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_UNPACK_HPP_INCLUDED
#define BOOST_PROTO_ACTION_UNPACK_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>
#include <boost/proto/cxx11/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // expand_pattern_2_
                template<std::size_t I, typename BasicAction>
                struct expand_pattern_2_
                {
                    using type = BasicAction;
                };

                template<std::size_t I, typename Action>
                struct expand_pattern_2_<I, pack(Action)>
                {
                    using type = proto::cxx11::_child<I>(Action);
                };

                template<typename Action>
                struct expand_pattern_2_<(std::size_t)-1, pack(Action)>
                {
                    using type = proto::cxx11::_value(Action);
                };

                template<std::size_t I, typename Ret, typename ...Actions>
                struct expand_pattern_2_<I, Ret(Actions...)>
                {
                    using type = Ret(typename expand_pattern_2_<I, Actions>::type...);
                };

                template<std::size_t I, typename Ret, typename ...Actions>
                struct expand_pattern_2_<I, Ret(*)(Actions...)>
                  : expand_pattern_2_<I, Ret(Actions...)>
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // expand_pattern_1_
                template<typename Indices, typename Actions>
                struct expand_pattern_1_;

                template<std::size_t ...I, typename Ret, typename ...Actions>
                struct expand_pattern_1_<utility::indices<I...>, Ret(Actions......)>
                  : utility::concat<
                        typename utility::pop_back<Ret(Actions...)>::type
                      , void(
                            typename expand_pattern_2_<
                                I
                              , typename utility::result_of::back<Actions...>::type
                            >::type...
                        )
                    >
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // collect_pack_actions_
                template<typename UnpackingPattern, typename Actions = void()>
                struct collect_pack_actions_
                {
                    using type = Actions;
                };

                // Note: Do *NOT* recurse into vararg functions. Those are nested pack actions,
                // which should not be considered together with this one.
                template<typename Ret, typename Head, typename ...Tail, typename Actions>
                struct collect_pack_actions_<Ret(Head, Tail...), Actions>
                  : collect_pack_actions_<
                        Ret(Tail...)
                      , typename collect_pack_actions_<Head, Actions>::type
                    >
                {};

                template<typename Ret, typename Head, typename ...Tail, typename Actions>
                struct collect_pack_actions_<Ret(*)(Head, Tail...), Actions>
                  : collect_pack_actions_<Ret(Head, Tail...), Actions>
                {};

                template<typename Action, typename ...Actions>
                struct collect_pack_actions_<pack(Action), void(Actions...)>
                {
                    using type = void(Actions..., as_action_<Action>);
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // compute_indices_2_
                template<std::size_t Arity0, std::size_t Arity1 = Arity0>
                struct compute_indices_2_
                  : std::conditional<
                        Arity0 == 0
                      , utility::indices<(std::size_t)-1>
                      , utility::make_indices<Arity0>
                    >
                {
                    static_assert(
                        Arity0 == Arity1
                      , "Two pack expressions in unpacking pattern have different arities"
                    );
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // compute_indices_1_
                template<typename Actions, typename ...Args>
                struct compute_indices_1_
                {
                    static_assert(
                        utility::never<Actions>::value
                      , "No pack expression found in unpacking pattern. Use proto::pack(<basic_action>) "
                        "to designate an action that returns the expression you want to unpack; "
                        "e.g., proto::pack(_) unpacks the current expression."
                    );
                };

                template<typename Head, typename...Tail, typename ...Args>
                struct compute_indices_1_<void(Head, Tail...), Args...>
                {
                    using arity = arity_of<decltype(Head()(std::declval<Args>()...))>;
                    using tail = compute_indices_1_<void(Tail...), Args...>;
                    using type = typename compute_indices_2_<arity::value, tail::arity::value>::type;
                };

                template<typename Head, typename ...Args>
                struct compute_indices_1_<void(Head), Args...>
                {
                    using arity = arity_of<decltype(Head()(std::declval<Args>()...))>;
                    using type = typename compute_indices_2_<arity::value>::type;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _unpack
                template<typename Ret, typename ...Actions>
                struct _unpack
                  : basic_action<_unpack<Ret, Actions...>>
                  // TODO: move implementation into _unpack_impl that is parameterized on result of
                  // collect_pack_actions_, and specialized on void(as_action_<_>) to have a more
                  // optimal implementation.
                {
                    using actions_type =
                        typename collect_pack_actions_<
                            typename utility::result_of::back<Actions...>::type
                        >::type;

                    template<typename ...Args>
                    auto operator()(Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        as_action_<
                            typename expand_pattern_1_<
                                typename compute_indices_1_<actions_type, Args...>::type
                              , Ret(Actions......)
                            >::type
                        >()(static_cast<Args &&>(args)...)
                    )
                };
            }

            namespace extension
            {
                // Handle actions with pack expansions
                template<typename Ret, typename ...Actions>
                struct action_impl<Ret(Actions......)>
                  : detail::_unpack<Ret, Actions...>
                {};
            }
        }
    }
}

#endif
