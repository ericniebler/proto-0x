///////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_EXPR_HPP_INCLUDED
#define BOOST_PROTO_EXPR_HPP_INCLUDED

#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/args.hpp>
#include <boost/proto/domain.hpp>
#include <boost/proto/tags.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct any { template<typename T> any(T const &); };

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_expr
            template<typename T>
            std::true_type is_expr(term<T> const &);

            template<typename ...T>
            std::true_type is_expr(args<T...> const &);

            std::false_type is_expr(any);

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_expr
            template<typename Tag, typename ...T>
            inline constexpr auto make_expr(Tag tag, T &&...t)
            BOOST_PROTO_RETURN(
                expr<Tag, args<T...>>(tag, static_cast<T &&>(t)...)
            )
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_expr
        template<typename T>
        struct is_expr
          : decltype(detail::is_expr(std::declval<T>()))
        {};

        // TODO move as_expr and make_expr wholly into the domain.

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr (no domain)
        template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
        inline constexpr auto as_expr(T &&t)
        BOOST_PROTO_RETURN(
            expr<tag::terminal, term<detail::as_arg<T>>>(tag::terminal(), static_cast<T &&>(t))
        )

        template<typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
        inline constexpr T && as_expr(T &&t) noexcept
        {
            return static_cast<T &&>(t);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr (with domain)
        template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
        inline auto as_expr(T &&t)
        BOOST_PROTO_RETURN(
            typename Domain::proto_generator()(expr<tag::terminal, term<detail::as_arg<T>>>(tag::terminal(), static_cast<T &&>(t)))
        )

        template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
        inline constexpr T && as_expr(T &&t) noexcept
        {
            return static_cast<T &&>(t);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_expr (no domain)
        template<typename Tag, typename ...T>
        inline constexpr auto make_expr(Tag tag, T &&...t)
        BOOST_PROTO_RETURN(
            detail::make_expr(tag, proto::as_expr(static_cast<T &&>(t))...)
        )

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_expr (with domain)
        template<typename Domain, typename Tag, typename ...T>
        inline auto make_expr(Tag tag, T &&...t)
        BOOST_PROTO_RETURN(
            typename Domain::proto_generator()(detail::make_expr(tag, proto::as_expr<Domain>(static_cast<T &&>(t))...))
        )

        namespace exprns
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_assign
            template<typename Expr, typename Domain = default_domain>
            struct expr_assign
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_assign);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<Expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &
                 -> decltype(proto::make_expr<Domain>(tag::assign(), std::declval<Expr &>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::assign(), static_cast<Expr &>(*this), static_cast<U &&>(u));
                }

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<Expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) const &
                 -> decltype(proto::make_expr<Domain>(tag::assign(), std::declval<Expr const &>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::assign(), static_cast<Expr const &>(*this), static_cast<U &&>(u));
                }

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<Expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &&
                 -> decltype(proto::make_expr<Domain>(tag::assign(), std::declval<Expr>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::assign(), static_cast<Expr &&>(static_cast<expr_assign &&>(*this)), static_cast<U &&>(u));
                }

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<Expr const, typename std::remove_reference<U>::type const>::value)>
                void operator=(U && u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_subscript
            template<typename Expr, typename Domain = default_domain>
            struct expr_subscript
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_subscript);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator[]
                template<typename U>
                auto operator[](U && u) &
                 -> decltype(proto::make_expr<Domain>(tag::subscript(), std::declval<Expr &>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::subscript(), static_cast<Expr &>(*this), static_cast<U &&>(u));
                }

                template<typename U>
                auto operator[](U && u) const &
                 -> decltype(proto::make_expr<Domain>(tag::subscript(), std::declval<Expr const &>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::subscript(), static_cast<Expr const &>(*this), static_cast<U &&>(u));
                }

                template<typename U>
                auto operator[](U && u) &&
                 -> decltype(proto::make_expr<Domain>(tag::subscript(), std::declval<Expr>(), static_cast<U &&>(u)))
                {
                    return proto::make_expr<Domain>(tag::subscript(), static_cast<Expr &&>(static_cast<expr_subscript &&>(*this)), static_cast<U &&>(u));
                }

                template<typename U>
                void operator[](U && u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_function
            template<typename Expr, typename Domain = default_domain>
            struct expr_function
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_function);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator()
                template<typename ...U>
                auto operator()(U &&... u) &
                 -> decltype(proto::make_expr<Domain>(tag::function(), std::declval<Expr &>(), static_cast<U &&>(u)...))
                {
                    return proto::make_expr<Domain>(tag::function(), static_cast<Expr &>(*this), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                auto operator()(U &&... u) const &
                 -> decltype(proto::make_expr<Domain>(tag::function(), std::declval<Expr const &>(), static_cast<U &&>(u)...))
                {
                    return proto::make_expr<Domain>(tag::function(), static_cast<Expr const &>(*this), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                auto operator()(U &&... u) &&
                 -> decltype(proto::make_expr<Domain>(tag::function(), std::declval<Expr>(), static_cast<U &&>(u)...))
                {
                    return proto::make_expr<Domain>(tag::function(), static_cast<Expr &&>(static_cast<expr_function &&>(*this)), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                void operator()(U &&... u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr
            template<typename Tag, typename Args>
            struct expr
              : Tag
              , Args
              //, expr_assign<expr<Tag, Args>> // clang problems here. investigate and file a bug.
              , expr_subscript<expr<Tag, Args>>
              , expr_function<expr<Tag, Args>>
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr);

                ////////////////////////////////////////////////////////////////////////////////////
                // typedefs
                typedef Tag             proto_tag_type;
                typedef Args            proto_args_type;
                typedef default_domain  proto_domain_type;

                ////////////////////////////////////////////////////////////////////////////////////
                // constructors
                constexpr expr(Tag tag, Args args)
                  : Tag(tag)
                  , Args(static_cast<Args &&>(args))
                {}

                explicit constexpr expr(Args args)
                  : expr(Tag(), static_cast<Args &&>(args))
                {}

                template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_arity::value <= 1 && !std::is_same<expr &, A>::value)>
                constexpr expr(Tag tag, A &&a)
                  : Tag(tag)
                  , Args(static_cast<A &&>(a))
                {}

                template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_arity::value <= 1 && !std::is_same<expr &, A>::value)>
                explicit constexpr expr(A &&a)
                  : expr(Tag(), static_cast<A &&>(a))
                {}

                template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_arity::value)>
                constexpr expr(Tag tag, A &&a, B &&b, C &&... c)
                  : Tag(tag)
                  , Args(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                {}

                template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_arity::value)>
                constexpr expr(A &&a, B &&b, C &&... c)
                  : expr(Tag(), static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                {}

                ////////////////////////////////////////////////////////////////////////////////////
                // accessors
                Tag proto_tag() const
                {
                    return *this;
                }

                Args & proto_args() &
                {
                    return *this;
                }

                Args const & proto_args() const &
                {
                    return *this;
                }

                Args && proto_args() &&
                {
                    return static_cast<Args &&>(*this);
                }

                default_domain proto_domain()
                {
                    return default_domain();
                }

                //using expr_assign<expr>::operator=;

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &
                BOOST_PROTO_RETURN(
                    proto::make_expr(tag::assign(), *this, static_cast<U &&>(u))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) const &
                BOOST_PROTO_RETURN(
                    proto::make_expr(tag::assign(), *this, static_cast<U &&>(u))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &&
                BOOST_PROTO_RETURN(
                    proto::make_expr(tag::assign(), static_cast<expr &&>(*this), static_cast<U &&>(u))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                void operator=(U && u) const && = delete;
            };
        }
    }
}

#endif
