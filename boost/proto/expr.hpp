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
#include <boost/mpl/integral_c_fwd.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/args.hpp>
#include <boost/proto/domain.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/pass_through.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // is_terminal_
            template<typename Tag>
            typename Tag::proto_is_terminal is_terminal_(int);

            template<typename Tag>
            std::false_type is_terminal_(long);

            ////////////////////////////////////////////////////////////////////////////////////////
            // are_equality_comparible
            template<typename L, typename R
              , BOOST_PROTO_ENABLE_IF_VALID_EXPR(std::declval<L>().proto_equal_to(std::declval<R>()))>
            std::true_type are_equality_comparible_(int);

            template<typename L, typename R>
            std::false_type are_equality_comparible_(long);

            template<typename L, typename R>
            struct are_equality_comparible
              : decltype(detail::are_equality_comparible_<L, R>(1))
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_boolean_convertible
            //  expr_boolean_convertible and friends are very carefully crafted, along with
            //  proto::args::operator==, to only allow expressions that can be compared to have
            //  a functioning proto_equal_to, and also to allow a==b and a!=b to have implicit
            //  conversions to bool only where it makes sense to allow it.
            ////////////////////////////////////////////////////////////////////////////////////////
            template<typename Expr, typename EnableIf = void>
            struct expr_boolean_convertible
            {};

            // We use the nasty, old smart-bool idiom below instead of an explicit converion
            // to bool because an explicit boolean conversion doesn't satisfy the requirements
            // of the EqualityComparible concept.
            template<typename T, typename U, typename Domain>
            struct expr_boolean_convertible<
                basic_expr<tag::equal_to, args<T, U>, Domain>
              , typename std::enable_if<are_equality_comparible<T, U>::value>::type
            >
            {
            private:
                struct proto_smart_bool { int member; };
                typedef int proto_smart_bool::*unspecified_bool_type;

            public:
                inline operator unspecified_bool_type() const
                BOOST_PROTO_RETURN(
                    proto::child<0>(static_cast<basic_expr<tag::equal_to, args<T, U>, Domain> const &>(*this)).proto_equal_to(
                    proto::child<1>(static_cast<basic_expr<tag::equal_to, args<T, U>, Domain> const &>(*this))) ? &proto_smart_bool::member : 0
                )
            };

            template<typename T, typename U, typename Domain>
            struct expr_boolean_convertible<
                basic_expr<tag::not_equal_to, args<T, U>, Domain>
              , typename std::enable_if<are_equality_comparible<T, U>::value>::type
            >
            {
            private:
                struct proto_smart_bool { int member; };
                typedef int proto_smart_bool::*unspecified_bool_type;

            public:
                inline operator unspecified_bool_type() const
                BOOST_PROTO_RETURN(
                    proto::child<0>(static_cast<basic_expr<tag::not_equal_to, args<T, U>, Domain> const &>(*this)).proto_equal_to(
                    proto::child<1>(static_cast<basic_expr<tag::not_equal_to, args<T, U>, Domain> const &>(*this))) ? 0 : &proto_smart_bool::member
                )
            };

            template<typename T, typename Domain>
            struct expr_boolean_convertible<
                basic_expr<tag::logical_not, args<T>, Domain>
              , typename std::enable_if<std::is_convertible<T, bool>::value>::type
            >
            {
            private:
                struct proto_smart_bool { int member; };
                typedef int proto_smart_bool::*unspecified_bool_type;

            public:
                inline operator unspecified_bool_type() const
                BOOST_PROTO_RETURN(
                    proto::child<0>(static_cast<basic_expr<tag::logical_not, args<T>, Domain> const &>(*this)) ? 0 : &proto_smart_bool::member
                )
            };

            template<typename T, typename U, typename Domain>
            struct expr_boolean_convertible<
                basic_expr<tag::logical_and, args<T, U>, Domain>
              , typename std::enable_if<
                    std::is_convertible<T, bool>::value && std::is_convertible<U, bool>::value
                >::type
            >
            {
            private:
                struct proto_smart_bool { int member; };
                typedef int proto_smart_bool::*unspecified_bool_type;

            public:
                inline operator unspecified_bool_type() const
                BOOST_PROTO_RETURN(
                    static_cast<bool>(proto::child<0>(static_cast<basic_expr<tag::logical_and, args<T, U>, Domain> const &>(*this))) &&
                    static_cast<bool>(proto::child<1>(static_cast<basic_expr<tag::logical_and, args<T, U>, Domain> const &>(*this)))
                    ? &proto_smart_bool::member : 0
                )
            };

            template<typename T, typename U, typename Domain>
            struct expr_boolean_convertible<
                basic_expr<tag::logical_or, args<T, U>, Domain>
              , typename std::enable_if<
                    std::is_convertible<T, bool>::value && std::is_convertible<U, bool>::value
                >::type
            >
            {
            private:
                struct proto_smart_bool { int member; };
                typedef int proto_smart_bool::*unspecified_bool_type;

            public:
                inline operator unspecified_bool_type() const
                BOOST_PROTO_RETURN(
                    static_cast<bool>(proto::child<0>(static_cast<basic_expr<tag::logical_or, args<T, U>, Domain> const &>(*this))) ||
                    static_cast<bool>(proto::child<1>(static_cast<basic_expr<tag::logical_or, args<T, U>, Domain> const &>(*this)))
                    ? &proto_smart_bool::member : 0
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // constructors
        // This will no longer be needed once clang implements inheriting constructors
        #define BOOST_PROTO_INHERIT_EXPR_CTORS(EXPR, BASE)                                          \
            using typename BASE::proto_tag_type;                                                    \
            using typename BASE::proto_args_type;                                                   \
                                                                                                    \
            constexpr EXPR(proto_tag_type tag, proto_args_type args)                                \
                noexcept(noexcept(                                                                  \
                    BASE(static_cast<proto_tag_type &&>(tag), static_cast<proto_args_type &&>(args))\
                ))                                                                                  \
              : BASE(static_cast<proto_tag_type &&>(tag), static_cast<proto_args_type &&>(args))    \
            {}                                                                                      \
                                                                                                    \
            constexpr explicit EXPR(proto_args_type args)                                           \
                noexcept(noexcept(                                                                  \
                    EXPR(proto_tag_type(), static_cast<proto_args_type &&>(args))                   \
                ))                                                                                  \
              : EXPR(proto_tag_type(), static_cast<proto_args_type &&>(args))                       \
            {}                                                                                      \
                                                                                                    \
            template<typename proto_A_                                                              \
              , BOOST_PROTO_ENABLE_IF(                                                              \
                    proto_args_type::proto_size::value == 1 &&                                      \
                    !(boost::proto::utility::is_base_of<EXPR, proto_A_>::value)                     \
                )                                                                                   \
            >                                                                                       \
            constexpr EXPR(proto_tag_type tag, proto_A_ &&a)                                        \
                noexcept(noexcept(                                                                  \
                    BASE(static_cast<proto_tag_type &&>(tag), static_cast<proto_A_ &&>(a))          \
                ))                                                                                  \
              : BASE(static_cast<proto_tag_type &&>(tag), static_cast<proto_A_ &&>(a))              \
            {}                                                                                      \
                                                                                                    \
            template<typename proto_A_                                                              \
              , BOOST_PROTO_ENABLE_IF(                                                              \
                    proto_args_type::proto_size::value == 1 &&                                      \
                    !(boost::proto::utility::is_base_of<EXPR, proto_A_>::value)                     \
                )                                                                                   \
            >                                                                                       \
            explicit constexpr EXPR(proto_A_ &&a)                                                   \
                noexcept(noexcept(                                                                  \
                    EXPR(proto_tag_type(), static_cast<proto_A_ &&>(a))                             \
                ))                                                                                  \
              : EXPR(proto_tag_type(), static_cast<proto_A_ &&>(a))                                 \
            {}                                                                                      \
                                                                                                    \
            template<typename proto_A_, typename proto_B_, typename ...proto_C_                     \
              , BOOST_PROTO_ENABLE_IF(proto_args_type::proto_size::value == sizeof...(proto_C_) + 2) \
            >                                                                                       \
            constexpr EXPR(proto_tag_type tag, proto_A_ &&a, proto_B_ &&b, proto_C_ &&... c)        \
                noexcept(noexcept(                                                                  \
                    BASE(                                                                           \
                        static_cast<proto_tag_type &&>(tag)                                         \
                      , static_cast<proto_A_ &&>(a)                                                 \
                      , static_cast<proto_B_ &&>(b)                                                 \
                      , static_cast<proto_C_ &&>(c)...                                              \
                    )                                                                               \
                ))                                                                                  \
              : BASE(                                                                               \
                    static_cast<proto_tag_type &&>(tag)                                             \
                  , static_cast<proto_A_ &&>(a)                                                     \
                  , static_cast<proto_B_ &&>(b)                                                     \
                  , static_cast<proto_C_ &&>(c)...                                                  \
                )                                                                                   \
            {}                                                                                      \
                                                                                                    \
            template<typename proto_A_, typename proto_B_, typename ...proto_C_                     \
              , BOOST_PROTO_ENABLE_IF(proto_args_type::proto_size::value == sizeof...(proto_C_) + 2)> \
            constexpr EXPR(proto_A_ &&a, proto_B_ &&b, proto_C_ &&... c)                            \
                noexcept(noexcept(                                                                  \
                    EXPR(                                                                           \
                        proto_tag_type()                                                            \
                      , static_cast<proto_A_ &&>(a)                                                 \
                      , static_cast<proto_B_ &&>(b)                                                 \
                      , static_cast<proto_C_ &&>(c)...                                              \
                    )                                                                               \
                ))                                                                                  \
              : EXPR(                                                                               \
                    proto_tag_type()                                                                \
                  , static_cast<proto_A_ &&>(a)                                                     \
                  , static_cast<proto_B_ &&>(b)                                                     \
                  , static_cast<proto_C_ &&>(c)...                                                  \
                )                                                                                   \
            {}                                                                                      \
                                                                                                    \
            typedef int boost_proto_inherited_ctors ## __LINE__                                     \
            /**/

        namespace exprs
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_assign
            template<typename Expr, typename Domain>
            struct expr_assign
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_assign);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                template<typename U, typename E = Expr
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                auto operator=(U && u) &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::assign()
                      , static_cast<E &>(*this)
                      , static_cast<U &&>(u)
                    )
                )

                template<typename U, typename E = Expr
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                auto operator=(U && u) const &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::assign()
                      , static_cast<E const &>(*this)
                      , static_cast<U &&>(u)
                    )
                )

                template<typename U, typename E = Expr
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                auto operator=(U && u) &&
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::assign()
                      , static_cast<E &&>(static_cast<expr_assign &&>(*this))
                      , static_cast<U &&>(u)
                    )
                )

                template<typename U
                  , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                void operator=(U && u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_subscript
            template<typename Expr, typename Domain>
            struct expr_subscript
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_subscript);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator[]
                template<typename U, typename E = Expr>
                auto operator[](U && u) &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::subscript()
                      , static_cast<E &>(*this)
                      , static_cast<U &&>(u)
                    )
                )

                template<typename U, typename E = Expr>
                auto operator[](U && u) const &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::subscript()
                      , static_cast<E const &>(*this)
                      , static_cast<U &&>(u)
                    )
                )

                template<typename U, typename E = Expr>
                auto operator[](U && u) &&
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::subscript()
                      , static_cast<E &&>(static_cast<expr_subscript &&>(*this))
                      , static_cast<U &&>(u)
                    )
                )

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
                template<typename ...U, typename E = Expr>
                auto operator()(U &&... u) &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::function()
                      , static_cast<E &>(*this)
                      , static_cast<U &&>(u)...
                    )
                )

                template<typename ...U, typename E = Expr>
                auto operator()(U &&... u) const &
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::function()
                      , static_cast<E const &>(*this)
                      , static_cast<U &&>(u)...
                    )
                )

                template<typename ...U, typename E = Expr>
                auto operator()(U &&... u) &&
                BOOST_PROTO_AUTO_RETURN(
                    boost::proto::domains::make_expr<Domain>(
                        boost::proto::tag::function()
                      , static_cast<E &&>(static_cast<expr_function &&>(*this))
                      , static_cast<U &&>(u)...
                    )
                )

                template<typename ...U>
                void operator()(U &&... u) const && = delete;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr_base
            struct expr_base
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct basic_expr
            template<typename Tag, typename ...Args, typename Domain>
            struct basic_expr<Tag, args<Args...>, Domain>
              : expr_base
              , Tag
              , args<Args...>
              , detail::expr_boolean_convertible<basic_expr<Tag, args<Args...>, Domain>>
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // Check constraints
                static_assert(
                    !decltype(detail::is_terminal_<Tag>(1))::value || sizeof...(Args) <= 1
                  , "terminals can have only 1 value"
                );

                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(basic_expr);

                ////////////////////////////////////////////////////////////////////////////////////
                // typedefs
                typedef Tag                                 proto_tag_type;
                typedef args<Args...>                       proto_args_type;
                typedef Domain                              proto_domain_type;
                typedef basic_expr                          proto_basic_expr_type;
                //typedef basic_expr                          proto_grammar_type;
                typedef action<pass_through(Args...)>    proto_algorithm_type;
                typedef
                    std::integral_constant<
                        std::size_t
                      , decltype(detail::is_terminal_<Tag>(1))::value ? 0 : sizeof...(Args)
                    >
                proto_arity;

                typedef tag::proto_expr                     fusion_tag; ///< For Fusion

                ////////////////////////////////////////////////////////////////////////////////////
                // constructors
                constexpr basic_expr(Tag tag, proto_args_type args)
                    noexcept(
                        noexcept(Tag(static_cast<Tag &&>(tag))) &&
                        noexcept(proto_args_type(static_cast<proto_args_type &&>(args)))
                    )
                  : Tag(static_cast<Tag &&>(tag))
                  , proto_args_type(static_cast<proto_args_type &&>(args))
                {}

                explicit constexpr basic_expr(proto_args_type args)
                    noexcept(noexcept(basic_expr(Tag(), static_cast<proto_args_type &&>(args))))
                  : basic_expr(Tag(), static_cast<proto_args_type &&>(args))
                {}

                template<typename A
                  , BOOST_PROTO_ENABLE_IF(
                        sizeof...(Args) == 1 && !(utility::is_base_of<basic_expr, A>::value)
                    )
                >
                constexpr basic_expr(Tag tag, A &&a)
                    noexcept(
                        noexcept(Tag(static_cast<Tag &&>(tag))) &&
                        noexcept(proto_args_type(static_cast<A &&>(a)))
                    )
                  : Tag(static_cast<Tag &&>(tag))
                  , proto_args_type(static_cast<A &&>(a))
                {}

                template<
                    typename A
                  , BOOST_PROTO_ENABLE_IF(
                        sizeof...(Args) == 1 && !(utility::is_base_of<basic_expr, A>::value)
                    )
                >
                explicit constexpr basic_expr(A &&a)
                    noexcept(noexcept(
                        basic_expr(Tag(), static_cast<A &&>(a))
                    ))
                  : basic_expr(Tag(), static_cast<A &&>(a))
                {}

                template<typename A, typename B, typename ...C
                  , BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == sizeof...(Args))
                >
                constexpr basic_expr(Tag tag, A &&a, B &&b, C &&... c)
                    noexcept(
                        noexcept(Tag(static_cast<Tag &&>(tag))) &&
                        noexcept(proto_args_type(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...))
                    )
                  : Tag(static_cast<Tag &&>(tag))
                  , proto_args_type(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                {}

                template<typename A, typename B, typename ...C
                  , BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == sizeof...(Args))
                >
                constexpr basic_expr(A &&a, B &&b, C &&... c)
                    noexcept(noexcept(
                        basic_expr(
                            Tag()
                          , static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...
                        )
                    ))
                  : basic_expr(
                        Tag()
                      , static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...
                    )
                {}

                ////////////////////////////////////////////////////////////////////////////////////
                // accessors
                Tag & proto_tag() & noexcept
                {
                    return *this;
                }

                Tag const & proto_tag() const & noexcept
                {
                    return *this;
                }

                Tag && proto_tag() && noexcept
                {
                    return static_cast<Tag &&>(*this);
                }

                proto_args_type & proto_args() & noexcept
                {
                    return *this;
                }

                proto_args_type const & proto_args() const & noexcept
                {
                    return *this;
                }

                proto_args_type && proto_args() && noexcept
                {
                    return static_cast<proto_args_type &&>(*this);
                }

                basic_expr & proto_base() & noexcept
                {
                    return *this;
                }

                basic_expr const & proto_base() const & noexcept
                {
                    return *this;
                }

                basic_expr && proto_base() && noexcept
                {
                    return static_cast<basic_expr &&>(*this);
                }

                Domain proto_domain() const noexcept(noexcept(Domain()))
                {
                    return Domain();
                }

                template<typename ...B>
                inline auto proto_equal_to(basic_expr<Tag, args<B...>, Domain> const &that) const
                BOOST_PROTO_AUTO_RETURN(
                    proto_args() == that.proto_args()
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // struct expr
            template<typename Tag, typename Args, typename Domain>
            struct expr
              : basic_expr<Tag, Args, Domain>
              , expr_assign<expr<Tag, Args, Domain>, Domain>
              , expr_subscript<expr<Tag, Args, Domain>, Domain>
              , expr_function<expr<Tag, Args, Domain>, Domain>
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr);

                ////////////////////////////////////////////////////////////////////////////////////
                // constructors
                //using basic_expr<Tag, Args, Domain>::basic_expr;
                typedef basic_expr<Tag, Args, Domain> proto_base_expr_type;
                BOOST_PROTO_INHERIT_EXPR_CTORS(expr, proto_base_expr_type);

                ////////////////////////////////////////////////////////////////////////////////////
                // operator=
                using expr_assign<expr, Domain>::operator=;
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_expr
        template<typename T>
        struct is_expr
          : std::is_base_of<expr_base, T>
        {
            typedef is_expr type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename T>
        struct is_expr<T &>
          : std::is_base_of<expr_base, T>
        {
            typedef is_expr type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename T>
        struct is_expr<T &&>
          : std::is_base_of<expr_base, T>
        {
            typedef is_expr type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_terminal
        template<typename Expr>
        struct is_terminal
          : Expr::proto_is_terminal
        {
            typedef is_terminal type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename Expr>
        struct is_terminal<Expr &>
          : Expr::proto_is_terminal
        {
            typedef is_terminal type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename Expr>
        struct is_terminal<Expr &&>
          : Expr::proto_is_terminal
        {
            typedef is_terminal type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // arity_of
        template<typename Expr>
        struct arity_of
          : Expr::proto_arity
        {
            typedef arity_of type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename Expr>
        struct arity_of<Expr &>
          : Expr::proto_arity
        {
            typedef arity_of type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        template<typename Expr>
        struct arity_of<Expr &&>
          : Expr::proto_arity
        {
            typedef arity_of type;
            typedef mpl::integral_c_tag tag; // HACK until mpl supports std::integral_constant
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _arity_of
        struct _arity_of
          : proto::basic_action<_arity_of>
        {
            template<typename E, typename ...Rest>
            arity_of<E> operator()(E && e, Rest &&...) const noexcept
            {
                return arity_of<E>();
            };
        };
    }
}

#endif
