////////////////////////////////////////////////////////////////////////////////////////////////////
// operators.hpp
// Contains all the overloaded operators that make it possible to build
// Proto expression trees.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_OPERATORS_HPP_INCLUDED
#define BOOST_PROTO_OPERATORS_HPP_INCLUDED

#include <type_traits>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/domain.hpp>
#include <boost/proto/matches.hpp>

#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_0
#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_1 , int

#define BOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg                                                                           \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_UNARY_(TRAIT, Arg))>                                \
    inline auto operator OP(Arg &&arg BOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                     \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::detail::make_expr_if<DOMAIN>(                                               \
            TAG(), static_cast<Arg &&>(arg))                                                        \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right                                                          \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right))>                       \
    inline auto operator OP(Left &&left, Right &&right)                                             \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::detail::make_expr_if<DOMAIN>(                                               \
            TAG(), static_cast<Left &&>(left), static_cast<Right &&>(right))                        \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_OPERATORS(TRAIT, DOMAIN)                                                 \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(+, boost::proto::unary_plus, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::negate, TRAIT, DOMAIN, 0)               \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(*, boost::proto::dereference, TRAIT, DOMAIN, 0)          \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(~, boost::proto::complement, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(&, boost::proto::address_of, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(!, boost::proto::logical_not, TRAIT, DOMAIN, 0)          \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::pre_inc, TRAIT, DOMAIN, 0)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::pre_dec, TRAIT, DOMAIN, 0)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::post_inc, TRAIT, DOMAIN, 1)            \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::post_dec, TRAIT, DOMAIN, 1)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<, boost::proto::shift_left, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>, boost::proto::shift_right, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::multiplies, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/, boost::proto::divides, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%, boost::proto::modulus, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::plus, TRAIT, DOMAIN)                   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::minus, TRAIT, DOMAIN)                  \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<, boost::proto::less, TRAIT, DOMAIN)                   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>, boost::proto::greater, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<=, boost::proto::less_equal, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>=, boost::proto::greater_equal, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(==, boost::proto::equal_to, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(!=, boost::proto::not_equal_to, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(||, boost::proto::logical_or, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&&, boost::proto::logical_and, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&, boost::proto::bitwise_and, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|, boost::proto::bitwise_or, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^, boost::proto::bitwise_xor, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(BOOST_PP_COMMA(), boost::proto::comma, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(->*, boost::proto::mem_ptr, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<=, boost::proto::shift_left_assign, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>=, boost::proto::shift_right_assign, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*=, boost::proto::multiplies_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/=, boost::proto::divides_assign, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%=, boost::proto::modulus_assign, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+=, boost::proto::plus_assign, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-=, boost::proto::minus_assign, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&=, boost::proto::bitwise_and_assign, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|=, boost::proto::bitwise_or_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^=, boost::proto::bitwise_xor_assign, TRAIT, DOMAIN)    \
    /**/

#define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)          (TRAIT<ARG>::value)
#define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT) (TRAIT<LEFT>::value || TRAIT<RIGHT>::value)

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // as_basic_expr
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            auto as_basic_expr(T &&t) ->
                decltype(typename Domain::store_child()(t));

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            auto as_basic_expr(T &&t) ->
                basic_expr<
                    terminal(decltype(typename Domain::store_value()(t)))
                  , Domain
                >;

            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_maker_if_2_
            template<typename ExprDesc, typename Domain
              , typename E = basic_expr<ExprDesc, Domain>
              , typename G = typename Domain::proto_grammar_type
              , BOOST_PROTO_ENABLE_IF(matches<E, G>::value)
            >
            inline constexpr auto expr_maker_if_2_()
            BOOST_PROTO_AUTO_RETURN(
                typename Domain::make_expr_raw()
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr_if_
            template<typename Domain>
            struct make_expr_if_
            {
                template<typename Tag, typename ...T>
                constexpr auto operator()(Tag tag, T &&...t) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::expr_maker_if_2_<
                        Tag(decltype(detail::as_basic_expr<Domain>(static_cast<T &&>(t)))...)
                      , Domain
                    >()(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_maker_if
            template<typename Domain, typename ...U>
            inline constexpr auto expr_maker_if(Domain, U const &...u)
            BOOST_PROTO_AUTO_RETURN(
                detail::make_expr_if_<decltype(detail::get_common_domain(Domain(), u...))>()
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr_if (with domain) - refuse to create expressions that are non-grammatical
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr_if(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker_if(Domain(), t...)(
                    static_cast<Tag &&>(tag)
                  , static_cast<T &&>(t)...
                )
            )
        }

        // Extensions are a superset of Proto expressions
        template<typename T>
        struct is_extension
          : is_expr<T>
        {};

        namespace exprs
        {
            // This defines all of Proto's built-in free operator overloads
            BOOST_PROTO_DEFINE_OPERATORS(is_extension, deduce_domain)

            // if_else, for the non-overloadable ternary conditional operator ?:
            template<typename A0, typename A1, typename A2>
            inline auto if_else(A0 &&a0, A1 &&a1, A2 &&a2)
            BOOST_PROTO_AUTO_RETURN(
                ::boost::proto::domains::make_expr<deduce_domain>(
                    tags::if_else_()
                  , static_cast<A0 &&>(a0)
                  , static_cast<A1 &&>(a1)
                  , static_cast<A2 &&>(a2)
                )
            )
        }

        using exprs::if_else;
    }
}

#undef BOOST_PROTO_APPLY_UNARY_
#undef BOOST_PROTO_APPLY_BINARY_

// Redefine BOOST_PROTO_APPLY_UNARY_ and BOOST_PROTO_APPLY_BINARY_ so that end users
// can use BOOST_PROTO_DEFINE_OPERATORS to define Proto operator overloads that work
// with their own terminal types.

#define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)                                                        \
    (boost::mpl::and_<                                                                              \
        TRAIT<typename std::remove_const<typename std::remove_reference<ARG>::type>::type>          \
      , boost::mpl::not_<boost::proto::is_extension<ARG>>                                           \
    >::value)                                                                                       \
    /**/

#define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT)                                               \
    (boost::mpl::and_<                                                                              \
        boost::mpl::or_<                                                                            \
            TRAIT<typename std::remove_const<typename std::remove_reference<LEFT>::type>::type>     \
          , TRAIT<typename std::remove_const<typename std::remove_reference<RIGHT>::type>::type>    \
        >                                                                                           \
      , boost::mpl::not_<                                                                           \
            boost::mpl::or_<                                                                        \
                boost::proto::is_extension<LEFT>                                                    \
              , boost::proto::is_extension<RIGHT>                                                   \
            >                                                                                       \
        >                                                                                           \
    >::value)                                                                                       \
    /**/

#endif
