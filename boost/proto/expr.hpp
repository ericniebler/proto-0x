///////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_EXPR_HPP_INCLUDED
#define BOOST_PROTO_EXPR_HPP_INCLUDED

#include <boost/mpl/bool.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/args.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_expr
        template<typename T, typename EnableIf = void>
        struct is_expr
          : std::false_type
        {};

        template<typename T>
        struct is_expr<T, typename T::proto_expr_>
          : std::true_type
        {};

        template<typename T>
        struct is_expr<T &, typename T::proto_expr_>
          : std::true_type
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_expr
        template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
        inline constexpr expr<tag::terminal, term<T>> as_expr(T &&t)
        {
            return expr<tag::terminal, term<T>>(static_cast<T &&>(t));
        }

        template<typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
        inline constexpr T && as_expr(T &&t) noexcept
        {
            return static_cast<T &&>(t);
        }

        template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
        inline auto as_expr_d(T &&t)
        BOOST_PROTO_RETURN(
            typename Domain::proto_generator()(expr<tag::terminal, term<T>, Domain>(static_cast<T &&>(t)))
        )

        template<typename Domain, typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
        inline constexpr T && as_expr_d(T &&t) noexcept
        {
            return static_cast<T &&>(t);
        }

        template<typename Tag, typename ...T>
        inline constexpr auto make_expr(T &&...t)
        BOOST_PROTO_RETURN(
            expr<Tag, args<T...>>(static_cast<T &&>(t)...)
        )

        template<typename Tag, typename Domain, typename ...T>
        inline auto make_expr_d(T &&...t)
        BOOST_PROTO_RETURN(
            typename Domain::proto_generator()(expr<Tag, args<T...>, Domain>(static_cast<T &&>(t)...))
        )

        namespace detail
        {
            template<typename Domain, typename Expr, typename ...T>
            inline auto downcast(Expr && e, T &&...)
            BOOST_PROTO_RETURN(
                typename Domain::proto_generator::proto_downcaster()(static_cast<Expr &&>(e))
            )
        }

        namespace exprns
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr
            template<typename Tag, typename Args, typename Domain>
            struct expr
              : Args
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr);

                typedef void proto_expr_;
                typedef Tag proto_tag;
                typedef Args proto_args;
                typedef Domain proto_domain;

                template<typename A, typename ...B
                  , BOOST_PROTO_ENABLE_IF(sizeof...(B) == (Args::proto_arity::value > 0 ? Args::proto_arity::value - 1 : 0))
                  , BOOST_PROTO_ENABLE_IF(!std::is_same<expr &, A>::value || sizeof...(B) > 0)
                >
                explicit constexpr expr(A &&a, B &&... b)
                  : Args(static_cast<A &&>(a), static_cast<B &&>(b)...)
                {}

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u)
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::assign, Domain>(proto::detail::downcast<Domain>(*this, u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) const
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::assign, Domain>(proto::detail::downcast<Domain>(*this, u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &&
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::assign, Domain>(proto::detail::downcast<Domain>(static_cast<expr &&>(*this), u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U>
                void operator=(U && u) const && = delete;

                ////////////////////////////////////////////////////////////////////////////////////
                // operator[]
                template<typename U>
                auto operator[](U && u)
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::subscript, Domain>(proto::detail::downcast<Domain>(*this, u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U>
                auto operator[](U && u) const
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::subscript, Domain>(proto::detail::downcast<Domain>(*this, u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U>
                auto operator[](U && u) &&
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::subscript, Domain>(proto::detail::downcast<Domain>(static_cast<expr &&>(*this), u), proto::as_expr_d<Domain>(static_cast<U &&>(u)))
                )

                template<typename U>
                void operator[](U && u) const && = delete;

                ////////////////////////////////////////////////////////////////////////////////////
                // operator()
                template<typename ...U>
                auto operator()(U &&...u)
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::function, Domain>(proto::detail::downcast<Domain>(*this, u...), proto::as_expr_d<Domain>(static_cast<U &&>(u))...)
                )

                template<typename ...U>
                auto operator()(U &&...u) const
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::function, Domain>(proto::detail::downcast<Domain>(*this, u...), proto::as_expr_d<Domain>(static_cast<U &&>(u))...)
                )

                template<typename ...U>
                auto operator()(U &&...u) &&
                BOOST_PROTO_RETURN(
                    proto::make_expr_d<tag::function, Domain>(proto::detail::downcast<Domain>(static_cast<expr &&>(*this), u...), proto::as_expr_d<Domain>(static_cast<U &&>(u))...)
                )

                template<typename ...U>
                void operator()(U &&...u) const && = delete;
            };
        }
    }
}

#endif
