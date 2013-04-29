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
#include <boost/proto/v5/detail/access.hpp>

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
                    exprs::get<I>(access::proto_args(e))
                )

                template<std::size_t I, typename ExprDesc, typename Domain>
                inline constexpr auto child(basic_expr<ExprDesc, Domain> const &e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<I>(access::proto_args(e))
                )

                template<std::size_t I, typename ExprDesc, typename Domain>
                inline constexpr auto child(basic_expr<ExprDesc, Domain> &&e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<I>(access::proto_args(static_cast<basic_expr<ExprDesc, Domain> &&>(e)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // get when 0th element is "virtual" (see virtual_member)
                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L & child(virtual_member<L, R, Domain> &a) noexcept
                {
                    return *(L *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a));
                }

                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L const & child(virtual_member<L, R, Domain> const &a) noexcept
                {
                    return *(L const *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a));
                }

                template<std::size_t I, typename L, typename R, typename Domain, BOOST_PROTO_ENABLE_IF(I == 0)>
                inline constexpr L && child(virtual_member<L, R, Domain> &&a) noexcept
                {
                    return static_cast<L &&>(*(L*)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)));
                }

                ////////////////////////////////////////////////////////////////////////////////////
                // left
                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(access::proto_args(e))
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> const & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(access::proto_args(e))
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto left(basic_expr<Tag(L, R), Domain> && e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<0>(access::proto_args(static_cast<basic_expr<Tag(L, R), Domain> &&>(e)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // left when left element is "virtual" (see virtual_member)
                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L & left(virtual_member<L, R, Domain> &a) noexcept
                {
                    return *(L *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a));
                }

                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L const & left(virtual_member<L, R, Domain> const &a) noexcept
                {
                    return *(L const *)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a));
                }

                template<std::size_t I, typename L, typename R, typename Domain>
                inline constexpr L && left(virtual_member<L, R, Domain> &&a) noexcept
                {
                    return static_cast<L &&>(*(L*)(((char *)&a) -
                        ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)));
                }

                ////////////////////////////////////////////////////////////////////////////////////
                // right
                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(access::proto_args(e))
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> const & e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(access::proto_args(e))
                )

                template<typename Tag, typename L, typename R, typename Domain>
                inline constexpr auto right(basic_expr<Tag(L, R), Domain> && e)
                BOOST_PROTO_AUTO_RETURN(
                    exprs::get<1>(access::proto_args(static_cast<basic_expr<Tag(L, R), Domain> &&>(e)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // value
                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> &that)
                BOOST_PROTO_AUTO_RETURN(
                    (access::proto_args(that).proto_child0) // extra parens are significant!
                )

                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> const &that)
                BOOST_PROTO_AUTO_RETURN(
                    (access::proto_args(that).proto_child0) // extra parens are significant!
                )

                template<typename ExprDesc, typename Domain>
                inline constexpr auto value(basic_expr<ExprDesc, Domain> &&that)
                BOOST_PROTO_AUTO_RETURN(
                    (access::proto_args(static_cast<basic_expr<ExprDesc, Domain> &&>(that)).proto_child0) // extra parens are significant!
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // tag_of
                template<typename Tag, typename ...Children, typename Domain>
                inline constexpr Tag &tag_of(basic_expr<Tag(Children...), Domain> &that) noexcept
                {
                    return access::proto_tag(that);
                }

                template<typename Tag, typename ...Children, typename Domain>
                inline constexpr Tag const &tag_of(basic_expr<Tag(Children...), Domain> const &that) noexcept
                {
                    return access::proto_tag(that);
                }

                template<typename Tag, typename ...Children, typename Domain>
                inline constexpr Tag && tag_of(basic_expr<Tag(Children...), Domain> &&that) noexcept
                {
                    return access::proto_tag(static_cast<basic_expr<Tag(Children...), Domain> &&>(that));
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
