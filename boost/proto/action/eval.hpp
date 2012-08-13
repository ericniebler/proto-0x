///////////////////////////////////////////////////////////////////////////////
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
#include <boost/utility/addressof.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/children.hpp>
#include <boost/proto/matches.hpp>
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
            // proto_get_pointer
            template<typename T>
            auto proto_get_pointer(T & t, long)
            BOOST_PROTO_AUTO_RETURN(
                boost::addressof(t) // TODO replace with std::addressof
            )

            template<typename T>
            auto proto_get_pointer(T && t, int)
            BOOST_PROTO_AUTO_RETURN(
                get_pointer(static_cast<T &&>(t))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // mem_fun_binder_
            template<typename T, typename PMF>
            class mem_fun_binder_
            {
                T obj_;
                PMF pmf_;

            public:
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(mem_fun_binder_);

                template<typename U>
                mem_fun_binder_(U && u, PMF pmf) noexcept(noexcept(T(static_cast<U &&>(u))))
                  : obj_(static_cast<U &&>(u))
                  , pmf_(pmf)
                {}

                template<typename ...A>
                auto operator()(A &&... a) const
                BOOST_PROTO_AUTO_RETURN(
                    (detail::proto_get_pointer(obj_, 1) ->* pmf_)(static_cast<A &&>(a)...)
                )
            };

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
                    BOOST_PROTO_TRY_CALL(op<Tag>)()(
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
                        utility::make_indices<0, arity_of<Expr>::value>()
                      , static_cast<Expr &&>(expr)
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };

            template<typename Action>
            struct _op_unpack<terminal, Action>
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

    #define BOOST_PROTO_UNARY_EVAL(OP, TAG)                                                         \
        template<>                                                                                  \
        struct op<TAG>                                                                              \
        {                                                                                           \
            template<typename T>                                                                    \
            auto operator()(T && t) const                                                           \
            BOOST_PROTO_AUTO_RETURN(                                                                \
                OP static_cast<T &&>(t)                                                             \
            )                                                                                       \
        };                                                                                          \
                                                                                                    \
        namespace detail                                                                            \
        {                                                                                           \
            template<typename ActiveGrammar>                                                        \
            struct _eval_case<ActiveGrammar, TAG>                                                   \
              : active_grammar<when(TAG(ActiveGrammar), _op_unpack<TAG, ActiveGrammar>)>            \
            {};                                                                                     \
        }                                                                                           \
        /**/

    #define BOOST_PROTO_BINARY_EVAL(OP, TAG)                                                        \
        template<>                                                                                  \
        struct op<TAG>                                                                              \
        {                                                                                           \
            template<typename T, typename U>                                                        \
            auto operator()(T && t, U && u) const                                                   \
            BOOST_PROTO_AUTO_RETURN(                                                                \
                static_cast<T &&>(t) OP static_cast<U &&>(u)                                        \
            )                                                                                       \
        };                                                                                          \
                                                                                                    \
        namespace detail                                                                            \
        {                                                                                           \
            template<typename ActiveGrammar>                                                        \
            struct _eval_case<ActiveGrammar, TAG>                                                   \
              : active_grammar<when(TAG(ActiveGrammar, ActiveGrammar), _op_unpack<TAG, ActiveGrammar>)> \
            {};                                                                                     \
        }                                                                                           \
        /**/

        BOOST_PROTO_UNARY_EVAL(+, unary_plus)
        BOOST_PROTO_UNARY_EVAL(-, negate)
        BOOST_PROTO_UNARY_EVAL(*, dereference)
        BOOST_PROTO_UNARY_EVAL(~, complement)
        BOOST_PROTO_UNARY_EVAL(&, address_of)
        BOOST_PROTO_UNARY_EVAL(!, logical_not)
        BOOST_PROTO_UNARY_EVAL(++, pre_inc)
        BOOST_PROTO_UNARY_EVAL(--, pre_dec)

        BOOST_PROTO_BINARY_EVAL(<<, shift_left)
        BOOST_PROTO_BINARY_EVAL(>>, shift_right)
        BOOST_PROTO_BINARY_EVAL(*, multiplies)
        BOOST_PROTO_BINARY_EVAL(/, divides)
        BOOST_PROTO_BINARY_EVAL(%, modulus)
        BOOST_PROTO_BINARY_EVAL(+, plus)
        BOOST_PROTO_BINARY_EVAL(-, minus)
        BOOST_PROTO_BINARY_EVAL(<, less)
        BOOST_PROTO_BINARY_EVAL(>, greater)
        BOOST_PROTO_BINARY_EVAL(<=, less_equal)
        BOOST_PROTO_BINARY_EVAL(>=, greater_equal)
        BOOST_PROTO_BINARY_EVAL(==, equal_to)
        BOOST_PROTO_BINARY_EVAL(!=, not_equal_to)
        BOOST_PROTO_BINARY_EVAL(||, logical_or)
        BOOST_PROTO_BINARY_EVAL(&&, logical_and)
        BOOST_PROTO_BINARY_EVAL(&, bitwise_and)
        BOOST_PROTO_BINARY_EVAL(|, bitwise_or)
        BOOST_PROTO_BINARY_EVAL(^, bitwise_xor)
        BOOST_PROTO_BINARY_EVAL(BOOST_PP_COMMA(), comma)

        BOOST_PROTO_BINARY_EVAL(=, assign)
        BOOST_PROTO_BINARY_EVAL(<<=, shift_left_assign)
        BOOST_PROTO_BINARY_EVAL(>>=, shift_right_assign)
        BOOST_PROTO_BINARY_EVAL(*=, multiplies_assign)
        BOOST_PROTO_BINARY_EVAL(/=, divides_assign)
        BOOST_PROTO_BINARY_EVAL(%=, modulus_assign)
        BOOST_PROTO_BINARY_EVAL(+=, plus_assign)
        BOOST_PROTO_BINARY_EVAL(-=, minus_assign)
        BOOST_PROTO_BINARY_EVAL(&=, bitwise_and_assign)
        BOOST_PROTO_BINARY_EVAL(|=, bitwise_or_assign)
        BOOST_PROTO_BINARY_EVAL(^=, bitwise_xor_assign)

        #undef BOOST_PROTO_UNARY_EVAL
        #undef BOOST_PROTO_BINARY_EVAL

        template<>
        struct op<post_inc>
        {
            template<typename T>
            auto operator()(T && t) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T &&>(t)++
            )
        };

        template<>
        struct op<post_dec>
        {
            template<typename T>
            auto operator()(T && t) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T &&>(t)--
            )
        };

        template<>
        struct op<subscript>
        {
            template<typename T, typename U>
            auto operator()(T && t, U && u) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T &&>(t)[ static_cast<U &&>(u) ]
            )
        };

        template<>
        struct op<mem_ptr>
        {
            template<typename T, typename U>
            auto operator()(T && t, U && u) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T &&>(t) ->* static_cast<U &&>(u)
            )

            template<typename Obj, typename Type, typename Class>
            auto operator()(Obj && obj, Type Class::*pm) const
            BOOST_PROTO_AUTO_RETURN(
                (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pm)
            )

            template<typename Obj, typename Type, typename Class, typename ...Args>
            auto operator()(Obj && obj, Type (Class::*pmf)(Args...)) const
            BOOST_PROTO_AUTO_RETURN(
                detail::mem_fun_binder_<Obj, Type (Class::*)(Args...)>(static_cast<Obj &&>(obj), pmf)
            )
        };

        template<>
        struct op<if_else_>
        {
            template<typename T, typename U, typename V>
            auto operator()(T && t, U && u, V && v) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<T &&>(t) ? static_cast<U &&>(u) : static_cast<V &&>(v)
            )
        };

        template<>
        struct op<function>
        {
            template<typename Fun, typename ...T>
            auto operator()(Fun && fun, T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Fun &&>(fun)(static_cast<T &&>(t)...)
            )

            template<typename Type, typename Class, typename Obj>
            auto operator()(Type Class::*pm, Obj && obj) const
            BOOST_PROTO_AUTO_RETURN(
                (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pm)
            )

            template<typename Type, typename Class, typename ...Args, typename Obj, typename ...T>
            auto operator()(Type (Class::*pmf)(Args...), Obj && obj, T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pmf)(static_cast<T &&>(t)...)
            )
        };

        namespace detail
        {
            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, post_inc>
              : active_grammar<when(post_inc(ActiveGrammar), _op_unpack<post_inc, ActiveGrammar>)>
            {};

            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, post_dec>
              : active_grammar<when(post_dec(ActiveGrammar), _op_unpack<post_dec, ActiveGrammar>)>
            {};

            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, subscript>
              : active_grammar<when(subscript(ActiveGrammar, ActiveGrammar), _op_unpack<subscript, ActiveGrammar>)>
            {};

            template<typename ActiveGrammar>
            struct _eval_case<ActiveGrammar, mem_ptr>
              : active_grammar<when(mem_ptr(ActiveGrammar, ActiveGrammar), _op_unpack<mem_ptr, ActiveGrammar>)>
            {};

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
                BOOST_PROTO_TRY_CALL(op<Tag>)()(static_cast<T &&>(t)...)
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
