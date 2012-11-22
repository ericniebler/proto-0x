////////////////////////////////////////////////////////////////////////////////////////////////////
// eval.hpp
// Evaluate a proto expression according to the rules of C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_EVAL_HPP_INCLUDED
#define BOOST_PROTO_ACTION_EVAL_HPP_INCLUDED

//#include <memory> for std::addressof
#include <utility>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/get_pointer.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/children.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/active_grammar.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/action/when.hpp>
#include <boost/proto/grammar/when.hpp>
#include <boost/proto/grammar/not.hpp>
#include <boost/proto/grammar/placeholders.hpp>

namespace boost
{
    namespace proto
    {
        template<typename Tag>
        struct op
        {
            template<typename ...T>
            utility::any operator()(T &&...) const noexcept
            {
                static_assert(
                    utility::never<Tag, T...>::value
                  , "proto::eval doesn't know how to evaluate this expression!"
                );
                return utility::any();
            }
        };

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // _eval_unknown
            struct _eval_unknown
              : basic_action<_eval_unknown>
            {
                template<typename Expr, typename ...Rest>
                utility::any operator()(Expr && expr, Rest &&...) const noexcept
                {
                    static_assert(
                        utility::never<Expr>::value
                      , "proto::_eval doesn't know how to evaluate this expression!"
                    );
                    return utility::any();
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _eval_case
            template<typename ActiveGrammar, typename Tag>
            struct _eval_case
              : active_grammar<when(not_(_), _eval_unknown)>
            {};

            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, terminal>
              : active_grammar<when(terminal(_), _value)>
            {};
        }

        template<>
        struct op<terminal>
          : utility::identity
        {};

        namespace detail
        {
            template<typename Tag, typename Action>
            struct _op_unpack
              : basic_action<_op_unpack<Tag, Action>>
            {
                template<std::size_t ...I, typename Expr, typename ...Rest>
                auto impl(utility::indices<I...>, Expr && expr, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    BOOST_PROTO_TRY_CALL(op<Tag>())(
                        action<Action>()(
                            proto::child<I>(static_cast<Expr &&>(expr))
                          , static_cast<Rest &&>(rest)...
                        )...
                    )
                )

                template<typename Expr, typename ...Rest>
                auto operator()(Expr && expr, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    this->impl(
                        utility::make_indices<arity_of<Expr>::value>()
                      , static_cast<Expr &&>(expr)
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            template<typename ActiveGrammar>
            struct _op_unpack<terminal, ActiveGrammar>
              : _value
            {};

            // Must respect short-circuit evaluation
            template<typename ActiveGrammar>
            struct _op_unpack<logical_or, ActiveGrammar>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && expr, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<ActiveGrammar>()(
                        proto::child<0>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                 || action<ActiveGrammar>()(
                        proto::child<1>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            // Must respect short-circuit evaluation
            template<typename ActiveGrammar>
            struct _op_unpack<logical_and, ActiveGrammar>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && expr, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<ActiveGrammar>()(
                        proto::child<0>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                 && action<ActiveGrammar>()(
                        proto::child<1>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            // Must respect short-circuit evaluation
            template<typename ActiveGrammar>
            struct _op_unpack<if_else_, ActiveGrammar>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && expr, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<ActiveGrammar>()(
                        proto::child<0>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                  ? action<ActiveGrammar>()(
                        proto::child<1>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                  : action<ActiveGrammar>()(
                        proto::child<2>(static_cast<Expr &&>(expr))
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };
        }

    #define BOOST_PROTO_UNARY_EVAL(TAG)                                                             \
        template<>                                                                                  \
        struct op<TAG>                                                                              \
          : TAG                                                                                     \
        {};                                                                                         \
                                                                                                    \
        namespace detail                                                                            \
        {                                                                                           \
            template<typename ActiveGrammar>                                                        \
            struct _eval_case<ActiveGrammar, TAG>                                                   \
              : active_grammar<when(TAG(ActiveGrammar), _op_unpack<TAG, ActiveGrammar>)>            \
            {};                                                                                     \
        }                                                                                           \
        /**/

    #define BOOST_PROTO_BINARY_EVAL(TAG)                                                            \
        template<>                                                                                  \
        struct op<TAG>                                                                              \
          : TAG                                                                                     \
        {};                                                                                         \
                                                                                                    \
        namespace detail                                                                            \
        {                                                                                           \
            template<typename ActiveGrammar>                                                        \
            struct _eval_case<ActiveGrammar, TAG>                                                   \
              : active_grammar<when(TAG(ActiveGrammar, ActiveGrammar), _op_unpack<TAG, ActiveGrammar>)> \
            {};                                                                                     \
        }                                                                                           \
        /**/

        BOOST_PROTO_UNARY_EVAL(unary_plus)
        BOOST_PROTO_UNARY_EVAL(negate)
        BOOST_PROTO_UNARY_EVAL(dereference)
        BOOST_PROTO_UNARY_EVAL(complement)
        BOOST_PROTO_UNARY_EVAL(address_of)
        BOOST_PROTO_UNARY_EVAL(logical_not)
        BOOST_PROTO_UNARY_EVAL(pre_inc)
        BOOST_PROTO_UNARY_EVAL(pre_dec)
        BOOST_PROTO_UNARY_EVAL(post_inc)
        BOOST_PROTO_UNARY_EVAL(post_dec)

        BOOST_PROTO_BINARY_EVAL(shift_left)
        BOOST_PROTO_BINARY_EVAL(shift_right)
        BOOST_PROTO_BINARY_EVAL(multiplies)
        BOOST_PROTO_BINARY_EVAL(divides)
        BOOST_PROTO_BINARY_EVAL(modulus)
        BOOST_PROTO_BINARY_EVAL(plus)
        BOOST_PROTO_BINARY_EVAL(minus)
        BOOST_PROTO_BINARY_EVAL(less)
        BOOST_PROTO_BINARY_EVAL(greater)
        BOOST_PROTO_BINARY_EVAL(less_equal)
        BOOST_PROTO_BINARY_EVAL(greater_equal)
        BOOST_PROTO_BINARY_EVAL(equal_to)
        BOOST_PROTO_BINARY_EVAL(not_equal_to)
        BOOST_PROTO_BINARY_EVAL(logical_or)
        BOOST_PROTO_BINARY_EVAL(logical_and)
        BOOST_PROTO_BINARY_EVAL(bitwise_and)
        BOOST_PROTO_BINARY_EVAL(bitwise_or)
        BOOST_PROTO_BINARY_EVAL(bitwise_xor)
        BOOST_PROTO_BINARY_EVAL(comma)
        BOOST_PROTO_BINARY_EVAL(subscript)
        BOOST_PROTO_BINARY_EVAL(mem_ptr)
        //BOOST_PROTO_BINARY_EVAL(member)

        BOOST_PROTO_BINARY_EVAL(assign)
        BOOST_PROTO_BINARY_EVAL(shift_left_assign)
        BOOST_PROTO_BINARY_EVAL(shift_right_assign)
        BOOST_PROTO_BINARY_EVAL(multiplies_assign)
        BOOST_PROTO_BINARY_EVAL(divides_assign)
        BOOST_PROTO_BINARY_EVAL(modulus_assign)
        BOOST_PROTO_BINARY_EVAL(plus_assign)
        BOOST_PROTO_BINARY_EVAL(minus_assign)
        BOOST_PROTO_BINARY_EVAL(bitwise_and_assign)
        BOOST_PROTO_BINARY_EVAL(bitwise_or_assign)
        BOOST_PROTO_BINARY_EVAL(bitwise_xor_assign)

        #undef BOOST_PROTO_UNARY_EVAL
        #undef BOOST_PROTO_BINARY_EVAL

        template<>
        struct op<if_else_>
          : function
        {};

        template<>
        struct op<function>
          : function
        {};

        namespace detail
        {
            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, if_else_>
              : active_grammar<when(if_else_(ActiveGrammar, ActiveGrammar, ActiveGrammar), _op_unpack<if_else_, ActiveGrammar>)>
            {};

            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, function>
              : active_grammar<when(function(ActiveGrammar...), _op_unpack<function, ActiveGrammar>)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // _eval_cases
            template<typename ActiveGrammar>
            struct _eval_cases
            {
                template<typename Tag>
                struct case_
                  : _eval_case<ActiveGrammar, Tag>
                {};
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _eval
        template<typename ActiveGrammar>
        struct _eval
          : action<switch_(detail::_eval_cases<ActiveGrammar>)>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // eval
        struct eval
        {
            template<typename Tag, typename ...T>
            auto operator()(Tag, T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                BOOST_PROTO_TRY_CALL(op<Tag>())(static_cast<T &&>(t)...)
            )
        };

        namespace detail
        {
            // Loopy indirection that allows proto::_eval<> to be
            // used without specifying an ActiveGrammar argument.
            struct _eval
              : proto::_eval<>
            {};
        }
    }
}

#endif
