////////////////////////////////////////////////////////////////////////////////////////////////////
// unpack_expr.hpp
// Definitions for unpack_expr
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_UNPACK_EXPR_HPP_INCLUDED
#define BOOST_PROTO_CXX11_UNPACK_EXPR_HPP_INCLUDED

#include <functional>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/value_of.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/make_expr.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                struct unroll_fusion_seq_impl_
                {
                    template<typename Fun, typename Cur, typename End, typename ...Ts
                      , BOOST_PROTO_ENABLE_IF((fusion::result_of::equal_to<Cur, End>::value))
                    >
                    constexpr auto operator()(Fun && fun, Cur const &, End const &, Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Fun &&>(fun)(static_cast<Ts &&>(ts)...)
                    )

                    template<typename Fun, typename Cur, typename End, typename ...Ts
                      , typename Impl = unroll_fusion_seq_impl_
                      , BOOST_PROTO_ENABLE_IF(!(fusion::result_of::equal_to<Cur, End>::value))
                    >
                    constexpr auto operator()(Fun && fun, Cur const &c, End const &e, Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        Impl()(
                            static_cast<Fun &&>(fun)
                          , fusion::next(c)
                          , e
                          , static_cast<Ts &&>(ts)...
                          , static_cast<typename fusion::result_of::value_of<Cur>::type>(fusion::deref(c))
                        )
                    )
                };

                template<typename Fun, typename Seq>
                inline constexpr auto unroll_fusion_seq_(Fun && fun, Seq && seq)
                BOOST_PROTO_AUTO_RETURN(
                    unroll_fusion_seq_impl_()(
                        static_cast<Fun &&>(fun)
                      , fusion::begin(static_cast<Seq &&>(seq))
                      , fusion::end(static_cast<Seq &&>(seq))
                    )
                )

                template<typename Domain, typename Tag>
                struct with_deduced_expr_maker_impl_
                {
                    Tag tag_;
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(with_deduced_expr_maker_impl_);

                    constexpr explicit with_deduced_expr_maker_impl_(Tag && t)
                        noexcept(noexcept(Tag(static_cast<Tag &&>(t))))
                      : tag_(static_cast<Tag &&>(t))
                    {}

                    template<typename ...Ts, typename Impl = with_deduced_expr_maker_impl_>
                    constexpr auto operator()(Ts && ...ts) &&
                    BOOST_PROTO_AUTO_RETURN(
                        detail::expr_maker(Domain(), ts...)(
                            const_cast<Impl &&>(*this).tag_
                          , static_cast<Ts &&>(ts)...
                        )
                    )
                };

                template<typename Domain, typename Tag>
                inline constexpr auto with_deduced_expr_maker_(Tag && tag)
                BOOST_PROTO_AUTO_RETURN(
                    with_deduced_expr_maker_impl_<Domain, Tag>(static_cast<Tag &&>(tag))
                )
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // unpack_expr (with domain)
            template<typename Domain, typename Seq, typename Tag>
            inline constexpr auto unpack_expr(Tag && tag, Seq && seq)
            BOOST_PROTO_AUTO_RETURN(
                detail::unroll_fusion_seq_(
                    detail::with_deduced_expr_maker_<Domain>(static_cast<Tag &&>(tag))
                  , static_cast<Seq &&>(seq)
                )
            )

            template<typename Tag, typename Domain, typename Seq>
            inline constexpr auto unpack_expr(Seq && seq)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::unpack_expr<Domain>(
                    Tag()
                  , static_cast<Seq &&>(seq)
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // unpack_expr (no domain)
            template<typename Seq, typename Tag>
            inline constexpr auto unpack_expr(Tag && tag, Seq && seq)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::unpack_expr<deduce_domain>(
                    static_cast<Tag &&>(tag)
                  , static_cast<Seq &&>(seq)
                )
            )

            template<typename Tag, typename Seq>
            inline constexpr auto unpack_expr(Seq && seq)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::unpack_expr<Tag, deduce_domain>(
                    static_cast<Seq &&>(seq)
                )
            )

            namespace functional
            {
                template<typename Tag, typename Domain>
                struct unpack_expr
                {
                    template<typename Seq>
                    constexpr auto operator()(Seq && seq) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::unpack_expr<Tag, Domain>(static_cast<Seq &&>(seq))
                    )
                };

                template<typename Domain>
                struct unpack_expr<Domain, typename std::enable_if<is_domain<Domain>::value>::type>
                {
                    template<typename Seq, typename Tag>
                    constexpr auto operator()(Tag && tag, Seq && seq) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::unpack_expr<Domain>(static_cast<Tag &&>(tag), static_cast<Seq &&>(seq))
                    )
                };

                template<typename Tag>
                struct unpack_expr<Tag, typename std::enable_if<is_tag<Tag>::value>::type>
                {
                    template<typename Seq>
                    constexpr auto operator()(Seq && seq) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::unpack_expr<Tag>(static_cast<Seq &&>(seq))
                    )
                };

                template<>
                struct unpack_expr<void, void>
                {
                    template<typename Seq, typename Tag>
                    constexpr auto operator()(Tag && tag, Seq && seq) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::unpack_expr(static_cast<Tag &&>(tag), static_cast<Seq &&>(seq))
                    )
                };
            }

            namespace result_of
            {
                template<typename Tag, typename Seq, typename Domain>
                struct unpack_expr
                {
                    using type = decltype(proto::v5::unpack_expr<Tag, Domain>(std::declval<Seq>()));
                };
            }
        }
    }
}

#endif
