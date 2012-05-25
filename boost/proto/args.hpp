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
        namespace exprns
        {
            namespace detail
            {
                ///////////////////////////////////////////////////////////////////////////
                // unrefwrap
                template<typename T>
                struct unrefwrap
                {
                    typedef T type;
                };

                template<typename T>
                struct unrefwrap<std::reference_wrapper<T> >
                {
                    typedef T &type;
                };

                ///////////////////////////////////////////////////////////////////////////
                // as_arg
                template<typename T>
                using as_arg = typename unrefwrap<typename std::decay<T>::type>::type;

                #define BOOST_PP_LOCAL_MACRO(N)                                                 \
                template<typename Args>                                                         \
                inline constexpr auto child_impl(Args &&that, std::integral_constant<std::size_t, N>)\
                BOOST_PROTO_RETURN(                                                             \
                    /*extra parens are significant!*/                                           \
                    (static_cast<Args &&>(that).BOOST_PP_CAT(proto_child, N))                   \
                )                                                                               \
                /**/

                #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(BOOST_PROTO_ARGS_UNROLL_MAX))
                #include BOOST_PP_LOCAL_ITERATE()

                template<typename Args, std::size_t I>
                inline constexpr auto child_impl(Args &&that, std::integral_constant<std::size_t, I>)
                BOOST_PROTO_RETURN(
                    detail::child_impl(
                        static_cast<Args &&>(that).proto_args_tail
                      , std::integral_constant<std::size_t, I-BOOST_PROTO_ARGS_UNROLL_MAX>()
                    )
                )
            }

            #define DISABLE_COPY_IF(CLASS, N, T)                                                    \
                BOOST_PP_COMMA_IF(BOOST_PP_EQUAL(N, 1))                                             \
                BOOST_PP_EXPR_IF(                                                                   \
                    BOOST_PP_EQUAL(N, 1)                                                            \
                  , BOOST_PROTO_ENABLE_IF(!std::is_same<T, CLASS &>::value)                         \
                )                                                                                   \
                /**/

            template<typename T>
            struct term
            {
                BOOST_PROTO_CLASS_DEFAULTS(term);

                typedef std::integral_constant<std::size_t, 0> proto_arity;
                typedef T proto_value_type;

                template<typename U DISABLE_COPY_IF(term, 1, U)>
                explicit constexpr term(U &&u)
                  : proto_value(static_cast<U &&>(u)) // std::forward is NOT constexpr!
                {}

                T proto_value;
            };

            template<>
            struct args<>
            {
                typedef std::integral_constant<std::size_t, 0> proto_arity;
            };

            #define INIT(Z, N, D) proto_child ## N(static_cast< U ## N && >( u ## N ))
            #define MEMBERS(Z, N, D) typedef T ## N proto_child_type ## N; T ## N proto_child ## N;

            #define BOOST_PP_LOCAL_MACRO(N)                                                         \
            template<BOOST_PP_ENUM_PARAMS(N, typename T)>                                           \
            struct args<BOOST_PP_ENUM_PARAMS(N, T)>                                                 \
            {                                                                                       \
                BOOST_PROTO_CLASS_DEFAULTS(args);                                                   \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename U) DISABLE_COPY_IF(args, N, U0)>          \
                explicit constexpr args(BOOST_PP_ENUM_BINARY_PARAMS(N, U, &&u))                     \
                  : BOOST_PP_ENUM(N, INIT, ~)                                                       \
                {}                                                                                  \
                                                                                                    \
                typedef std::integral_constant<std::size_t, N> proto_arity;                         \
                BOOST_PP_REPEAT(N, MEMBERS, ~)                                                      \
            };                                                                                      \
            /**/

            #define BOOST_PP_LOCAL_LIMITS (1, BOOST_PROTO_ARGS_UNROLL_MAX)
            #include BOOST_PP_LOCAL_ITERATE()

            // A args type that can be statically initialized
            template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename T), typename ...Tail>
            struct args<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, T), Tail...>
            {
                BOOST_PROTO_CLASS_DEFAULTS(args);

                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest
                  , BOOST_PROTO_ENABLE_IF(sizeof...(Rest) == sizeof...(Tail))
                    DISABLE_COPY_IF(args, BOOST_PROTO_ARGS_UNROLL_MAX, U0)
                >
                explicit constexpr args(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U, &&u), Rest &&...rest)
                  : BOOST_PP_ENUM(BOOST_PROTO_ARGS_UNROLL_MAX, INIT, ~)
                  , proto_args_tail(static_cast<Rest &&>(rest)...) // std::forward is NOT constexpr!
                {}

                typedef std::integral_constant<std::size_t, BOOST_PROTO_ARGS_UNROLL_MAX + sizeof...(Tail)> proto_arity;
                BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, MEMBERS, ~)
                args<Tail...> proto_args_tail;
            };

            #undef INIT
            #undef MEMBERS
            #undef DISABLE_COPY_IF

            ///////////////////////////////////////////////////////////////////////////////
            // child
            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &a)
            BOOST_PROTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> const &a)
            BOOST_PROTO_RETURN(
                detail::child_impl(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &&a)
            BOOST_PROTO_RETURN(
                detail::child_impl(static_cast<args<T...> &&>(a), std::integral_constant<std::size_t, I>())
            )

            template<typename T>
            inline constexpr auto value(term<T> &that)
            BOOST_PROTO_RETURN(
                (that.proto_value) // extra parens are significant!
            )

            template<typename T>
            inline constexpr auto value(term<T> const &that)
            BOOST_PROTO_RETURN(
                (that.proto_value) // extra parens are significant!
            )

            template<typename T>
            inline constexpr auto value(term<T> &&that)
            BOOST_PROTO_RETURN(
                (static_cast<term<T> &&>(that).proto_value)  // extra parens are significant!
            )

            ///////////////////////////////////////////////////////////////////////////////
            // make_args
            template<typename ...T>
            inline args<detail::as_arg<T>...> make_args(T &&...t)
            {
                return args<detail::as_arg<T>...>{t...};
            }
        }

        using exprns::child;
        using exprns::value;
    }
}

#endif
