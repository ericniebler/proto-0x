////////////////////////////////////////////////////////////////////////////////////////////////////
// accessors.hpp
// child, left, right, value
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACCESSORS_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACCESSORS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace exprs
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // child
                template<std::size_t I, typename ExprDesc, typename Domain>
                inline constexpr auto child(basic_expr<ExprDesc, Domain> &e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<I>(e.proto_args())
                )

                template<std::size_t I, typename ExprDesc, typename Domain>
                inline constexpr auto child(basic_expr<ExprDesc, Domain> const &e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<I>(e.proto_args())
                )

                template<std::size_t I, typename ExprDesc, typename Domain>
                inline constexpr auto child(basic_expr<ExprDesc, Domain> &&e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<I>(static_cast<basic_expr<ExprDesc, Domain> &&>(e).proto_args())
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // get when 0th element is "virtual" (see virtual_member)
                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L & child(virtual_member<L, R, Domain> &a)
                BOOST_PROTO_RETURN(
                    *(L *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
                )

                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L const & child(virtual_member<L, R, Domain> const &a)
                BOOST_PROTO_RETURN(
                    *(L const *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
                )

                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L && child(virtual_member<L, R, Domain> &&a)
                BOOST_PROTO_RETURN(
                    static_cast<L &&>(*(L*)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // left
                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(e.proto_args())
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> const & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(e.proto_args())
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> && e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(static_cast<basic_expr<Tag(L, R), Domain> &&>(e).proto_args())
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // left when left element is "virtual" (see virtual_member)
                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L & left(virtual_member<L, R, Domain> &a)
                BOOST_PROTO_RETURN(
                    *(L *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
                )

                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L const & left(virtual_member<L, R, Domain> const &a)
                BOOST_PROTO_RETURN(
                    *(L const *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
                )

                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L && left(virtual_member<L, R, Domain> &&a)
                BOOST_PROTO_RETURN(
                    static_cast<L &&>(*(L*)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // right
                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(e.proto_args())
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> const & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(e.proto_args())
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> && e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(static_cast<basic_expr<Tag(L, R), Domain> &&>(e).proto_args())
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // value
                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> &that)
                BOOST_PROTO_AUTO_RETURN(
                    (that.proto_args().proto_child0) // extra parens are significant!
                )

                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> const &that)
                BOOST_PROTO_AUTO_RETURN(
                    (that.proto_args().proto_child0) // extra parens are significant!
                )

                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> &&that)
                BOOST_PROTO_AUTO_RETURN(
                    (static_cast<basic_expr<ExprDesc, Domain> &&>(that).proto_args().proto_child0)  // extra parens are significant!
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // tag_of
                template<typename ExprDesc, typename Domain>
                inline constexpr auto tag_of(basic_expr<ExprDesc, Domain> &that) noexcept
                    -> typename basic_expr<ExprDesc, Domain>::proto_tag_type &
                {
                    return that.proto_tag();
                }

                template<typename ExprDesc, typename Domain>
                inline constexpr auto tag_of(basic_expr<ExprDesc, Domain> const &that) noexcept
                    -> typename basic_expr<ExprDesc, Domain>::proto_tag_type const &
                {
                    return that.proto_tag();
                }

                template<typename ExprDesc, typename Domain>
                inline constexpr auto tag_of(basic_expr<ExprDesc, Domain> &&that) noexcept
                    -> typename basic_expr<ExprDesc, Domain>::proto_tag_type &&
                {
                    return static_cast<basic_expr<ExprDesc, Domain> &&>(that).proto_tag();
                }
            }

            using exprs::child;
            using exprs::left;
            using exprs::right;
            using exprs::value;
            using exprs::tag_of;
        }
    }
}

#endif // BOOST_PROTO_CXX11_ACCESSORS_HPP_INCLUDED
