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

namespace boost { namespace proto
{
    //namespace detail
    //{
    //    template<typename MakeExpr, typename Grammar>
    //    struct lazy_matches
    //      : proto::matches<typename MakeExpr::type, Grammar>
    //    {};

    //    template<typename Domain, typename Grammar, typename Trait, typename Tag, typename Arg>
    //    struct enable_unary
    //      : boost::lazy_enable_if_c<
    //            boost::mpl::and_<
    //                Trait
    //              , lazy_matches<result_of::make_expr<Tag, basic_default_domain, Arg>, Grammar>
    //            >::value
    //          , result_of::make_expr<Tag, Domain, Arg>
    //        >
    //    {};

    //    template<typename Domain, typename Trait, typename Tag, typename Arg>
    //    struct enable_unary<Domain, proto::_, Trait, Tag, Arg &>
    //      : boost::lazy_enable_if_c<
    //            Trait::value
    //          , result_of::make_expr<Tag, Domain, Arg &>
    //        >
    //    {};

    //    template<typename Trait, typename Tag, typename Arg>
    //    struct enable_unary<deduce_domain, not_a_grammar, Trait, Tag, Arg &>
    //      : enable_unary<
    //            typename domain_of<Arg>::type
    //          , typename domain_of<Arg>::type::proto_grammar
    //          , Trait
    //          , Tag
    //          , Arg &
    //        >
    //    {};

    //    template<typename Domain, typename Grammar, typename Trait, typename Tag, typename Left, typename Right>
    //    struct enable_binary
    //      : boost::lazy_enable_if_c<
    //            boost::mpl::and_<
    //                Trait
    //              , lazy_matches<result_of::make_expr<Tag, basic_default_domain, Left, Right>, Grammar>
    //            >::value
    //          , result_of::make_expr<Tag, Domain, Left, Right>
    //        >
    //    {};

    //    template<typename Domain, typename Trait, typename Tag, typename Left, typename Right>
    //    struct enable_binary<Domain, proto::_, Trait, Tag, Left &, Right &>
    //      : boost::lazy_enable_if_c<
    //            Trait::value
    //          , result_of::make_expr<Tag, Domain, Left &, Right &>
    //        >
    //    {};

    //    template<typename Trait, typename Tag, typename Left, typename Right>
    //    struct enable_binary<deduce_domain, not_a_grammar, Trait, Tag, Left &, Right &>
    //      : enable_binary<
    //            typename deduce_domain2<Left, Right>::type
    //          , typename deduce_domain2<Left, Right>::type::proto_grammar
    //          , Trait
    //          , Tag
    //          , Left &
    //          , Right &
    //        >
    //    {};

