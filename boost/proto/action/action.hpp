///////////////////////////////////////////////////////////////////////////////
// action.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ALGORITHM_ALGORITHM_HPP_INCLUDED
#define BOOST_PROTO_ALGORITHM_ALGORITHM_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/action/integral_constant.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/construct.hpp>

namespace boost
{
    namespace proto
    {
        namespace algorithms
        {
            struct block {};
            struct matches {};
            struct match {};
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename T, int>
        struct action
          : T::proto_algorithm_type
        {};

        template<typename T, int I>
        struct action<action<T>, I>
          : action<T>
        {};

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // _matches basic_action
            template<typename Gram>
            struct _matches
              : proto::basic_action<_matches<Gram>>
            {
                template<typename Expr, typename... Rest>
                auto operator()(Expr &&, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::matches<Expr, Gram>()
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // action
        template<typename ...Actions, int I>
        struct action<algorithms::block(Actions...), I>
          : action<and_(Actions...)>
        {};

        template<typename Expr, int I>
        struct action<algorithms::matches(Expr), I>
          : detail::_matches<Expr>
        {};

        template<typename Huh, typename ...Oops, int I>
        struct action<algorithms::matches(Huh, Oops...), I>
        {
            static_assert(
                utility::never<Huh, Oops...>::value
              , "algorithms::matches usage: match(<expression-pattern>). "
                "(Did you mean match(case_(<...>), <etc>)?)"
            );
        };

        template<typename ...Cases, int I>
        struct action<algorithms::match(Cases...), I>
          : proto::action<proto::or_(Cases...)>
        {};

        // The tags get a pass-through basic_action
    #define BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(Tag)                                           \
        template<typename Action, int I>                                                               \
        struct action<tag::Tag(Action), I>                                                             \
          : proto::action<proto::pass_through(Action)>                                                 \
        {}                                                                                          \
        /**/

    #define BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(Tag)                                          \
        template<typename Tfx0, typename Tfx1, int I>                                               \
        struct action<tag::Tag(Tfx0, Tfx1), I>                                                      \
          : proto::action<proto::pass_through(Tfx0, Tfx1)>                                          \
        {}                                                                                          \
        /**/

        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(terminal);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(unary_plus);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(negate);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(dereference);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(complement);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(address_of);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(logical_not);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(pre_inc);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(pre_dec);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(post_inc);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_UNARY(post_dec);

        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(shift_left);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(shift_right);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(multiplies);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(divides);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(modulus);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(plus);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(minus);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(less);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(greater);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(less_equal);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(greater_equal);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(equal_to);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(not_equal_to);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(logical_or);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(logical_and);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_and);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_or);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_xor);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(comma);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(mem_ptr);

        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(shift_left_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(shift_right_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(multiplies_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(divides_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(modulus_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(plus_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(minus_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_and_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_or_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(bitwise_xor_assign);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(subscript);
        BOOST_PROTO_PASS_THROUGH_ALGORITHM_BINARY(member);

        template<typename Tfx0, typename Tfx1, typename Tfx2, int I>
        struct action<tag::if_else_(Tfx0, Tfx1, Tfx2), I>
          : proto::action<proto::pass_through(Tfx0, Tfx1, Tfx2)>
        {};

        template<typename ...Actions, int I>
        struct action<tag::function(Actions...), I>
          : proto::action<proto::pass_through(Actions...)>
        {};

        template<typename Tag, typename Value, int I>
        struct action<tag::nullary_expr(Tag, Value), I>
          : proto::action<proto::pass_through(Value)>
        {};

        template<typename Tag, typename Action, int I>
        struct action<tag::unary_expr(Tag, Action), I>
          : proto::action<proto::pass_through(Action)>
        {};

        template<typename Tag, typename Tfx0, typename Tfx1, int I>
        struct action<tag::binary_expr(Tag, Tfx0, Tfx1), I>
          : proto::action<proto::pass_through(Tfx0, Tfx1)>
        {};

        template<typename Tag, typename ...Actions, int I>
        struct action<tag::nary_expr(Tag, Actions...), I>
          : proto::action<proto::pass_through(Actions...)>
        {};
    }
}

#endif
