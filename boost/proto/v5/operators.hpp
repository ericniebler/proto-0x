////////////////////////////////////////////////////////////////////////////////////////////////////
// operators.hpp
// Contains all the overloaded operators that make it possible to build
// Proto expression trees.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_OPERATORS_HPP_INCLUDED
#define BOOST_PROTO_CXX11_OPERATORS_HPP_INCLUDED

#include <type_traits>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/domain.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/make_expr.hpp>

#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_0
#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_1 , int

#define BOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg                                                                           \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_UNARY_(TRAIT, Arg))                                 \
      , BOOST_PROTO_ENABLE_IF(::boost::proto::v5::detail::is_grammatical<TAG(Arg), DOMAIN>::value)  \
    >                                                                                               \
    inline auto operator OP(Arg &&arg BOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                     \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::v5::make_expr<TAG, DOMAIN>(static_cast<Arg &&>(arg))                        \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right                                                          \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right))                        \
      , BOOST_PROTO_ENABLE_IF(                                                                      \
            ::boost::proto::v5::detail::is_grammatical<TAG(Left, Right), DOMAIN>::value             \
        )                                                                                           \
    >                                                                                               \
    inline auto operator OP(Left &&left, Right &&right)                                             \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::v5::make_expr<TAG, DOMAIN>(                                                 \
            static_cast<Left &&>(left)                                                              \
          , static_cast<Right &&>(right)                                                            \
        )                                                                                           \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_OPERATORS(TRAIT, DOMAIN)                                                 \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(+, boost::proto::v5::unary_plus, TRAIT, DOMAIN, 0)            \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::v5::negate, TRAIT, DOMAIN, 0)                \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(*, boost::proto::v5::dereference, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(~, boost::proto::v5::complement, TRAIT, DOMAIN, 0)            \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(&, boost::proto::v5::address_of, TRAIT, DOMAIN, 0)            \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(!, boost::proto::v5::logical_not, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::v5::pre_inc, TRAIT, DOMAIN, 0)              \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::v5::pre_dec, TRAIT, DOMAIN, 0)              \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::v5::post_inc, TRAIT, DOMAIN, 1)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::v5::post_dec, TRAIT, DOMAIN, 1)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<, boost::proto::v5::shift_left, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>, boost::proto::v5::shift_right, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::v5::multiplies, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/, boost::proto::v5::divides, TRAIT, DOMAIN)                 \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%, boost::proto::v5::modulus, TRAIT, DOMAIN)                 \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::v5::plus, TRAIT, DOMAIN)                    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::v5::minus, TRAIT, DOMAIN)                   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<, boost::proto::v5::less, TRAIT, DOMAIN)                    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>, boost::proto::v5::greater, TRAIT, DOMAIN)                 \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<=, boost::proto::v5::less_equal, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>=, boost::proto::v5::greater_equal, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(==, boost::proto::v5::equal_to, TRAIT, DOMAIN)               \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(!=, boost::proto::v5::not_equal_to, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(||, boost::proto::v5::logical_or, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&&, boost::proto::v5::logical_and, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&, boost::proto::v5::bitwise_and, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|, boost::proto::v5::bitwise_or, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^, boost::proto::v5::bitwise_xor, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(BOOST_PP_COMMA(), boost::proto::v5::comma, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(->*, boost::proto::v5::mem_ptr, TRAIT, DOMAIN)               \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<=, boost::proto::v5::shift_left_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>=, boost::proto::v5::shift_right_assign, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*=, boost::proto::v5::multiplies_assign, TRAIT, DOMAIN)      \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/=, boost::proto::v5::divides_assign, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%=, boost::proto::v5::modulus_assign, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+=, boost::proto::v5::plus_assign, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-=, boost::proto::v5::minus_assign, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&=, boost::proto::v5::bitwise_and_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|=, boost::proto::v5::bitwise_or_assign, TRAIT, DOMAIN)      \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^=, boost::proto::v5::bitwise_xor_assign, TRAIT, DOMAIN)     \
    /**/

#define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)          (TRAIT<ARG>::value)
#define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT) (TRAIT<LEFT>::value || TRAIT<RIGHT>::value)

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // is_grammatical
                template<typename ExprDesc, typename Domain>
                struct is_grammatical
                {
                    using domain_type = domains::basic_expr_domain_adaptor<Domain>;
                    using expr_type = typename result_of::make_expr<ExprDesc, domain_type>::type;
                    using grammar_type = typename expr_type::proto_domain_type::proto_grammar_type;
                    static constexpr bool value = proto::matches<expr_type, grammar_type>::value;
                    using type = std::integral_constant<bool, value>;
                };
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
                    v5::make_expr<if_else_>(
                        static_cast<A0 &&>(a0)
                      , static_cast<A1 &&>(a1)
                      , static_cast<A2 &&>(a2)
                    )
                )

                /// INTERNAL ONLY
                template<typename T, typename U>
                auto mem_ptr_helper(T && t, U && u, int)
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) ->* static_cast<U &&>(u)
                )
            }

            using exprs::if_else;
        }
    }
}

#undef BOOST_PROTO_APPLY_UNARY_
#undef BOOST_PROTO_APPLY_BINARY_

// Redefine BOOST_PROTO_APPLY_UNARY_ and BOOST_PROTO_APPLY_BINARY_ so that end users
// can use BOOST_PROTO_DEFINE_OPERATORS to define Proto operator overloads that work
// with their own terminal types.

#define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)                                                        \
    (boost::mpl::and_<                                                                              \
        TRAIT<boost::proto::v5::utility::uncvref<ARG>>                                              \
      , boost::mpl::not_<boost::proto::v5::is_extension<ARG>>                                       \
    >::value)                                                                                       \
    /**/

#define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT)                                               \
    (boost::mpl::and_<                                                                              \
        boost::mpl::or_<                                                                            \
            TRAIT<boost::proto::v5::utility::uncvref<LEFT>>                                         \
          , TRAIT<boost::proto::v5::utility::uncvref<RIGHT>>                                        \
        >                                                                                           \
      , boost::mpl::not_<                                                                           \
            boost::mpl::or_<                                                                        \
                boost::proto::v5::is_extension<LEFT>                                                \
              , boost::proto::v5::is_extension<RIGHT>                                               \
            >                                                                                       \
        >                                                                                           \
    >::value)                                                                                       \
    /**/

#endif
