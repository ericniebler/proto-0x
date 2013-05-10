////////////////////////////////////////////////////////////////////////////////////////////////////
// operators.hpp
// Contains all the overloaded operators that make it possible to build
// Proto expression trees.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_OPERATORS_HPP_INCLUDED
#define BOOST_PROTO_V5_OPERATORS_HPP_INCLUDED

#include <type_traits>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/domain.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/make_expr.hpp>

#define BOOST_PROTO_DEFINE_OPERATORS() <boost/proto/v5/detail/operators.hpp>

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
                template<typename Expr>
                struct is_grammatical_impl
                  : proto::matches<
                        Expr
                      , typename result_of::domain_of<Expr>::type::proto_grammar_type
                    >
                {};

                template<typename ExprDesc, typename Domain>
                struct is_grammatical
                  : is_grammatical_impl<
                        typename result_of::make_expr<
                            ExprDesc
                          , domains::basic_expr_domain_adaptor<Domain>
                        >::type
                    >
                {};
            }

            // Extensions are a superset of Proto expressions
            template<typename T>
            struct is_extension
              : is_expr<T>
            {};

            namespace exprs
            {
                // This defines all of Proto's built-in free operator overloads
                #define BOOST_PROTO_OPERATOR_TRAITS (is_extension, deduce_domain)
                #include BOOST_PROTO_DEFINE_OPERATORS()

                // if_else, for the non-overloadable ternary conditional operator ?:
                template<typename A0, typename A1, typename A2>
                inline auto if_else(A0 &&a0, A1 &&a1, A2 &&a2)
                BOOST_PROTO_AUTO_RETURN(
                    v5::make_expr<if_else_, deduce_domain>(
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
