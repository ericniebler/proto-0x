///////////////////////////////////////////////////////////////////////////////
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
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // build_result_
            template<typename Ret, typename List, typename... Rest>
            struct build_result_;

            template<typename Ret, typename ...Actions, typename... Rest>
            struct build_result_<Ret, utility::list<Actions...>, Rest...>
            {
                typedef Ret type(Actions..., Rest...);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_2_
            template<std::size_t I, typename BasicAction>
            struct expand_pattern_2_
            {
                typedef BasicAction type;
            };

            template<std::size_t I, typename Action>
            struct expand_pattern_2_<I, pack(Action)>
            {
                typedef proto::_child<I> type(Action);
            };

            template<typename Action>
            struct expand_pattern_2_<(std::size_t)-1, pack(Action)>
            {
                typedef proto::_value type(Action);
            };

            template<std::size_t I, typename Ret, typename ...Actions>
            struct expand_pattern_2_<I, Ret(Actions...)>
            {
                typedef Ret type(typename expand_pattern_2_<I, Actions>::type...);
            };

            template<std::size_t I, typename Ret, typename ...Actions>
            struct expand_pattern_2_<I, Ret(*)(Actions...)>
              : expand_pattern_2_<I, Ret(Actions...)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_1_
            template<typename Indices, typename BasicAction>
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

            ////////////////////////////////////////////////////////////////////////////////////////
            // collect_pack_actions_
            template<typename UnpackingPattern, typename Actions = void()>
            struct collect_pack_actions_
            {
                typedef Actions type;
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
                typedef void type(Actions..., action<Action>);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_2_
            template<std::size_t Arity0, std::size_t Arity1 = Arity0>
            struct compute_indices_2_
              : std::conditional<
                    Arity0 == 0
                  , utility::indices<(std::size_t)-1>
                  , utility::make_indices<0, Arity0>
                >
            {
                static_assert(
                    Arity0 == Arity1
                  , "Two pack expressions in unpacking pattern have different arities"
                );
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_1_
            template<typename Actions, typename ...Args>
            struct compute_indices_1_
            {
                static_assert(
                    utility::never<Actions>::value
                  , "No pack expression found in unpacking pattern. Use proto::pack(<basic_action>) "
                    "to designate a basic_action that returns the expression you want to unpack; "
                    "e.g., proto::pack(_) unpacks the current expression."
                );
            };

            template<typename Head, typename...Tail, typename ...Args>
            struct compute_indices_1_<void(Head, Tail...), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef compute_indices_1_<void(Tail...), Args...> tail;
                typedef typename compute_indices_2_<arity::value, tail::arity::value>::type type;
            };

            template<typename Head, typename ...Args>
            struct compute_indices_1_<void(Head), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef typename compute_indices_2_<arity::value>::type type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _unpack
            template<typename Ret, typename ...Actions>
            struct _unpack
              : basic_action<_unpack<Ret, Actions...>>
            {
                typedef
                    typename collect_pack_actions_<
                        typename utility::result_of::back<Actions...>::type
                    >::type
                actions_type;

                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    action<
                        typename expand_pattern_1_<
                            typename compute_indices_1_<actions_type, Args...>::type
                          , Ret(Actions......)
                        >::type
                    >()(static_cast<Args &&>(args)...)
                )
            };
        }

        // Usage: construct(wrap<proto::_value>(proto::_value))
        template<typename Ret, typename ...Actions>
        struct action<Ret(*)(Actions......)>
          : action<Ret(Actions......)>
        {};

        // Handle actions with pack expansions
        template<typename Ret, typename ...Actions>
        struct action<Ret(Actions......)>
          : detail::_unpack<Ret, Actions...>
        {};
    }
}

#endif
