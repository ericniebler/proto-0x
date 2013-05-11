////////////////////////////////////////////////////////////////////////////////////////////////////
// virtual_member.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_VIRTUAL_MEMBER_HPP_INCLUDED
#define BOOST_PROTO_V5_VIRTUAL_MEMBER_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>
#include <boost/proto/v5/expr.hpp>
#include <boost/utility/addressof.hpp>

// INTERNAL ONLY
//
#define BOOST_PROTO_EXTENDS_MEMBER_DETAIL(R, THIS, ELEM)                                            \
    boost::proto::v5::virtual_member<                                                               \
        THIS                                                                                        \
      , BOOST_PP_TUPLE_ELEM(2, 0, ELEM)                                                             \
    > BOOST_PP_TUPLE_ELEM(2, 1, ELEM);                                                              \
    /**/

// \brief For declaring virtual data members in an extension class.
//
#define BOOST_PROTO_EXTENDS_MEMBERS(THIS, SEQ)                                                      \
    union                                                                                           \
    {                                                                                               \
        char proto_member_union_start_;                                                             \
        BOOST_PP_SEQ_FOR_EACH(BOOST_PROTO_EXTENDS_MEMBER_DETAIL, THIS, SEQ)                         \
    }                                                                                               \
    /**/

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace exprs
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // virtual_
                template<typename Expr>
                struct virtual_
                  : Expr
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // virtual_member_
                template<typename A, typename B>
                struct virtual_member_<tags::member(virtual_<A>, B)>
                  : basic_expr<tags::member(virtual_<A>, B)>
                  , expr_assign<virtual_member_<tags::member(virtual_<A>, B)>>
                  , expr_subscript<virtual_member_<tags::member(virtual_<A>, B)>>
                  , expr_function<virtual_member_<tags::member(virtual_<A>, B)>>
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(virtual_member_);
                    using expr_assign<virtual_member_>::operator=;
                };
            }
        }
    }
}

#endif
