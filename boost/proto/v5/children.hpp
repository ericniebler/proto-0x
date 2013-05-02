////////////////////////////////////////////////////////////////////////////////////////////////////
// children.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_CHILDREN_HPP_INCLUDED
#define BOOST_PROTO_V5_CHILDREN_HPP_INCLUDED

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
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>

// Must be greater than or equal to 1. (1 means don't do loop unrolling.)
#ifndef BOOST_PROTO_ARGS_UNROLL_MAX
#define BOOST_PROTO_ARGS_UNROLL_MAX 10
#endif

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                struct child_impl_
                {
                    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
                    template<typename Children>                                                     \
                    static inline constexpr auto child(                                             \
                        Children &&that                                                             \
                      , std::integral_constant<std::size_t, N>                                      \
                    )                                                                               \
                    BOOST_PROTO_AUTO_RETURN(                                                        \
                        /*extra parens are significant!*/                                           \
                        (static_cast<Children &&>(that).BOOST_PP_CAT(proto_child, N))               \
                    )                                                                               \
                    /**/

                    #define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(BOOST_PROTO_ARGS_UNROLL_MAX))
                    #include BOOST_PP_LOCAL_ITERATE()

                    template<typename Children, std::size_t I, typename Impl = child_impl_>
                    static inline constexpr auto child(
                        Children &&that
                      , std::integral_constant<std::size_t, I>
                    )
                    BOOST_PROTO_AUTO_RETURN(
                        Impl::child(
                            static_cast<Children &&>(that).proto_children_tail
                          , std::integral_constant<std::size_t, I - BOOST_PROTO_ARGS_UNROLL_MAX>()
                        )
                    )
                };
            }

            namespace exprs
            {
                #define DISABLE_COPY_IF(CLASS, N, T)                                                \
                    BOOST_PP_COMMA_IF(BOOST_PP_EQUAL(N, 1))                                         \
                    BOOST_PP_EXPR_IF(                                                               \
                        BOOST_PP_EQUAL(N, 1)                                                        \
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<CLASS, T>::value))              \
                    )                                                                               \
                    /**/

                template<>
                struct children<>
                {
                    inline constexpr bool operator==(children const &) const
                    {
                        return true;
                    }

                    inline constexpr bool operator!=(children const &) const
                    {
                        return false;
                    }

                    using proto_size = std::integral_constant<std::size_t, 0>;
                };

                #define INIT(Z, N, D) proto_child ## N(static_cast< U ## N && >( u ## N ))
                #define MEMBERS(Z, N, D) using proto_child_type ## N = T ## N; T ## N proto_child ## N;
                #define NOEXCEPT(Z, N, D) noexcept(T ## N(static_cast< U ## N && >( u ## N ))) &&
                #define EQUAL_TO(Z, N, D) static_cast<bool>(proto_child ## N == that. proto_child ## N) &&
                #define INVOKE(Z, N, D) f(D.proto_child ## N);

                #define BOOST_PP_LOCAL_MACRO(N)                                                     \
                template<BOOST_PP_ENUM_PARAMS(N, typename T)>                                       \
                struct children<BOOST_PP_ENUM_PARAMS(N, T)>                                         \
                {                                                                                   \
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(children);                                    \
                    using proto_size = std::integral_constant<std::size_t, N>;                      \
                    BOOST_PP_REPEAT(N, MEMBERS, ~)                                                  \
                                                                                                    \
                    template<BOOST_PP_ENUM_PARAMS(N, typename U) DISABLE_COPY_IF(children, N, U0)>  \
                    explicit constexpr children(BOOST_PP_ENUM_BINARY_PARAMS(N, U, &&u))             \
                        noexcept(BOOST_PP_REPEAT(N, NOEXCEPT, ~) true)                              \
                      : BOOST_PP_ENUM(N, INIT, ~)                                                   \
                    {}                                                                              \
                                                                                                    \
                    template<BOOST_PP_ENUM_PARAMS(N, typename U)>                                   \
                    inline auto operator==(children<BOOST_PP_ENUM_PARAMS(N, U)> const &that) const  \
                    BOOST_PROTO_AUTO_RETURN(                                                        \
                        BOOST_PP_REPEAT(N, EQUAL_TO, ~) true                                        \
                    )                                                                               \
                                                                                                    \
                    template<BOOST_PP_ENUM_PARAMS(N, typename U)>                                   \
                    inline auto operator!=(children<BOOST_PP_ENUM_PARAMS(N, U)> const &that) const  \
                    BOOST_PROTO_AUTO_RETURN(                                                        \
                        !(*this == that)                                                            \
                    )                                                                               \
                };                                                                                  \
                                                                                                    \
                template<typename Children>                                                         \
                struct children_element<BOOST_PP_DEC(N), Children>                                  \
                {                                                                                   \
                    using type = typename Children::BOOST_PP_CAT(proto_child_type, BOOST_PP_DEC(N)); \
                };                                                                                  \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                           \
                inline F for_each(children<BOOST_PP_ENUM_PARAMS(N, T)> & a, F && f)                 \
                {                                                                                   \
                    BOOST_PP_REPEAT(N, INVOKE, a)                                                   \
                    return static_cast<F &&>(f);                                                    \
                }                                                                                   \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                           \
                inline F for_each(children<BOOST_PP_ENUM_PARAMS(N, T)> const & a, F && f)           \
                {                                                                                   \
                    BOOST_PP_REPEAT(N, INVOKE, a)                                                   \
                    return static_cast<F &&>(f);                                                    \
                }                                                                                   \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F>                           \
                inline F for_each(children<BOOST_PP_ENUM_PARAMS(N, T)> && a, F && f)                \
                {                                                                                   \
                    BOOST_PP_REPEAT(N, INVOKE, (static_cast<children<BOOST_PP_ENUM_PARAMS(N, T)> &&>(a))) \
                    return static_cast<F &&>(f);                                                    \
                }                                                                                   \
                /**/

                #define BOOST_PP_LOCAL_LIMITS (1, BOOST_PROTO_ARGS_UNROLL_MAX)
                #include BOOST_PP_LOCAL_ITERATE()

                // A children type that can be statically initialized
                template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename T), typename ...Tail>
                struct children<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, T), Tail...>
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(children);
                    using proto_size = std::integral_constant<std::size_t, BOOST_PROTO_ARGS_UNROLL_MAX + sizeof...(Tail)>;
                    BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, MEMBERS, ~)
                    using proto_children_tail_type = children<Tail...>;
                    children<Tail...> proto_children_tail;

                    template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest
                      , BOOST_PROTO_ENABLE_IF(sizeof...(Rest) == sizeof...(Tail))
                        DISABLE_COPY_IF(children, BOOST_PROTO_ARGS_UNROLL_MAX, U0)
                    >
                    explicit constexpr children(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U, &&u), Rest &&...rest)
                        noexcept(
                            BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, NOEXCEPT, ~)
                            noexcept(children<Tail...>(static_cast<Rest &&>(rest)...))
                        )
                      : BOOST_PP_ENUM(BOOST_PROTO_ARGS_UNROLL_MAX, INIT, ~)
                      , proto_children_tail(static_cast<Rest &&>(rest)...) // std::forward is NOT constexpr!
                    {}

                    template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest>
                    inline auto operator==(children<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U), Rest...> const &that) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, EQUAL_TO, ~) proto_children_tail == that.proto_children_tail
                    )

                    template<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, typename U), typename ...Rest>
                    inline auto operator!=(children<BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, U), Rest...> const &that) const
                    BOOST_PROTO_AUTO_RETURN(
                        !(*this == that)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // children specialization for virtual members
                template<typename A, typename B>
                struct children<virtual_<A>, B>
                {
                    static_assert(
                        is_terminal<B>::value
                      , "Only empty terminal expressions are allowed as virtual data members"
                    );

                    static_assert(
                        std::is_empty<typename children_element<0, typename B::proto_children_type>::type>::value
                      , "Only empty terminal expressions are allowed as virtual data members"
                    );

                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(children);
                    using proto_size = std::integral_constant<std::size_t, 2>;
                    using proto_child_type0 = A;
                    using proto_child_type1 = B;
                    B proto_child1;

                    template<typename T, BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<children, T>::value))>
                    explicit constexpr children(T && t) noexcept(noexcept(B(static_cast<T &&>(t))))
                      : proto_child1(static_cast<T &&>(t))
                    {}

                    template<typename U>
                    inline auto operator==(children<virtual_<A>, U> const & that) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<bool>(proto_child1 == that.proto_child1)
                    )

                    template<typename U>
                    inline auto operator!=(children<virtual_<A>, U> const & that) const
                    BOOST_PROTO_AUTO_RETURN(
                        !(*this == that)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // children_element
                template<std::size_t N, typename Children>
                struct children_element
                  : children_element<N - BOOST_PROTO_ARGS_UNROLL_MAX, typename Children::proto_children_tail_type>
                {};

                template<typename ...T, typename F>
                inline F for_each(children<T...> & a, F && f)
                {
                    BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                    return exprs::for_each(a.proto_children_tail, static_cast<F &&>(f));
                }

                template<typename ...T, typename F>
                inline F for_each(children<T...> const & a, F && f)
                {
                    BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                    return exprs::for_each(a.proto_children_tail, static_cast<F &&>(f));
                }

                template<typename ...T, typename F>
                inline F for_each(children<T...> && a, F && f)
                {
                    BOOST_PP_REPEAT(BOOST_PROTO_ARGS_UNROLL_MAX, INVOKE, a)
                    return exprs::for_each(
                        static_cast<children<T...> &&>(a).proto_children_tail
                      , static_cast<F &&>(f)
                    );
                }

                #undef INIT
                #undef MEMBERS
                #undef NOEXCEPT
                #undef EQUAL_TO
                #undef INVOKE
                #undef DISABLE_COPY_IF

                ////////////////////////////////////////////////////////////////////////////////////
                // get
                template<std::size_t I, typename ...T>
                inline constexpr auto get(children<T...> &a)
                BOOST_PROTO_AUTO_RETURN(
                    detail::child_impl_::child(a, std::integral_constant<std::size_t, I>())
                )

                template<std::size_t I, typename ...T>
                inline constexpr auto get(children<T...> const &a)
                BOOST_PROTO_AUTO_RETURN(
                    detail::child_impl_::child(a, std::integral_constant<std::size_t, I>())
                )

                template<std::size_t I, typename ...T>
                inline constexpr auto get(children<T...> &&a)
                BOOST_PROTO_AUTO_RETURN(
                    detail::child_impl_::child(
                        static_cast<children<T...> &&>(a)
                      , std::integral_constant<std::size_t, I>()
                    )
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // make_args
                template<typename ...T>
                inline constexpr auto make_args(T &&... t)
                BOOST_PROTO_AUTO_RETURN(
                    children<T...>(static_cast<T &&>(t)...)
                )
            }
        }
    }
}

#endif
