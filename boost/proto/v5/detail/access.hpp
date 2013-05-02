////////////////////////////////////////////////////////////////////////////////////////////////////
// access.hpp
//
// Copyright 2013 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_DETAIL_ACCESS_HPP_INCLUDED
#define BOOST_PROTO_V5_DETAIL_ACCESS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace exprs
            {
                struct access
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // basic_expr accessors
                    template<typename Tag, typename ...Children, typename Domain>
                    static Tag & proto_tag(basic_expr<Tag(Children...), Domain> &e) noexcept
                    {
                        return e.tag_and_children_.first();
                    }

                    template<typename Tag, typename ...Children, typename Domain>
                    static constexpr Tag const & proto_tag(basic_expr<Tag(Children...), Domain> const &e) noexcept
                    {
                        return e.tag_and_children_.first();
                    }

                    template<typename Tag, typename ...Children, typename Domain>
                    static constexpr Tag && proto_tag(basic_expr<Tag(Children...), Domain> &&e) noexcept
                    {
                        return static_cast<detail::compressed_pair<Tag, exprs::children<Children...>> &&>(e.tag_and_children_).first();
                    }

                    template<typename Tag, typename ...Children, typename Domain>
                    static children<Children...> & proto_args(basic_expr<Tag(Children...), Domain> &e) noexcept
                    {
                        return e.tag_and_children_.second();
                    }

                    template<typename Tag, typename ...Children, typename Domain>
                    static constexpr children<Children...> const & proto_args(basic_expr<Tag(Children...), Domain> const &e) noexcept
                    {
                        return e.tag_and_children_.second();
                    }

                    template<typename Tag, typename ...Children, typename Domain>
                    static constexpr children<Children...> && proto_args(basic_expr<Tag(Children...), Domain> &&e) noexcept
                    {
                        return static_cast<detail::compressed_pair<Tag, exprs::children<Children...>> &&>(e.tag_and_children_).second();
                    }

                    template<typename Tag, typename ...A, typename Domain, typename ...B>
                    static inline constexpr auto proto_equal_to(
                        basic_expr<Tag(A...), Domain> const &lhs
                      , basic_expr<Tag(B...), Domain> const &rhs)
                    BOOST_PROTO_AUTO_RETURN(
                        access::proto_args(lhs) == access::proto_args(rhs)
                    )
                };
            }
        }
    }
}

#endif
