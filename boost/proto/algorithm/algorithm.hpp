///////////////////////////////////////////////////////////////////////////////
// algorithm.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ALGORITHM_AS_ALGORITHM_HPP_INCLUDED
#define BOOST_PROTO_ALGORITHM_AS_ALGORITHM_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/transform/integral_constant.hpp>
#include <boost/proto/transform/base.hpp>
#include <boost/proto/transform/construct.hpp>

namespace boost
{
    namespace proto
    {
        namespace algorithms
        {
            struct if_ {};
            //struct else_if {};
            struct then_ {};
            struct else_ {};
            struct or_ {};
            struct and_ {};
            struct matches {};
            struct match {};
            struct case_ {};
            struct default_ {};

            using namespace proto::tags::tag;

            struct nullary_expr {};
            struct unary_expr {};
            struct binary_expr {};
            struct nary_expr {};
        }

        template<typename ...Algos>
        struct algorithm;

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // as_grammar_
            template<typename Expr>
            struct as_grammar_
            {
                typedef Expr type;
            };

            template<typename Tag, typename... Grams>
            struct as_grammar_<Tag(*)(Grams...)>
            {
                typedef proto::expr<Tag, args<typename as_grammar_<Grams>::type...>> type;
            };

            template<typename Value>
            struct as_grammar_<algorithms::terminal(*)(Value)>
            {
                typedef proto::terminal<Value> type;
            };

            template<typename Tag, typename Value>
            struct as_grammar_<algorithms::nullary_expr(*)(Tag, Value)>
            {
                typedef proto::nullary_expr<Tag, Value> type;
            };

            template<typename Tag, typename Gram0>
            struct as_grammar_<algorithms::unary_expr(*)(Tag, Gram0)>
            {
                typedef proto::unary_expr<Tag, typename as_grammar_<Gram0>::type> type;
            };

            template<typename Tag, typename Gram0, typename Gram1>
            struct as_grammar_<algorithms::binary_expr(*)(Tag, Gram0, Gram1)>
            {
                typedef
                    proto::binary_expr<
                        Tag
                      , typename as_grammar_<Gram0>::type
                      , typename as_grammar_<Gram1>::type
                    >
                type;
            };

            template<typename Tag, typename ...Grams>
            struct as_grammar_<algorithms::nary_expr(*)(Tag, Grams...)>
            {
                typedef proto::nary_expr<Tag, typename as_grammar_<Grams>::type...> type;
            };

            // TODO expensive. Maybe proto::matches should just work with function-type-based
            // expression patterns natively, and eliminate this as_grammar_ B.S.
            template<typename Tag, typename... Grams>
            struct as_grammar_<Tag(*)(Grams......)>
              : as_grammar_<
                    typename utility::concat<
                        typename utility::pop_back<Tag(Grams...)>::type
                      , void(
                            vararg<
                                typename as_grammar_<
                                    typename utility::result_of::back<Grams...>::type
                                >::type
                            >
                        )
                    >::type *
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // as_rule_
            template<typename T>
            struct as_rule_
            {
                static_assert(
                    utility::never<T>::value
                  , "In algorithms::match(), expecting case_(<pattern>, <statements...>)"
                );
            };

            template<typename Expr, typename ...Grams>
            struct as_rule_<algorithms::case_(*)(Expr, Grams...)>
            {
                typedef proto::when<typename as_grammar_<Expr>::type, algorithm<Grams...>> type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _matches transform
            template<typename Gram>
            struct _matches
              : proto::transform<_matches<Gram>>
            {
                template<typename Expr, typename... Rest>
                auto operator()(Expr &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    typename proto::matches<Expr, Gram>::type()
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // algorithm (needed?)
        template<typename ...Algo>
        struct algorithm
          : proto::and_<Algo...>
        {};

        template<typename Algo>
        struct algorithm<Algo>
          : proto::as_transform<Algo>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename Cond, typename ...Then, typename ...Else, int I>
        struct as_transform<
            algorithms::if_(Cond, algorithms::then_(*)(Then...), algorithms::else_(*)(Else...)), I
        >
          : proto::if_<
                as_transform<Cond>
              , algorithm<Then...>
              , algorithm<Else...>
            >
        {};

        // Give a reasonable error for incorrect usage of algorithms::if_
        template<typename ...Oops, int I>
        struct as_transform<algorithms::if_(Oops...), I>
        {
            static_assert(
                utility::never<Oops...>::value
              , "algorithms::if_ usage: if_(<condition>, then_(<statements...>), else_(<statements...>))"
            );
        };

        template<typename Expr, int I>
        struct as_transform<algorithms::matches(Expr), I>
          : detail::_matches<typename detail::as_grammar_<Expr>::type>
        {};

        template<typename Huh, typename ...Oops, int I>
        struct as_transform<algorithms::matches(Huh, Oops...), I>
        {
            static_assert(
                utility::never<Huh, Oops...>::value
              , "algorithms::matches usage: match(<expression-pattern>). "
                "(Did you mean match(case_(<...>), <etc>)?)"
            );
        };

        template<typename ...Cases, int I>
        struct as_transform<algorithms::match(Cases...), I>
          : proto::or_<
                typename detail::as_rule_<Cases>::type...
            >
        {};
    }
}

#endif