    //} // detail

#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_0
#define BOOST_PROTO_UNARY_OP_IS_POSTFIX_1 , int

#define BOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg, BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_UNARY_(TRAIT, Arg))>             \
    inline auto operator OP(Arg &&arg BOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                     \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::domains::make_expr<DOMAIN>(TAG(), static_cast<Arg &&>(arg))                 \
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right, BOOST_PROTO_ENABLE_IF(BOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right))>\
    inline auto operator OP(Left &&left, Right &&right)                                             \
    BOOST_PROTO_AUTO_RETURN(                                                                        \
        ::boost::proto::domains::make_expr<DOMAIN>(TAG(), static_cast<Left &&>(left), static_cast<Right &&>(right))\
    )                                                                                               \
    /**/

#define BOOST_PROTO_DEFINE_OPERATORS(TRAIT, DOMAIN)                                                 \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(+, boost::proto::tag::unary_plus, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::tag::negate, TRAIT, DOMAIN, 0)               \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(*, boost::proto::tag::dereference, TRAIT, DOMAIN, 0)          \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(~, boost::proto::tag::complement, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(&, boost::proto::tag::address_of, TRAIT, DOMAIN, 0)           \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(!, boost::proto::tag::logical_not, TRAIT, DOMAIN, 0)          \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::tag::pre_inc, TRAIT, DOMAIN, 0)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::tag::pre_dec, TRAIT, DOMAIN, 0)             \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(++, boost::proto::tag::post_inc, TRAIT, DOMAIN, 1)            \
    BOOST_PROTO_DEFINE_UNARY_OPERATOR(--, boost::proto::tag::post_dec, TRAIT, DOMAIN, 1)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<, boost::proto::tag::shift_left, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>, boost::proto::tag::shift_right, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::tag::multiplies, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/, boost::proto::tag::divides, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%, boost::proto::tag::modulus, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::tag::plus, TRAIT, DOMAIN)                   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::tag::minus, TRAIT, DOMAIN)                  \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<, boost::proto::tag::less, TRAIT, DOMAIN)                   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>, boost::proto::tag::greater, TRAIT, DOMAIN)                \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<=, boost::proto::tag::less_equal, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>=, boost::proto::tag::greater_equal, TRAIT, DOMAIN)         \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(==, boost::proto::tag::equal_to, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(!=, boost::proto::tag::not_equal_to, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(||, boost::proto::tag::logical_or, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&&, boost::proto::tag::logical_and, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&, boost::proto::tag::bitwise_and, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|, boost::proto::tag::bitwise_or, TRAIT, DOMAIN)             \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^, boost::proto::tag::bitwise_xor, TRAIT, DOMAIN)            \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(BOOST_PP_COMMA(), boost::proto::tag::comma, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(->*, boost::proto::tag::mem_ptr, TRAIT, DOMAIN)              \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(<<=, boost::proto::tag::shift_left_assign, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(>>=, boost::proto::tag::shift_right_assign, TRAIT, DOMAIN)   \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(*=, boost::proto::tag::multiplies_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(/=, boost::proto::tag::divides_assign, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(%=, boost::proto::tag::modulus_assign, TRAIT, DOMAIN)        \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(+=, boost::proto::tag::plus_assign, TRAIT, DOMAIN)           \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(-=, boost::proto::tag::minus_assign, TRAIT, DOMAIN)          \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(&=, boost::proto::tag::bitwise_and_assign, TRAIT, DOMAIN)    \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(|=, boost::proto::tag::bitwise_or_assign, TRAIT, DOMAIN)     \
    BOOST_PROTO_DEFINE_BINARY_OPERATOR(^=, boost::proto::tag::bitwise_xor_assign, TRAIT, DOMAIN)    \
    /**/

    // Extensions are a superset of Proto expressions
    template<typename T>
    struct is_extension
      : is_expr<T>
    {};

    #define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)          (TRAIT<ARG>::value)
    #define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT) (TRAIT<LEFT>::value || TRAIT<RIGHT>::value)

    namespace exprs
    {
        // This defines all of Proto's built-in free operator overloads
        BOOST_PROTO_DEFINE_OPERATORS(is_extension, deduce_domain)

        // if_else, for the non-overloadable ternary conditional operator ?:
        template<typename A0, typename A1, typename A2>
        inline auto if_else(A0 &&a0, A1 &&a1, A2 &&a2)
        BOOST_PROTO_AUTO_RETURN(
            ::boost::proto::domains::make_expr<
                typename detail::common_domain2<
                    decltype(detail::get_domain(a1))
                  , decltype(detail::get_domain(a2))
                >::type
            >(
                tag::if_else_()
              , static_cast<A0 &&>(a0)
              , static_cast<A1 &&>(a1)
              , static_cast<A2 &&>(a2)
            )
        )
    }

    using exprs::if_else;

    #undef BOOST_PROTO_APPLY_UNARY_
    #undef BOOST_PROTO_APPLY_BINARY_

    // Redefine BOOST_PROTO_APPLY_UNARY_ and BOOST_PROTO_APPLY_BINARY_ so that end users
    // can use BOOST_PROTO_DEFINE_OPERATORS to define Proto operator overloads that work
    // with their own terminal types.

    #define BOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)                                                    \
        (boost::mpl::and_<                                                                          \
            TRAIT<typename std::remove_const<typename std::remove_reference<ARG>::type>::type>      \
          , boost::mpl::not_<boost::proto::is_extension<ARG> >                                      \
        >::value)                                                                                   \
        /**/

    #define BOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT)                                           \
        (boost::mpl::and_<                                                                          \
            boost::mpl::or_<                                                                        \
                TRAIT<typename std::remove_const<typename std::remove_reference<LEFT>::type>::type> \
              , TRAIT<typename std::remove_const<typename std::remove_reference<RIGHT>::type>::type>\
            >                                                                                       \
          , boost::mpl::not_<                                                                       \
                boost::mpl::or_<                                                                    \
                    boost::proto::is_extension<LEFT>                                                \
                  , boost::proto::is_extension<RIGHT>                                               \
                >                                                                                   \
            >                                                                                       \
        >::value)                                                                                   \
        /**/

}}

#endif
