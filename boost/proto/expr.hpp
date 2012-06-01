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
            struct any { template<typename T> constexpr any(T const &) noexcept; };

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_expr
            template<typename ...T>
            std::true_type is_expr(args<T...> const &);

            std::false_type is_expr(any);

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_terminal
            template<typename Expr>
            typename Expr::proto_is_terminal is_terminal(Expr const &);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_expr
        template<typename T>
        struct is_expr
          : decltype(detail::is_expr(std::declval<T>()))
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_terminal
        template<typename Expr>
        struct is_terminal
          : decltype(detail::is_terminal(std::declval<Expr>()))
        {};

        ////////////////////////////////////////////////////////////////////////////////////
        // constructors TODO: add noexcept clauses
        // This will no longer be needed once clang implements inheriting constructors
        #define BOOST_PROTO_INHERIT_EXPR_CTORS(EXPR, BASE)                                          \
            constexpr EXPR(Tag tag, Args args)                                                      \
              : BASE(static_cast<Tag &&>(tag), static_cast<Args &&>(args))                          \
            {}                                                                                      \
                                                                                                    \
            constexpr EXPR(Args args)                                                               \
              : EXPR(Tag(), static_cast<Args &&>(args))                                             \
            {}                                                                                      \
                                                                                                    \
            template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_size::value == 1 && !std::is_same<EXPR &, A>::value)> \
            constexpr EXPR(Tag tag, A &&a)                                                          \
              : BASE(static_cast<Tag &&>(tag), static_cast<A &&>(a))                                \
            {}                                                                                      \
                                                                                                    \
            template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_size::value == 1 && !std::is_same<EXPR &, A>::value)> \
            explicit constexpr EXPR(A &&a)                                                          \
              : EXPR(Tag(), static_cast<A &&>(a))                                                   \
            {}                                                                                      \
                                                                                                    \
            template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_size::value)> \
            constexpr EXPR(Tag tag, A &&a, B &&b, C &&... c)                                        \
              : BASE(static_cast<Tag &&>(tag), static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...) \
            {}                                                                                      \
                                                                                                    \
            template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_size::value)> \
            constexpr EXPR(A &&a, B &&b, C &&... c)                                                 \
              : EXPR(Tag(), static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)    \
            {}                                                                                      \
            /**/

        ////////////////////////////////////////////////////////////////////////////////////
        // operator=
        #define BOOST_PROTO_DEFINE_EXPR_ASSIGN(EXPR, DOMAIN)                                        \
            template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<EXPR const, typename std::remove_reference<U>::type const>::value)> \
            auto operator=(U && u) &                                                                \
            BOOST_PROTO_RETURN(                                                                     \
                boost::proto::domains::make_expr<DOMAIN>(boost::proto::tag::assign(), *this, static_cast<U &&>(u))  \
            )                                                                                       \
                                                                                                    \
            template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<EXPR const, typename std::remove_reference<U>::type const>::value)> \
            auto operator=(U && u) const &                                                          \
            BOOST_PROTO_RETURN(                                                                     \
                boost::proto::domains::make_expr<DOMAIN>(boost::proto::tag::assign(), *this, static_cast<U &&>(u))  \
            )                                                                                       \
                                                                                                    \
            template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<EXPR const, typename std::remove_reference<U>::type const>::value)> \
            auto operator=(U && u) &&                                                               \
            BOOST_PROTO_RETURN(                                                                     \
                boost::proto::domains::make_expr<DOMAIN>(boost::proto::tag::assign(), static_cast<EXPR &&>(*this), static_cast<U &&>(u)) \
            )                                                                                       \
                                                                                                    \
            template<typename U, BOOST_PROTO_ENABLE_IF(!std::is_same<EXPR const, typename std::remove_reference<U>::type const>::value)> \
            void operator=(U && u) const && = delete;                                               \
            /**/

        namespace exprs
        {
            // TODO: these need noexcept clauses

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_assign
            template<typename Expr, typename Domain>
            struct expr_assign
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_assign);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                BOOST_PROTO_DEFINE_EXPR_ASSIGN(expr_assign, Domain)
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_subscript
            template<typename Expr, typename Domain>
            struct expr_subscript
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_subscript);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator[]
                template<typename U>
                auto operator[](U && u) &
                 -> decltype(boost::proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), std::declval<Expr &>(), static_cast<U &&>(u)))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), static_cast<Expr &>(*this), static_cast<U &&>(u));
                }

                template<typename U>
                auto operator[](U && u) const &
                 -> decltype(proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), std::declval<Expr const &>(), static_cast<U &&>(u)))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), static_cast<Expr const &>(*this), static_cast<U &&>(u));
                }

                template<typename U>
                auto operator[](U && u) &&
                 -> decltype(proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), std::declval<Expr>(), static_cast<U &&>(u)))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::subscript(), static_cast<Expr &&>(static_cast<expr_subscript &&>(*this)), static_cast<U &&>(u));
                }

                template<typename U>
                void operator[](U && u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_function
            template<typename Expr, typename Domain>
            struct expr_function
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_function);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator()
                template<typename ...U>
                auto operator()(U &&... u) &
                 -> decltype(boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), std::declval<Expr &>(), static_cast<U &&>(u)...))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), static_cast<Expr &>(*this), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                auto operator()(U &&... u) const &
                 -> decltype(boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), std::declval<Expr const &>(), static_cast<U &&>(u)...))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), static_cast<Expr const &>(*this), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                auto operator()(U &&... u) &&
                 -> decltype(boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), std::declval<Expr>(), static_cast<U &&>(u)...))
                {
                    return boost::proto::domains::make_expr<Domain>(boost::proto::tag::function(), static_cast<Expr &&>(static_cast<expr_function &&>(*this)), static_cast<U &&>(u)...);
                }

                template<typename ...U>
                void operator()(U &&... u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct basic_expr
            template<typename Tag, typename Args>
            struct basic_expr
              : Tag, Args
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(basic_expr);

                ////////////////////////////////////////////////////////////////////////////////////
                // typedefs
                typedef Tag             proto_tag_type;
                typedef Args            proto_args_type;
                typedef default_domain  proto_domain_type;

                typedef
                    std::integral_constant<
                        std::size_t
                      , Tag::proto_is_terminal::value ? 0 : Args::proto_size::value
                    >
                proto_arity;

                // TODO: add noexcept to ctors

                ////////////////////////////////////////////////////////////////////////////////////
                // constructors
                constexpr basic_expr(Tag tag, Args args)
                  : Tag(static_cast<Tag &&>(tag))
                  , Args(static_cast<Args &&>(args))
                {}

                explicit constexpr basic_expr(Args args)
                  : basic_expr(Tag(), static_cast<Args &&>(args))
                {}

                template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_size::value <= 1 && !std::is_same<basic_expr &, A>::value)>
                constexpr basic_expr(Tag tag, A &&a)
                  : Tag(static_cast<Tag &&>(tag))
                  , Args(static_cast<A &&>(a))
                {}

                template<typename A, BOOST_PROTO_ENABLE_IF(Args::proto_size::value <= 1 && !std::is_same<basic_expr &, A>::value)>
                explicit constexpr basic_expr(A &&a)
                  : basic_expr(Tag(), static_cast<A &&>(a))
                {}

                template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_size::value)>
                constexpr basic_expr(Tag tag, A &&a, B &&b, C &&... c)
                  : Tag(static_cast<Tag &&>(tag))
                  , Args(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                {}

                template<typename A, typename B, typename ...C, BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == Args::proto_size::value)>
                constexpr basic_expr(A &&a, B &&b, C &&... c)
                  : basic_expr(Tag(), static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
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
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr
            template<typename Tag, typename Args>
            struct expr
              : basic_expr<Tag, Args>
              //, expr_assign<expr<Tag, Args>> // clang problems here. investigate and file a bug.
              , expr_subscript<expr<Tag, Args>>
              , expr_function<expr<Tag, Args>>
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr);

                ////////////////////////////////////////////////////////////////////////////////////
                // constructors
                typedef basic_expr<Tag, Args> proto_base_expr;
                BOOST_PROTO_INHERIT_EXPR_CTORS(expr, proto_base_expr)

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                BOOST_PROTO_DEFINE_EXPR_ASSIGN(expr, default_domain)
            };
        }
    }
}

#endif
