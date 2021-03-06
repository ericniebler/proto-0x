////////////////////////////////////////////////////////////////////////////////////////////////////
// make_expr.hpp
// Definitions for make_expr and as_expr.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_MAKE_EXPR_HPP_INCLUDED
#define BOOST_PROTO_V5_MAKE_EXPR_HPP_INCLUDED

#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/accessors.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/detail/common_domain.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // get_domain
                template<typename T>
                typename T::proto_domain_type get_domain(T const *);

                template<typename A, typename B>
                typename A::proto_domain_type get_domain(virtual_member<A, B> const *);

                default_domain get_domain(void const *);

                ////////////////////////////////////////////////////////////////////////////////////
                // get_common_domain_impl
                template<typename Domain, typename ...T>
                struct get_common_domain_impl
                {
                    using type = Domain;
                };

                template<typename ...T>
                struct get_common_domain_impl<deduce_domain, T...>
                {
                    using type =
                        typename common_domain<decltype(detail::get_domain((T const *)0))...>::type;
                };

                template<typename ...T>
                struct get_common_domain_impl<domains::safe_domain_adaptor<deduce_domain>, T...>
                {
                    using type =
                        domains::safe_domain_adaptor<
                            typename common_domain<decltype(detail::get_domain((T const *)0))...>::type
                        >;
                };

                template<typename ...T>
                struct get_common_domain_impl<domains::basic_expr_domain_adaptor<deduce_domain>, T...>
                {
                    using type =
                        domains::basic_expr_domain_adaptor<
                            typename common_domain<decltype(detail::get_domain((T const *)0))...>::type
                        >;
                };

                ////////////////////////////////////////////////////////////////////////////////////
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

                ////////////////////////////////////////////////////////////////////////////////////
                // detail::normalize_expr
                template<typename This, typename Value>
                inline constexpr auto normalize_expr(virtual_member<This, Value> && e)
                BOOST_PROTO_AUTO_RETURN(
                    // Must build a real binary expression from virtual members before the left hand
                    // side (actually the member's enclosing object) dies and we lose it forever.
                    typename result_of::domain_of<virtual_member<This, Value>>::type::make_expr()(
                        member()
                      , utility::by_val()(
                            proto::v5::child<0>(static_cast<virtual_member<This, Value> &&>(e))
                        )
                      , utility::by_val()(
                            proto::v5::child<1>(static_cast<virtual_member<This, Value> &&>(e))
                        )
                    )
                )

                template<typename T>
                inline constexpr T && normalize_expr(T && t) noexcept
                {
                    return static_cast<T &&>(t);
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////
            // as_expr (with domain)
            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_storer(Domain(), t)(detail::normalize_expr(static_cast<T &&>(t)))
            )

            template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t)(proto::v5::terminal(), static_cast<T &&>(t))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // as_expr (no domain)
            template<typename T>
            inline constexpr auto as_expr(T &&t)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::as_expr<default_domain>(static_cast<T &&>(t))
            )

            ////////////////////////////////////////////////////////////////////////////////////
            // make_expr (with domain)
            template<typename Domain, typename ...T, typename Tag
              , BOOST_PROTO_ENABLE_IF(is_tag<Tag>::value)>
            inline constexpr auto make_expr(Tag && tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                detail::expr_maker(Domain(), t...)(
                    static_cast<Tag &&>(tag)
                  , static_cast<T &&>(t)...
                )
            )

            template<typename Tag, typename Domain, typename ...T>
            inline constexpr auto make_expr(T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::make_expr<Domain>(Tag(), static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr (no domain)
            template<typename ...T, typename Tag>
            inline constexpr auto make_expr(Tag && tag, T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::make_expr<deduce_domain>(
                    static_cast<Tag &&>(tag)
                  , static_cast<T &&>(t)...
                )
            )

            template<typename Tag, typename ...T>
            inline constexpr auto make_expr(T &&...t)
            BOOST_PROTO_AUTO_RETURN(
                proto::v5::make_expr<Tag, deduce_domain>(
                    static_cast<T &&>(t)...
                )
            )

            namespace functional
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // functional::as_expr
                template<typename Domain>
                struct as_expr
                {
                    /// INTERNAL ONLY
                    using proto_action_type =
                        as_expr<domains::safe_domain_adaptor<Domain>>;

                    template<typename T>
                    inline constexpr auto operator()(T &&t) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::as_expr<Domain>(static_cast<T &&>(t))
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // functional::make_expr (with domain)
                template<typename Tag, typename Domain>
                struct make_expr
                {
                    /// INTERNAL ONLY
                    using proto_action_type =
                        make_expr<Tag, domains::safe_domain_adaptor<Domain>>;

                    template<typename ...T>
                    inline constexpr auto operator()(T &&...t) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::make_expr<Tag, Domain>(static_cast<T &&>(t)...)
                    )
                };

                template<typename Domain>
                struct make_expr<Domain, typename std::enable_if<is_domain<Domain>::value>::type>
                {
                    /// INTERNAL ONLY
                    using proto_action_type =
                        make_expr<domains::safe_domain_adaptor<Domain>>;

                    template<typename ...T, typename Tag>
                    inline constexpr auto operator()(Tag && tag, T &&...t) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::make_expr<Domain>(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // functional::make_expr (no domain)
                template<typename Tag>
                struct make_expr<Tag, typename std::enable_if<is_tag<Tag>::value>::type>
                {
                    /// INTERNAL ONLY
                    using proto_action_type =
                        make_expr<Tag, domains::safe_domain_adaptor<domains::deduce_domain>>;

                    template<typename ...T>
                    inline constexpr auto operator()(T &&...t) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::make_expr<Tag>(static_cast<T &&>(t)...)
                    )
                };

                template<>
                struct make_expr<void, void>
                {
                    /// INTERNAL ONLY
                    using proto_action_type =
                        make_expr<domains::safe_domain_adaptor<domains::deduce_domain>>;

                    template<typename ...T, typename Tag>
                    inline constexpr auto operator()(Tag && tag, T &&...t) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::v5::make_expr(static_cast<Tag &&>(tag), static_cast<T &&>(t)...)
                    )
                };
            }

            namespace result_of
            {
                template<typename T, typename Domain>
                struct as_expr
                {
                    using type = decltype(proto::v5::as_expr<Domain>(std::declval<T>()));
                };

                template<typename Tag, typename ...T, typename Domain>
                struct make_expr<Tag(T...), Domain>
                {
                    using type = decltype(proto::v5::make_expr<Tag, Domain>(std::declval<T>()...));
                };
            }
        }
    }
}

#endif
