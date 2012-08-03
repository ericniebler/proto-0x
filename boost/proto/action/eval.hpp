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
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/when.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
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
            // _eval_unknown
            struct _eval_unknown
              : basic_action<_eval_unknown>
            {
                template<typename E, typename ...T>
                utility::any operator()(E && e, T &&...) const noexcept
                {
                    static_assert(
                        utility::never<E>::value
                      , "proto::_eval doesn't know how to evaluate this expression!"
                    );
                    return utility::any();
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _eval_case
            template<typename Grammar, typename Tag>
            struct _eval_case
              : when<not_(_), _eval_unknown>
            {};

            template<typename Grammar>
            struct _eval_case<Grammar, tag::terminal>
              : when<tag::terminal(_), _value>
            {};

            #define BOOST_PROTO_UNARY_EVAL(OP, TAG)                                                 \
            template<typename Grammar>                                                              \
            struct BOOST_PP_CAT(_eval_, TAG)                                                        \
              : basic_action<BOOST_PP_CAT(_eval_, TAG)<Grammar>>                                       \
            {                                                                                       \
                template<typename E, typename ...T>                                                 \
                auto operator()(E && e, T &&... t) const                                            \
                BOOST_PROTO_AUTO_RETURN(                                                            \
                    OP action<Grammar>()(                                                     \
                        proto::child<0>(static_cast<E &&>(e))                                       \
                      , static_cast<T &&>(t)...                                                     \
                    )                                                                               \
                )                                                                                   \
            };                                                                                      \
                                                                                                    \
            template<typename Grammar>                                                              \
            struct _eval_case<Grammar, tag::TAG>                                                    \
              : when<tag::unary_expr(tag::TAG, Grammar), BOOST_PP_CAT(_eval_, TAG)<Grammar>>        \
            {};                                                                                     \
            /**/

            #define BOOST_PROTO_BINARY_EVAL(OP, TAG)                                                \
            template<typename Grammar>                                                              \
            struct BOOST_PP_CAT(_eval_, TAG)                                                        \
              : basic_action<BOOST_PP_CAT(_eval_, TAG)<Grammar>>                                       \
            {                                                                                       \
                template<typename E, typename ...T>                                                 \
                auto operator()(E && e, T &&... t) const                                            \
                BOOST_PROTO_AUTO_RETURN(                                                            \
                    action<Grammar>()(                                                        \
                        proto::child<0>(static_cast<E &&>(e))                                       \
                      , static_cast<T &&>(t)...                                                     \
                    ) OP                                                                            \
                    action<Grammar>()(                                                        \
                        proto::child<1>(static_cast<E &&>(e))                                       \
                      , static_cast<T &&>(t)...                                                     \
                    )                                                                               \
                )                                                                                   \
            };                                                                                      \
                                                                                                    \
            template<typename Grammar>                                                              \
            struct _eval_case<Grammar, tag::TAG>                                                    \
              : when<                                                                               \
                    tag::binary_expr(tag::TAG, Grammar, Grammar)                                    \
                  , BOOST_PP_CAT(_eval_, TAG)<Grammar>                                              \
                >                                                                                   \
            {};                                                                                     \
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
            BOOST_PROTO_BINARY_EVAL(BOOST_PP_COMMA(), comma)

            #undef BOOST_PROTO_UNARY_EVAL
            #undef BOOST_PROTO_BINARY_EVAL

            template<typename Grammar>
            struct _eval_post_inc
              : basic_action<_eval_post_inc<Grammar>>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ) ++
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::post_inc>
              : when<tag::post_inc(Grammar), _eval_post_inc<Grammar>>
            {};

            template<typename Grammar>
            struct _eval_post_dec
              : basic_action<_eval_post_dec<Grammar>>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ) --
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::post_dec>
              : when<tag::post_dec(Grammar), _eval_post_dec<Grammar>>
            {};

            template<typename Grammar>
            struct _eval_subscript
              : basic_action<_eval_subscript<Grammar>>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ) [
                        action<Grammar>()(
                            proto::child<1>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )
                    ]
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::subscript>
              : when<tag::subscript(Grammar, Grammar), _eval_subscript<Grammar>>
            {};

            template<typename Grammar>
            struct _eval_if_else_
              : basic_action<_eval_if_else_<Grammar>>
            {
                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    )
                  ? action<Grammar>()(
                        proto::child<1>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    )
                  : action<Grammar>()(
                        proto::child<2>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    )
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::if_else_>
              : when<tag::if_else_(Grammar, Grammar, Grammar), _eval_if_else_<Grammar>>
            {};

            template<typename Grammar>
            struct _eval_function
              : basic_action<_eval_function<Grammar>>
            {
                template<std::size_t...I, typename Fun, typename E, typename ...T>
                auto impl_(utility::indices<I...>, Fun && fun, E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Fun &&>(fun)(
                        action<Grammar>()(
                            proto::child<I>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )...
                    )
                )

                template<typename Type, typename Class, typename E, typename ...T>
                auto impl_(utility::indices<1>, Type Class::*pm, E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    (proto_get_pointer(action<Grammar>()(
                        proto::child<1>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ), 1) ->* pm)
                )

                template<std::size_t ...I, typename Type, typename Class, typename ...Args, typename E, typename ...T>
                auto impl_(utility::indices<1, I...>, Type (Class::*pmf)(Args...), E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    (proto_get_pointer(action<Grammar>()(
                        proto::child<1>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ), 1) ->* pmf)(
                        action<Grammar>()(
                            proto::child<I>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )...
                    )
                )

                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    this->impl_(
                        utility::make_indices<1, arity_of<E>::value>()
                      , action<Grammar>()(
                            proto::child<0>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )
                      , static_cast<E &&>(e)
                      , static_cast<T &&>(t)...
                    )
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::function>
              : when<tag::function(Grammar...), _eval_function<Grammar>>
            {};

            template<typename T, typename PMF>
            struct memfun
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(memfun);

                T obj_;
                PMF pmf_;

                template<typename U>
                memfun(U && u, PMF pmf) noexcept(noexcept(T(static_cast<U &&>(u))))
                  : obj_(static_cast<U &&>(u))
                  , pmf_(pmf)
                {}

                template<typename ...A>
                auto operator()(A &&... a) const
                BOOST_PROTO_AUTO_RETURN(
                    (proto_get_pointer(obj_, 1) ->* pmf_)(static_cast<A &&>(a)...)
                )
            };

            template<typename T, typename PMF>
            auto as_memfun(T && t, PMF pmf)
            BOOST_PROTO_AUTO_RETURN(
                memfun<T, PMF>(static_cast<T &&>(t), pmf)
            )

            template<typename Grammar>
            struct _eval_mem_ptr
              : basic_action<_eval_mem_ptr<Grammar>>
            {
                template<typename Right, typename E, typename ...T>
                auto impl_(Right && r, E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ) ->* static_cast<Right &&>(r)
                )

                template<typename Type, typename Class, typename E, typename ...T>
                auto impl_(Type Class::*pm, E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    (proto_get_pointer(action<Grammar>()(
                        proto::child<0>(static_cast<E &&>(e))
                      , static_cast<T &&>(t)...
                    ), 1) ->* pm)
                )

                template<typename Type, typename Class, typename ...Args, typename E, typename ...T>
                auto impl_(Type (Class::*pmf)(Args...), E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::as_memfun(
                        action<Grammar>()(
                            proto::child<0>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )
                      , pmf
                    )
                )

                template<typename E, typename ...T>
                auto operator()(E && e, T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    this->impl_(
                        action<Grammar>()(
                            proto::child<1>(static_cast<E &&>(e))
                          , static_cast<T &&>(t)...
                        )
                      , static_cast<E &&>(e)
                      , static_cast<T &&>(t)...
                    )
                )
            };

            template<typename Grammar>
            struct _eval_case<Grammar, tag::mem_ptr>
              : when<tag::mem_ptr(Grammar, Grammar), _eval_mem_ptr<Grammar>>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // _eval_cases
            template<typename Grammar>
            struct _eval_cases
            {
                template<typename Tag>
                struct case_
                  : _eval_case<Grammar, Tag>
                {};
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _eval
        template<typename Grammar>
        struct _eval
          : action<switch_(detail::_eval_cases<Grammar>)>
        {};

        namespace detail
        {
            // Loopy indirection that allows proto::_eval<> to be
            // used without specifying a Grammar argument.
            struct _eval
              : proto::_eval<>
            {};
        }
    }
}

#endif
