///////////////////////////////////////////////////////////////////////////////
// args.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_PROTO_ARGS_HPP_INCLUDED
#define BOOST_PROTO_ARGS_HPP_INCLUDED

#include <utility>
#include <functional>
#include <type_traits>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/proto/proto_fwd.hpp>

// Must be greater than or equal to 1. (1 means don't do loop unrolling.)
#ifndef BOOST_PROTO_ARGS_UNROLL_MAX
#define BOOST_PROTO_ARGS_UNROLL_MAX 10
#endif

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            #define BOOST_PP_LOCAL_MACRO(N)                                                         \
            template<typename Args>                                                                 \
            inline constexpr auto child_impl(Args &&that, std::integral_constant<std::size_t, N>)   \
            BOOST_PROTO_AUTO_RETURN(                                                                \
                /*extra parens are significant!*/                                                   \
                (static_cast<Args &&>(that).BOOST_PP_CAT(proto_child, N))                           \
            )                                                                                       \
            /**/

            #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(BOOST_PROTO_ARGS_UNROLL_MAX))
            #include BOOST_PP_LOCAL_ITERATE()

            template<typename Args, std::size_t I>
            inline constexpr auto child_impl(Args &&that, std::integral_constant<std::size_t, I>)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(
                    static_cast<Args &&>(that).proto_args_tail
                  , std::integral_constant<std::size_t, I-BOOST_PROTO_ARGS_UNROLL_MAX>()
                )
            )
        }

        namespace exprs
        {
            #define DISABLE_COPY_IF(CLASS, N, T)                                                    \
                BOOST_PP_COMMA_IF(BOOST_PP_EQUAL(N, 1))                                             \
                BOOST_PP_EXPR_IF(                                                                   \
                    BOOST_PP_EQUAL(N, 1)                                                            \
                  , BOOST_PROTO_ENABLE_IF(!std::is_same<T, CLASS &>::value)                         \
                )                                                                                   \
                /**/

            template<>
            struct args<>
            {
                inline constexpr bool operator==(args const &) const
                {
                    return true;
                }

                inline constexpr bool operator!=(args const &) const
                {
                    return false;
                }

                typedef std::integral_constant<std::size_t, 0> proto_size;
            };

            #define INIT(Z, N, D) proto_child ## N(static_cast< U ## N && >( u ## N ))
            #define CTORS(Z, N, D) static_cast<void>(T ## N(static_cast< U ## N && >( u ## N )))
            #define MEMBERS(Z, N, D) typedef T ## N proto_child_type ## N; T ## N proto_child ## N;
            #define EQUAL_TO(Z, N, D) static_cast<bool>(proto_child ## N == that. proto_child ## N) &&

            #define BOOST_PP_LOCAL_MACRO(N)                                                         \
            template<BOOST_PP_ENUM_PARAMS(N, typename T)>                                           \
            struct args<BOOST_PP_ENUM_PARAMS(N, T)>                                                 \
            {                                                                                       \
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(args);                                            \
                typedef std::integral_constant<std::size_t, N> proto_size;                          \
                BOOST_PP_REPEAT(N, MEMBERS, ~)                                                      \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename U) DISABLE_COPY_IF(args, N, U0)>          \
                explicit constexpr args(BOOST_PP_ENUM_BINARY_PARAMS(N, U, &&u))                     \
                    noexcept(noexcept(BOOST_PP_ENUM(N, CTORS, ~)))                                  \
                  : BOOST_PP_ENUM(N, INIT, ~)                                                       \
                {}                                                                                  \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename U)>                                       \
                inline auto operator==(args<BOOST_PP_ENUM_PARAMS(N, U)> const &that) const          \
                BOOST_PROTO_AUTO_RETURN(                                                            \
                    BOOST_PP_REPEAT(N, EQUAL_TO, ~) true                                            \
                )                                                                                   \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename U)>                                       \
                inline auto operator!=(args<BOOST_PP_ENUM_PARAMS(N, U)> const &that) const          \
                BOOST_PROTO_AUTO_RETURN(                                                            \
                    !(*this == that)                                                                \
                )                                                                                   \
            };                                                                                      \
            /**/

            #define BOOST_PP_LOCAL_LIMITS (1, BOOST_PROTO_ARGS_UNROLL_MAX)
            #include BOOST_PP_LOCAL_ITERATE()

            // A args type that can be statically initialized
            template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename T), typename ...Tail>
            struct args<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, T), Tail...>
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(args);
                typedef std::integral_constant<std::size_t, BOOST_PROTO_ARGS_UNROLL_MAX + sizeof...(Tail)> proto_size;
                BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, MEMBERS, ~)
                args<Tail...> proto_args_tail;

                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest
                  , BOOST_PROTO_ENABLE_IF(sizeof...(Rest) == sizeof...(Tail))
                    DISABLE_COPY_IF(args, BOOST_PROTO_ARGS_UNROLL_MAX, U0)
                >
                explicit constexpr args(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U, &&u), Rest &&...rest)
                    noexcept(noexcept(
                        BOOST_PP_ENUM(BOOST_PROTO_ARGS_UNROLL_MAX, CTORS, ~)
                      , static_cast<void>(args<Tail...>(static_cast<Rest &&>(rest)...))
                    ))
                  : BOOST_PP_ENUM(BOOST_PROTO_ARGS_UNROLL_MAX, INIT, ~)
                  , proto_args_tail(static_cast<Rest &&>(rest)...) // std::forward is NOT constexpr!
                {}

                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest>
                inline auto operator==(args<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U), Rest...> const &that) const
                BOOST_PROTO_AUTO_RETURN(
                    BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, EQUAL_TO, ~) proto_args_tail == that.proto_args_tail
                )

                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest>
                inline auto operator!=(args<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U), Rest...> const &that) const
                BOOST_PROTO_AUTO_RETURN(
                    !(*this == that)
                )
            };

            #undef INIT
            #undef CTORS
            #undef MEMBERS
            #undef EQUAL_TO
            #undef DISABLE_COPY_IF

            ///////////////////////////////////////////////////////////////////////////////
            // child
            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(static_cast<args<T...> &&>(a), std::integral_constant<std::size_t, I>())
            )

            ///////////////////////////////////////////////////////////////////////////////
            // left
            template<typename L, typename R>
            inline constexpr auto left(args<L, R> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, 0>())
            )

            template<typename L, typename R>
            inline constexpr auto left(args<L, R> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, 0>())
            )

            template<typename L, typename R>
            inline constexpr auto left(args<L, R> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(static_cast<args<L, R> &&>(a), std::integral_constant<std::size_t, 0>())
            )

            ///////////////////////////////////////////////////////////////////////////////
            // right
            template<typename L, typename R>
            inline constexpr auto right(args<L, R> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, 1>())
            )

            template<typename L, typename R>
            inline constexpr auto right(args<L, R> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, 1>())
            )

            template<typename L, typename R>
            inline constexpr auto right(args<L, R> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl(static_cast<args<L, R> &&>(a), std::integral_constant<std::size_t, 1>())
            )

            template<typename T>
            inline constexpr auto value(args<T> &that)
            BOOST_PROTO_AUTO_RETURN(
                (that.proto_child0) // extra parens are significant!
            )

            template<typename T>
            inline constexpr auto value(args<T> const &that)
            BOOST_PROTO_AUTO_RETURN(
                (that.proto_child0) // extra parens are significant!
            )

            template<typename T>
            inline constexpr auto value(args<T> &&that)
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<args<T> &&>(that).proto_child0)  // extra parens are significant!
            )

            template<typename ...T>
            inline constexpr auto make_args(T &&... t)
            BOOST_PROTO_AUTO_RETURN(
                args<T...>(static_cast<T &&>(t)...)
            )
        }

        using exprs::child;
        using exprs::left;
        using exprs::right;
        using exprs::value;
    }
}

#endif
