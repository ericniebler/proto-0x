///////////////////////////////////////////////////////////////////////////////
// grammar.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/action/action.hpp>

#define BOOST_PROTO_BOOL_ACTION(If, Expr)                                                           \
    static_cast<bool>(                                                                              \
        std::remove_reference<                                                                      \
            decltype(proto::action<If>()(std::declval<Expr>()))                                     \
        >::type::value                                                                              \
    )                                                                                               \
    /**/

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_pattern
            template<typename Fun>
            struct expr_pattern
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // grammar_of_
            template<typename T>
            typename T::proto_grammar_type *grammar_of_(int);

            template<typename T>
            T *grammar_of_(long);

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_wrap
            template<typename Expr, typename Grammar>
            struct matches_wrap
              : matches<Expr, Grammar>
            {
                typedef Grammar proto_grammar_type;
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar
        template<typename Grammar>
        struct grammar
          : action<Grammar>
        {
            typedef Grammar proto_grammar_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar_of
        template<typename Grammar>
        struct grammar_of
          : std::remove_pointer<decltype(detail::grammar_of_<Grammar>(1))>
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(Args...)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args...)>
              , Ret(Args...)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(*)(Args...)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args...)>
              , Ret(Args...)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(Args......)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args......)>
              , Ret(Args......)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(*)(Args......)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args......)>
              , Ret(Args......)
            >
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // proto::matches specializations
        template<typename Expr>
        struct matches<Expr, proto::_>
          : std::true_type
        {};

        // Handle proto::or_
        template<typename Expr, typename ...Grammars>
        struct matches<Expr, proto::or_(Grammars...)>
          : utility::or_<detail::matches_wrap<Expr, Grammars>...>
        {};

        // Handle proto::and_
        template<typename Expr, typename ...Grammars>
        struct matches<Expr, proto::and_(Grammars...)>
          : utility::and_<matches<Expr, Grammars>...>
        {};

        // Handle proto::not_
        template<typename Expr, typename Grammar>
        struct matches<Expr, proto::not_(Grammar)>
          : std::integral_constant<bool, !matches<Expr, Grammar>::value>
        {};

        // Handle proto::if_
        template<typename Expr, typename If>
        struct matches<Expr, proto::if_(If)>
          : std::integral_constant<bool, BOOST_PROTO_BOOL_ACTION(If, Expr)>
        {};

        template<typename Expr, typename If, typename Then>
        struct matches<Expr, proto::if_(If, Then)>
          : std::conditional<
                BOOST_PROTO_BOOL_ACTION(If, Expr)
              , matches<Expr, Then>
              , std::false_type
            >::type
        {};

        template<typename Expr, typename If, typename Then, typename Else>
        struct matches<Expr, proto::if_(If, Then, Else)>
          : std::conditional<
                BOOST_PROTO_BOOL_ACTION(If, Expr)
              , matches<Expr, Then>
              , matches<Expr, Else>
            >::type
        {};

        // Handle proto::switch_
        template<typename Expr, typename Cases>
        struct matches<Expr, proto::switch_(Cases)>
          : matches<
                Expr                
              , typename grammar_of<
                    typename Cases::template case_<typename tag_of<Expr>::type>
                >::type
            >
        {};

        template<typename Expr, typename Cases, typename Action>
        struct matches<Expr, proto::switch_(Cases, Action)>
          : matches<
                Expr
              , typename grammar_of<
                    typename Cases::template case_<
                        decltype(action<Action>()(std::declval<Expr>()))
                    >
                >::type
            >
        {};

        template<typename Expr, typename ...Rules>
        struct matches<Expr, proto::algorithms::match(Rules...)>
          : utility::or_<detail::matches_wrap<Expr, Rules>...>
        {};

        template<typename Expr, typename Grammar, typename ...Actions>
        struct matches<Expr, proto::case_(Grammar, Actions...)>
          : matches<Expr, Grammar>
        {};
    }
}

#undef BOOST_PROTO_BOOL_ACTION

#endif
