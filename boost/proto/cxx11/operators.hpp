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
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/tags.hpp>
#include <boost/proto/cxx11/domain.hpp>
#include <boost/proto/cxx11/matches.hpp>

#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_0
#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_1 , int

#define BOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg                                                                           \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_UNARY_(TRAIT, Arg))>                                \
    inline auto operator OP(Arg &&arg BOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                     \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::cxx11::detail::make_expr_if<DOMAIN>(                                        \
            TAG(), static_cast<Arg &&>(arg))                                                        \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right                                                          \
      , BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right))>                       \
    inline auto operator OP(Left &&left, Right &&right)                                             \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::cxx11::detail::make_expr_if<DOMAIN>(                                        \
            TAG(), static_cast<Left &&>(left), static_cast<Right &&>(right))                        \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_OPERATORS(TRAIT, DOMAIN)                                                 \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(+, boost::proto::cxx11::unary_plus, TRAIT, DOMAIN, 0)         \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::cxx11::negate, TRAIT, DOMAIN, 0)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(*, boost::proto::cxx11::dereference, TRAIT, DOMAIN, 0)        \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(~, boost::proto::cxx11::complement, TRAIT, DOMAIN, 0)         \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(&, boost::proto::cxx11::address_of, TRAIT, DOMAIN, 0)         \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(!, boost::proto::cxx11::logical_not, TRAIT, DOMAIN, 0)        \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::cxx11::pre_inc, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::cxx11::pre_dec, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::cxx11::post_inc, TRAIT, DOMAIN, 1)          \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::cxx11::post_dec, TRAIT, DOMAIN, 1)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<, boost::proto::cxx11::shift_left, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>, boost::proto::cxx11::shift_right, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::cxx11::multiplies, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/, boost::proto::cxx11::divides, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%, boost::proto::cxx11::modulus, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::cxx11::plus, TRAIT, DOMAIN)                 \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::cxx11::minus, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<, boost::proto::cxx11::less, TRAIT, DOMAIN)                 \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>, boost::proto::cxx11::greater, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<=, boost::proto::cxx11::less_equal, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>=, boost::proto::cxx11::greater_equal, TRAIT, DOMAIN)       \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(==, boost::proto::cxx11::equal_to, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(!=, boost::proto::cxx11::not_equal_to, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(||, boost::proto::cxx11::logical_or, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&&, boost::proto::cxx11::logical_and, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&, boost::proto::cxx11::bitwise_and, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|, boost::proto::cxx11::bitwise_or, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^, boost::proto::cxx11::bitwise_xor, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(BOOST_PP_COMMA(), boost::proto::cxx11::comma, TRAIT, DOMAIN) \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(->*, boost::proto::cxx11::mem_ptr, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<=, boost::proto::cxx11::shift_left_assign, TRAIT, DOMAIN)  \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>=, boost::proto::cxx11::shift_right_assign, TRAIT, DOMAIN) \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*=, boost::proto::cxx11::multiplies_assign, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/=, boost::proto::cxx11::divides_assign, TRAIT, DOMAIN)      \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%=, boost::proto::cxx11::modulus_assign, TRAIT, DOMAIN)      \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+=, boost::proto::cxx11::plus_assign, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-=, boost::proto::cxx11::minus_assign, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&=, boost::proto::cxx11::bitwise_and_assign, TRAIT, DOMAIN)  \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|=, boost::proto::cxx11::bitwise_or_assign, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^=, boost::proto::cxx11::bitwise_xor_assign, TRAIT, DOMAIN)  \
    /**/

#define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)          (TRAIT<ARG>::value)
#define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT) (TRAIT<LEFT>::value || TRAIT<RIGHT>::value)

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // as_basic_expr
                template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
                auto as_basic_expr(T &&t) ->
                    decltype(typename Domain::store_child()(t));

                template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
                auto as_basic_expr(T &&t) ->
                    exprs::basic_expr<
                        terminal(decltype(typename Domain::store_value()(t)))
                      , Domain
                    >;

                ////////////////////////////////////////////////////////////////////////////////////
                // expr_maker_if_2_
                template<typename ExprDesc, typename Domain
                  , typename E = exprs::basic_expr<ExprDesc, Domain>
                  , typename G = typename Domain::proto_grammar_type
                  , BOOST_PROTO_ENABLE_IF(matches<E, G>::value)
                >
                inline constexpr auto expr_maker_if_2_()
                BOOST_PROTO_AUTO_RETURN(
                    typename Domain::make_expr_raw()
                )

                ////////////////////////////////////////////////////////////////////////////////////
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

                ////////////////////////////////////////////////////////////////////////////////////
                // expr_maker_if
                template<typename Domain, typename ...U>
                inline constexpr auto expr_maker_if(Domain, U const &...u)
                BOOST_PROTO_AUTO_RETURN(
                    detail::make_expr_if_<decltype(detail::get_common_domain(Domain(), u...))>()
                )

                ////////////////////////////////////////////////////////////////////////////////////
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
                    cxx11::make_expr<tags::if_else_, deduce_domain>(
                        static_cast<A0 &&>(a0)
                      , static_cast<A1 &&>(a1)
                      , static_cast<A2 &&>(a2)
                    )
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
        TRAIT<typename std::remove_const<typename std::remove_reference<ARG>::type>::type>          \
      , boost::mpl::not_<boost::proto::cxx11::is_extension<ARG>>                                    \
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
                boost::proto::cxx11::is_extension<LEFT>                                             \
              , boost::proto::cxx11::is_extension<RIGHT>                                            \
            >                                                                                       \
        >                                                                                           \
    >::value)                                                                                       \
    /**/

#endif
