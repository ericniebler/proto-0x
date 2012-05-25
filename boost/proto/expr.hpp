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
        // as_child
        template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
        expr<tag::terminal, term<T>> as_child(T &&t)
        {
            return expr<tag::terminal, term<T>>(static_cast<T &&>(t));
        }

        template<typename T, BOOST_PROTO_ENABLE_IF(is_expr<T>::value)>
        inline constexpr T && as_child(T &&t) noexcept
        {
            return static_cast<T &&>(t);
        }

        namespace exprns
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr
            template<typename Tag, typename Args>
            struct expr
              : Args
            {
                BOOST_PROTO_CLASS_DEFAULTS(expr);

                typedef void proto_expr_;
                typedef Tag proto_tag;
                typedef Args proto_args;

                template<typename A, typename ...B
                  , BOOST_PROTO_ENABLE_IF(sizeof...(B) == (Args::proto_arity::value > 0 ? Args::proto_arity::value - 1 : 0))
                  , BOOST_PROTO_ENABLE_IF(!std::is_same<expr &, A>::value)
                >
                explicit constexpr expr(A &&a, B &&... b)
                  : Args(static_cast<A &&>(a), static_cast<B &&>(b)...)
                {}

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u)
                BOOST_PROTO_RETURN(
                    expr<tag::assign, args<expr &, decltype((proto::as_child(static_cast<U &&>(u))))>>(*this, proto::as_child(static_cast<U &&>(u)))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) const
                BOOST_PROTO_RETURN(
                    expr<tag::assign, args<expr const &, decltype((proto::as_child(static_cast<U &&>(u))))>>(*this, proto::as_child(static_cast<U &&>(u)))
                )

                template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<expr const, typename std::remove_reference<U>::type const>::value)>
                auto operator=(U && u) &&
                BOOST_PROTO_RETURN(
                    expr<tag::assign, args<expr, decltype((proto::as_child(static_cast<U &&>(u))))>>(static_cast<expr &&>(*this), proto::as_child(static_cast<U &&>(u)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // operator[]
                template<typename U>
                auto operator[](U && u)
                BOOST_PROTO_RETURN(
                    expr<tag::subscript, args<expr &, decltype((proto::as_child(static_cast<U &&>(u))))>>(*this, proto::as_child(static_cast<U &&>(u)))
                )

                template<typename U>
                auto operator[](U && u) const
                BOOST_PROTO_RETURN(
                    expr<tag::subscript, args<expr const &, decltype((proto::as_child(static_cast<U &&>(u))))>>(*this, proto::as_child(static_cast<U &&>(u)))
                )

                template<typename U>
                auto operator[](U && u) &&
                BOOST_PROTO_RETURN(
                    expr<tag::subscript, args<expr, decltype((proto::as_child(static_cast<U &&>(u))))>>(static_cast<expr &&>(*this), proto::as_child(static_cast<U &&>(u)))
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // operator()
                template<typename ...U>
                auto operator()(U &&...u)
                BOOST_PROTO_RETURN(
                    expr<tag::function, args<expr &, decltype((proto::as_child(static_cast<U &&>(u))))...>>(*this, proto::as_child(static_cast<U &&>(u))...)
                )

                template<typename ...U>
                auto operator()(U &&...u) const
                BOOST_PROTO_RETURN(
                    expr<tag::function, args<expr const &, decltype((proto::as_child(static_cast<U &&>(u))))...>>(*this, proto::as_child(static_cast<U &&>(u))...)
                )

                template<typename ...U>
                auto operator()(U &&...u) &&
                BOOST_PROTO_RETURN(
                    expr<tag::function, args<expr, decltype((proto::as_child(static_cast<U &&>(u))))...>>(static_cast<expr &&>(*this), proto::as_child(static_cast<U &&>(u))...)
                )
            };
        }
    }
}

#endif
