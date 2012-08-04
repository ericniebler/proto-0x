///////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED
#define BOOST_PROTO_ACTION_EXPR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/pass_through.hpp>

namespace boost
{
    namespace proto
    {
        namespace algorithms
        {
            struct matches {};
        }

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
        // TODO rename matches_ and put somewhere else
        template<typename Expr>
        struct action<algorithms::matches(Expr)>
          : detail::_matches<Expr>
        {};

        // BUGBUG These exhaustive specializations are not general; they do not handle
        // any user-defined tags whould should also be handled similarly.

        // The tags get a pass-through basic_action
    #define BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(Tag)                                              \
        template<typename Action>                                                                   \
        struct action<tag::Tag(Action)>                                                             \
          : proto::action<proto::pass_through(Action)>                                              \
        {}                                                                                          \
        /**/

    #define BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(Tag)                                             \
        template<typename Tfx0, typename Tfx1>                                                      \
        struct action<tag::Tag(Tfx0, Tfx1)>                                                         \
          : proto::action<proto::pass_through(Tfx0, Tfx1)>                                          \
        {}                                                                                          \
        /**/

        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(terminal);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(unary_plus);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(negate);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(dereference);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(complement);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(address_of);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(logical_not);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(pre_inc);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(pre_dec);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(post_inc);
        BOOST_PROTO_PASS_THROUGH_ACTION_UNARY(post_dec);

        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(shift_left);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(shift_right);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(multiplies);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(divides);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(modulus);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(plus);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(minus);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(less);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(greater);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(less_equal);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(greater_equal);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(equal_to);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(not_equal_to);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(logical_or);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(logical_and);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_and);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_or);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_xor);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(comma);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(mem_ptr);

        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(shift_left_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(shift_right_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(multiplies_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(divides_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(modulus_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(plus_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(minus_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_and_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_or_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(bitwise_xor_assign);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(subscript);
        BOOST_PROTO_PASS_THROUGH_ACTION_BINARY(member);

        template<typename Tfx0, typename Tfx1, typename Tfx2>
        struct action<tag::if_else_(Tfx0, Tfx1, Tfx2)>
          : proto::action<proto::pass_through(Tfx0, Tfx1, Tfx2)>
        {};

        template<typename ...Actions>
        struct action<tag::function(Actions...)>
          : proto::action<proto::pass_through(Actions...)>
        {};

        template<typename Tag, typename Value>
        struct action<tag::nullary_expr(Tag, Value)>
          : proto::action<proto::pass_through(Value)>
        {};

        template<typename Tag, typename Action>
        struct action<tag::unary_expr(Tag, Action)>
          : proto::action<proto::pass_through(Action)>
        {};

        template<typename Tag, typename Tfx0, typename Tfx1>
        struct action<tag::binary_expr(Tag, Tfx0, Tfx1)>
          : proto::action<proto::pass_through(Tfx0, Tfx1)>
        {};

        template<typename Tag, typename ...Actions>
        struct action<tag::nary_expr(Tag, Actions...)>
          : proto::action<proto::pass_through(Actions...)>
        {};
    }
}

#endif
