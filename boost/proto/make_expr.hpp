///////////////////////////////////////////////////////////////////////////////
// make_expr.hpp
// Definitions for make_expr and as_expr.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MAKE_EXPR_HPP_INCLUDED
#define BOOST_PROTO_MAKE_EXPR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/children.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/detail/common_domain.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // get_domain
            template<typename T>
            typename T::proto_domain_type get_domain(int);

            template<typename T>
            default_domain get_domain(long);

            ////////////////////////////////////////////////////////////////////////////////////////
            // get_domain
            template<typename Domain, typename ...T>
            struct get_common_domain_impl
            {
                typedef Domain type;
            };

            template<typename ...T>
            struct get_common_domain_impl<deduce_domain, T...>
              : common_domain<decltype(detail::get_domain<T>(1))...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // Misc domain helpers
            template<typename Domain, typename ...T>
            inline constexpr auto get_common_domain(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type()
            )

            template<typename Domain, typename ...T>
            inline constexpr auto value_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::store_value()
            )

            template<typename Domain, typename ...T>
            inline constexpr auto child_storer(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::store_child()
            )

            template<typename Domain, typename ...T>
            inline constexpr auto expr_maker(Domain const &, T const &...)
            BOOST_PROTO_AUTO_RETURN(
                typename get_common_domain_impl<Domain, T...>::type::make_expr_raw()
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // detail::normalize_expr
            template<typename This, typename Value, typename Domain>
            inline auto constexpr normalize_expr(virtual_member<This, Value, Domain> && e)
            BOOST_PROTO_AUTO_RETURN(
                // Must build a real binary expression from virtual members before the left hand
                // side (actually the member's enclosing object) dies and we lose it forever.
                typename Domain::make_expr()(
                    member()
                  , utility::by_val()(
                        proto::child<0>(static_cast<virtual_member<This, Value, Domain> &&>(e))
                    )
                  , utility::by_val()(
                        proto::child<1>(static_cast<virtual_member<This, Value, Domain> &&>(e))
                    )
                )
            )

            template<typename T>
            inline constexpr T && normalize_expr(T && t) noexcept
            {
                return static_cast<T &&>(t);
            }
        }

        namespace domains
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // as_expr (with domain)
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_storer(Domain(), t)(detail::normalize_expr(static_cast<T &&>(t)))
            )

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t)(proto::terminal(), static_cast<T &&>(t))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr (with domain)
            template<typename Domain, typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t...)(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
            )

            namespace functional
            {
                template<typename Domain>
                struct as_expr
                {
                    template<typename T>
                    inline constexpr auto operator()(T &&t) const
                    BOOST_PROTO_AUTO_RETURN(
                        domains::as_expr<Domain>(static_cast<T &&>(t))
                    )
                };

                template<typename Domain>
                struct make_expr
                {
                    template<typename Tag, typename ...T>
                    inline constexpr auto operator()(Tag tag, T &&...t) const
                    BOOST_PROTO_AUTO_RETURN(
                        domains::make_expr<Domain>(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                    )
                };
            }

            namespace result_of
            {
                template<typename Domain, typename T>
                struct as_expr
                {
                    typedef decltype(domains::as_expr<Domain>(std::declval<T>())) type;
                };

                template<typename Domain, typename Tag, typename ...T>
                struct make_expr
                {
                    typedef decltype(domains::make_expr<Domain>(Tag(), std::declval<T>()...)) type;
                };
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr (no domain)
        template<typename T>
        inline constexpr auto as_expr(T &&t)
        BOOST_PROTO_AUTO_RETURN(
            proto::domains::as_expr<default_domain>(static_cast<T &&>(t))
        )

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_expr (no domain)
        template<typename Tag, typename ...T>
        inline constexpr auto make_expr(Tag tag, T &&...t)
        BOOST_PROTO_AUTO_RETURN(
            proto::domains::make_expr<deduce_domain>(
                static_cast<Tag &&>(tag)
              , static_cast<T &&>(t)...
            )
        )

        namespace functional
        {
            struct as_expr
            {
                template<typename T>
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::as_expr(static_cast<T &&>(t))
                )
            };

            struct make_expr
            {
                template<typename Tag, typename ...T>
                inline constexpr auto operator()(Tag tag, T &&...t) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::make_expr(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                )
            };
        }

        namespace result_of
        {
            template<typename T>
            struct as_expr
            {
                typedef decltype(proto::as_expr(std::declval<T>())) type;
            };

            template<typename Tag, typename ...T>
            struct make_expr
            {
                typedef decltype(proto::make_expr(Tag(), std::declval<T>()...)) type;
            };
        }
    }
}

#endif
