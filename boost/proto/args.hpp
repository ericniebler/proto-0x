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
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
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
#include <boost/proto/utility.hpp>
#include <boost/proto/action/base.hpp>

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
            ////////////////////////////////////////////////////////////////////////////////////////
            // as_virtual_member
            template<typename L, typename R, typename D = typename L::proto_domain_type>
            constexpr auto as_virtual_member(args<exprs::virtual_<L>, terminal<R>> const &a)
            BOOST_PROTO_AUTO_RETURN(
                (char *)static_cast<virtual_member<L, R, D> const *>(&a)
            )

            struct child_impl_
            {
                #define BOOST_PP_LOCAL_MACRO(N)                                                     \
                template<typename Args>                                                             \
                static inline constexpr auto child(                                                 \
                    Args &&that                                                                     \
                  , std::integral_constant<std::size_t, N>                                          \
                )                                                                                   \
                BOOST_PROTO_AUTO_RETURN(                                                            \
                    /*extra parens are significant!*/                                               \
                    (static_cast<Args &&>(that).BOOST_PP_CAT(proto_child, N))                       \
                )                                                                                   \
                /**/

                #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(BOOST_PROTO_ARGS_UNROLL_MAX))
                #include BOOST_PP_LOCAL_ITERATE()

                template<typename Args, std::size_t I, typename Impl = child_impl_>
                static inline constexpr auto child(
                    Args &&that
                  , std::integral_constant<std::size_t, I>
                )
                BOOST_PROTO_AUTO_RETURN(
                    Impl::child(
                        static_cast<Args &&>(that).proto_args_tail
                      , std::integral_constant<std::size_t, I - BOOST_PROTO_ARGS_UNROLL_MAX>()
                    )
                )
            };
        }

        namespace exprs
        {
            #define DISABLE_COPY_IF(CLASS, N, T)                                                    \
                BOOST_PP_COMMA_IF(BOOST_PP_EQUAL(N, 1))                                             \
                BOOST_PP_EXPR_IF(                                                                   \
                    BOOST_PP_EQUAL(N, 1)                                                            \
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<CLASS, T>::value))                  \
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
            #define MEMBERS(Z, N, D) typedef T ## N proto_child_type ## N; T ## N proto_child ## N;
            #define NOEXCEPT(Z, N, D) noexcept(T ## N(static_cast< U ## N && >( u ## N ))) &&
            #define EQUAL_TO(Z, N, D) static_cast<bool>(proto_child ## N == that. proto_child ## N) &&
            #define INVOKE(Z, N, D) f(D.proto_child ## N);

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
                    noexcept(BOOST_PP_REPEAT(N, NOEXCEPT, ~) true)                                  \
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
                                                                                                    \
            template<typename Args>                                                                 \
            struct args_element<BOOST_PP_DEC(N), Args>                                              \
            {                                                                                       \
                typedef typename Args::BOOST_PP_CAT(proto_child_type, BOOST_PP_DEC(N)) type;        \
            };                                                                                      \
                                                                                                    \
            template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                               \
            inline F for_each(args<BOOST_PP_ENUM_PARAMS(N, T)> & a, F const & f)                    \
            {                                                                                       \
                BOOST_PP_REPEAT(N, INVOKE, a)                                                       \
                return f;                                                                           \
            }                                                                                       \
                                                                                                    \
            template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                               \
            inline F for_each(args<BOOST_PP_ENUM_PARAMS(N, T)> const & a, F const & f)              \
            {                                                                                       \
                BOOST_PP_REPEAT(N, INVOKE, a)                                                       \
                return f;                                                                           \
            }                                                                                       \
                                                                                                    \
            template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                               \
            inline F for_each(args<BOOST_PP_ENUM_PARAMS(N, T)> && a, F const & f)                   \
            {                                                                                       \
                BOOST_PP_REPEAT(N, INVOKE, (static_cast<args<BOOST_PP_ENUM_PARAMS(N, T)> &&>(a)))   \
                return f;                                                                           \
            }                                                                                       \
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
                typedef args<Tail...> proto_args_tail_type;
                args<Tail...> proto_args_tail;

                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest
                  , BOOST_PROTO_ENABLE_IF(sizeof...(Rest) == sizeof...(Tail))
                    DISABLE_COPY_IF(args, BOOST_PROTO_ARGS_UNROLL_MAX, U0)
                >
                explicit constexpr args(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U, &&u), Rest &&...rest)
                    noexcept(
                        BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, NOEXCEPT, ~)
                        noexcept(args<Tail...>(static_cast<Rest &&>(rest)...))
                    )
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

            ////////////////////////////////////////////////////////////////////////////////////////
            // args specialization for virtual members
            template<typename A, typename B>
            struct args<virtual_<A>, B>
            {
                static_assert(
                    is_terminal<B>::value
                  , "Only empty terminal expressions are allowed as virtual data members"
                );

                static_assert(
                    std::is_empty<typename args_element<0, typename B::proto_args_type>::type>::value
                  , "Only empty terminal expressions are allowed as virtual data members"
                );

                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(args);
                typedef std::integral_constant<std::size_t, 2> proto_size;
                typedef A proto_child_type0;
                typedef B proto_child_type1;
                B proto_child1;

                template<typename T, BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<args, T>::value))>
                explicit constexpr args(T && t) noexcept(noexcept(B(static_cast<T &&>(t))))
                  : proto_child1(static_cast<T &&>(t))
                {}

                template<typename U>
                inline auto operator==(args<virtual_<A>, U> const & that) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<bool>(proto_child1 == that.proto_child1)
                )

                template<typename U>
                inline auto operator!=(args<virtual_<A>, U> const & that) const
                BOOST_PROTO_AUTO_RETURN(
                    !(*this == that)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // args_element
            template<std::size_t N, typename Args>
            struct args_element
              : args_element<N - BOOST_PROTO_ARGS_UNROLL_MAX, typename Args::proto_args_tail_type>
            {};

            template<typename ...T, typename F>
            inline F for_each(args<T...> & a, F const & f)
            {
                BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                return exprs::for_each(a.proto_args_tail, f);
            }

            template<typename ...T, typename F>
            inline F for_each(args<T...> const & a, F const & f)
            {
                BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                return exprs::for_each(a.proto_args_tail, f);
            }

            template<typename ...T, typename F>
            inline F for_each(args<T...> && a, F const & f)
            {
                BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                return exprs::for_each(static_cast<args<T...> &&>(a).proto_args_tail, f);
            }

            #undef INIT
            #undef MEMBERS
            #undef NOEXCEPT
            #undef EQUAL_TO
            #undef INVOKE
            #undef DISABLE_COPY_IF

            ////////////////////////////////////////////////////////////////////////////////////////
            // child
            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, I>())
            )

            template<std::size_t I, typename ...T>
            inline constexpr auto child(args<T...> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(
                    static_cast<args<T...> &&>(a)
                  , std::integral_constant<std::size_t, I>()
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // child when 0th element is "virtual" (see virtual_member)
            template<std::size_t I, typename L, typename R, BOOST_PROTO_ENABLE_IF(I == 0)>
            inline constexpr L & child(args<virtual_<L>, R> &a)
            BOOST_PROTO_RETURN(
                *(L *)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
            )

            template<std::size_t I, typename L, typename R, BOOST_PROTO_ENABLE_IF(I == 0)>
            inline constexpr L const & child(args<virtual_<L>, R> const &a)
            BOOST_PROTO_RETURN(
                *(L const *)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
            )

            template<std::size_t I, typename L, typename R, BOOST_PROTO_ENABLE_IF(I == 0)>
            inline constexpr L && child(args<virtual_<L>, R> &&a)
            BOOST_PROTO_RETURN(
                static_cast<L &&>(*(L*)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // left
            template<typename L, typename R>
            inline constexpr auto left(args<L, R> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, 0>())
            )

            template<typename L, typename R>
            inline constexpr auto left(args<L, R> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, 0>())
            )

            template<typename L, typename R>
            inline constexpr auto left(args<L, R> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(
                    static_cast<args<L, R> &&>(a)
                  , std::integral_constant<std::size_t, 0>()
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // left when 0th element is "virtual" (see virtual_member)
            template<typename L, typename R>
            inline constexpr L & left(args<virtual_<L>, R> &a)
            BOOST_PROTO_RETURN(
                *(L *)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
            )

            template<typename L, typename R>
            inline constexpr L const & left(args<virtual_<L>, R> const &a)
            BOOST_PROTO_RETURN(
                *(L const *)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a))
            )

            template<typename L, typename R>
            inline constexpr L && left(args<virtual_<L>, R> &&a)
            BOOST_PROTO_RETURN(
                static_cast<L &&>(*(L *)(detail::as_virtual_member(a) -
                    ((char *)&((L *)&a)->proto_member_union_start_ - (char *)&a)))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // right
            template<typename L, typename R>
            inline constexpr auto right(args<L, R> &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, 1>())
            )

            template<typename L, typename R>
            inline constexpr auto right(args<L, R> const &a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(a, std::integral_constant<std::size_t, 1>())
            )

            template<typename L, typename R>
            inline constexpr auto right(args<L, R> &&a)
            BOOST_PROTO_AUTO_RETURN(
                detail::child_impl_::child(
                    static_cast<args<L, R> &&>(a)
                  , std::integral_constant<std::size_t, 1>()
                )
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // value
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

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_args
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

        namespace result_of
        {
            template<typename Expr, std::size_t N>
            struct child
            {
                typedef decltype(proto::child<N>(std::declval<Expr>())) type;
            };

            template<typename Expr>
            struct left
            {
                typedef decltype(proto::left(std::declval<Expr>())) type;
            };

            template<typename Expr>
            struct right
            {
                typedef decltype(proto::right(std::declval<Expr>())) type;
            };

            template<typename Expr>
            struct value
            {
                typedef decltype(proto::value(std::declval<Expr>())) type;
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _child
        template<std::size_t N>
        struct _child
          : basic_action<_child<N>>
        {
            template<typename E, typename ...Rest>
            auto operator()(E && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                proto::child<N>(e)
            )
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _value
        struct _value
          : basic_action<_value>
        {
            template<typename E, typename ...Rest>
            auto operator()(E && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                proto::value(e)
            )
        };
    }
}

#endif
