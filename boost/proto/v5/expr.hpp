////////////////////////////////////////////////////////////////////////////////////////////////////
// expr.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_EXPR_HPP_INCLUDED
#define BOOST_PROTO_CXX11_EXPR_HPP_INCLUDED

#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c_fwd.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>
#include <boost/proto/v5/accessors.hpp>
#include <boost/proto/v5/domain.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/detail/compressed_pair.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // are_equality_comparible
                template<typename L, typename R
                  , BOOST_PROTO_ENABLE_IF_VALID_EXPR(std::declval<L>().proto_equal_to(std::declval<R>()))>
                std::true_type are_equality_comparible_(int);

                template<typename L, typename R>
                std::false_type are_equality_comparible_(long);

                template<typename L, typename R>
                using are_equality_comparible = decltype(detail::are_equality_comparible_<L, R>(1));

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr_boolean_convertible
                //  expr_boolean_convertible and friends are very carefully crafted, along with
                //  proto::args::operator==, to only allow expressions that can be compared to have
                //  a functioning proto_equal_to, and also to allow a==b and a!=b to have implicit
                //  conversions to bool only where it makes sense to allow it.
                ////////////////////////////////////////////////////////////////////////////////////
                template<typename ExprDesc, typename EnableIf = void>
                struct expr_boolean_convertible
                {};

                // We use the nasty, old smart-bool idiom below instead of an explicit converion
                // to bool because an explicit boolean conversion doesn't satisfy the requirements
                // of the EqualityComparible concept.
                template<typename T, typename U, typename Domain>
                struct expr_boolean_convertible<
                    exprs::basic_expr<equal_to(T, U), Domain>
                  , typename std::enable_if<are_equality_comparible<T, U>::value>::type
                >
                {
                private:
                    struct proto_smart_bool { int member; };
                    using unspecified_bool_type = int proto_smart_bool::*;

                public:
                    inline operator unspecified_bool_type() const
                    BOOST_PROTO_RETURN(
                        proto::v5::child<0>(static_cast<exprs::basic_expr<equal_to(T, U), Domain> const &>(*this)).proto_equal_to(
                        proto::v5::child<1>(static_cast<exprs::basic_expr<equal_to(T, U), Domain> const &>(*this)))
                        ? &proto_smart_bool::member : 0
                    )
                };

                template<typename T, typename U, typename Domain>
                struct expr_boolean_convertible<
                    exprs::basic_expr<not_equal_to(T, U), Domain>
                  , typename std::enable_if<are_equality_comparible<T, U>::value>::type
                >
                {
                private:
                    struct proto_smart_bool { int member; };
                    using unspecified_bool_type = int proto_smart_bool::*;

                public:
                    inline operator unspecified_bool_type() const
                    BOOST_PROTO_RETURN(
                        proto::v5::child<0>(static_cast<exprs::basic_expr<not_equal_to(T, U), Domain> const &>(*this)).proto_equal_to(
                        proto::v5::child<1>(static_cast<exprs::basic_expr<not_equal_to(T, U), Domain> const &>(*this)))
                        ? 0 : &proto_smart_bool::member
                    )
                };

                template<typename T, typename Domain>
                struct expr_boolean_convertible<
                    exprs::basic_expr<logical_not(T), Domain>
                  , typename std::enable_if<std::is_convertible<T, bool>::value>::type
                >
                {
                private:
                    struct proto_smart_bool { int member; };
                    using unspecified_bool_type = int proto_smart_bool::*;

                public:
                    inline operator unspecified_bool_type() const
                    BOOST_PROTO_RETURN(
                        proto::v5::child<0>(static_cast<exprs::basic_expr<logical_not(T), Domain> const &>(*this))
                        ? 0 : &proto_smart_bool::member
                    )
                };

                template<typename T, typename U, typename Domain>
                struct expr_boolean_convertible<
                    exprs::basic_expr<logical_and(T, U), Domain>
                  , typename std::enable_if<
                        std::is_convertible<T, bool>::value && std::is_convertible<U, bool>::value
                    >::type
                >
                {
                private:
                    struct proto_smart_bool { int member; };
                    using unspecified_bool_type = int proto_smart_bool::*;

                public:
                    inline operator unspecified_bool_type() const
                    BOOST_PROTO_RETURN(
                        static_cast<bool>(proto::v5::child<0>(static_cast<exprs::basic_expr<logical_and(T, U), Domain> const &>(*this))) &&
                        static_cast<bool>(proto::v5::child<1>(static_cast<exprs::basic_expr<logical_and(T, U), Domain> const &>(*this)))
                        ? &proto_smart_bool::member : 0
                    )
                };

                template<typename T, typename U, typename Domain>
                struct expr_boolean_convertible<
                    exprs::basic_expr<logical_or(T, U), Domain>
                  , typename std::enable_if<
                        std::is_convertible<T, bool>::value && std::is_convertible<U, bool>::value
                    >::type
                >
                {
                private:
                    struct proto_smart_bool { int member; };
                    using unspecified_bool_type = int proto_smart_bool::*;

                public:
                    inline operator unspecified_bool_type() const
                    BOOST_PROTO_RETURN(
                        static_cast<bool>(proto::v5::child<0>(static_cast<exprs::basic_expr<logical_or(T, U), Domain> const &>(*this))) ||
                        static_cast<bool>(proto::v5::child<1>(static_cast<exprs::basic_expr<logical_or(T, U), Domain> const &>(*this)))
                        ? &proto_smart_bool::member : 0
                    )
                };

                template<typename Expr, typename Domain>
                struct as_child_
                {
                    using type = Expr;
                };

                template<typename Tag, typename ...Children, typename Domain>
                struct as_child_<Tag(*)(Children...), Domain>
                  : result_of::make_expr<Tag(typename as_child_<Children, Domain>::type...), Domain>
                {};

                template<typename Tag, typename ...Children, typename Domain>
                struct as_basic_expr_<Tag(Children...), Domain>
                {
                    using type = exprs::basic_expr<Tag(typename as_child_<Children, Domain>::type...), Domain>;
                };

                template<typename Expr, typename Domain>
                struct as_expr_
                {
                    using type = exprs::expr<Expr, Domain>;
                };

                template<typename Tag, typename ...Children, typename Domain>
                struct as_expr_<Tag(Children...), Domain>
                {
                    using type = exprs::expr<Tag(typename as_child_<Children, Domain>::type...), Domain>;
                };
            }

            namespace exprs
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr_assign
                template<typename Expr>
                struct expr_assign
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_assign);

                    ////////////////////////////////////////////////////////////////////////////////
                    // operator=
                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                    auto operator=(U && u) &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::assign, Domain>(
                            static_cast<E &>(*this)
                          , static_cast<U &&>(u)
                        )
                    )

                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                    constexpr auto operator=(U && u) const &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::assign, Domain>(
                            static_cast<E const &>(*this)
                          , static_cast<U &&>(u)
                        )
                    )

                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type
                      , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<expr_assign, U>::value))>
                    constexpr auto operator=(U && u) const &&
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::assign, Domain>(
                            static_cast<E &&>(const_cast<expr_assign &&>(*this))
                          , static_cast<U &&>(u)
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr_subscript
                template<typename Expr>
                struct expr_subscript
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_subscript);

                    ////////////////////////////////////////////////////////////////////////////////
                    // operator[]
                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    auto operator[](U && u) &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::subscript, Domain>(
                            static_cast<E &>(*this)
                          , static_cast<U &&>(u)
                        )
                    )

                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    constexpr auto operator[](U && u) const &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::subscript, Domain>(
                            static_cast<E const &>(*this)
                          , static_cast<U &&>(u)
                        )
                    )

                    template<typename U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    constexpr auto operator[](U && u) const &&
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::subscript, Domain>(
                            static_cast<E &&>(const_cast<expr_subscript &&>(*this))
                          , static_cast<U &&>(u)
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr_function
                template<typename Expr>
                struct expr_function
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_function);

                    ////////////////////////////////////////////////////////////////////////////////
                    // operator()
                    template<typename ...U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    auto operator()(U &&... u) &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::function, Domain>(
                            static_cast<E &>(*this)
                          , static_cast<U &&>(u)...
                        )
                    )

                    template<typename ...U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    constexpr auto operator()(U &&... u) const &
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::function, Domain>(
                            static_cast<E const &>(*this)
                          , static_cast<U &&>(u)...
                        )
                    )

                    template<typename ...U, typename E = Expr
                      , typename Domain = typename E::proto_domain_type>
                    constexpr auto operator()(U &&... u) const &&
                    BOOST_PROTO_AUTO_RETURN(
                        v5::make_expr<v5::function, Domain>(
                            static_cast<E &&>(const_cast<expr_function &&>(*this))
                          , static_cast<U &&>(u)...
                        )
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr_base
                struct expr_base
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // struct basic_expr
                template<typename Tag, typename ...Children, typename Domain>
                struct basic_expr<Tag(Children...), Domain>
                  : expr_base
                  , detail::expr_boolean_convertible<basic_expr<Tag(Children...), Domain>>
                {
                private:
                    using tag_and_children_type =
                        detail::compressed_pair<
                            Tag
                          , exprs::children<Children...>
                        >;
                    tag_and_children_type tag_and_children_;

                public:
                    ////////////////////////////////////////////////////////////////////////////////
                    // Check constraints
                    static_assert(
                        !detail::is_terminal_tag<Tag>::value || sizeof...(Children) <= 1
                      , "terminals can have only 1 value"
                    );

                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(basic_expr);

                    ////////////////////////////////////////////////////////////////////////////////
                    // usings and typedefs
                    using proto_tag_type                = Tag;
                    using proto_is_terminal_type        = typename proto_tag_type::proto_is_terminal_type;
                    using proto_children_type           = exprs::children<Children...>;
                    using proto_size                    = typename proto_children_type::proto_size;
                    using proto_domain_type             = Domain;
                    using proto_basic_expr_type         = basic_expr;
                    using proto_expr_descriptor_type    = Tag(Children...);
                    using proto_arity                   =
                        std::integral_constant<
                            std::size_t
                          , detail::is_terminal_tag<Tag>::value ? 0 : sizeof...(Children)
                        >;

                    using fusion_tag                    = proto_expr<Tag, proto_domain_type>; ///< For Fusion

                    ////////////////////////////////////////////////////////////////////////////////
                    // constructors
                private:
                    constexpr basic_expr(Tag tag, proto_children_type args)
                        noexcept(
                            noexcept(
                                tag_and_children_type(
                                    static_cast<Tag &&>(tag)
                                  , static_cast<proto_children_type &&>(args)
                                )
                            )
                        )
                      : tag_and_children_(
                            static_cast<Tag &&>(tag)
                          , static_cast<proto_children_type &&>(args)
                        )
                    {}

                    explicit constexpr basic_expr(proto_children_type args)
                        noexcept(noexcept(basic_expr(Tag(), static_cast<proto_children_type &&>(args))))
                      : basic_expr(Tag(), static_cast<proto_children_type &&>(args))
                    {}

                public:
                    template<typename A
                      , BOOST_PROTO_ENABLE_IF(
                            sizeof...(Children) == 1 && !(utility::is_base_of<basic_expr, A>::value)
                        )
                    >
                    constexpr basic_expr(Tag tag, A &&a)
                        noexcept(
                            noexcept(
                                tag_and_children_type(
                                    static_cast<Tag &&>(tag)
                                  , proto_children_type(static_cast<A &&>(a))
                                )
                            )
                        )
                      : tag_and_children_(
                            static_cast<Tag &&>(tag)
                          , proto_children_type(static_cast<A &&>(a))
                        )
                    {}

                    template<
                        typename A
                      , BOOST_PROTO_ENABLE_IF(
                            sizeof...(Children) == 1 && !(utility::is_base_of<basic_expr, A>::value)
                        )
                    >
                    explicit constexpr basic_expr(A &&a)
                        // http://llvm.org/bugs/show_bug.cgi?id=15757
                        // noexcept(noexcept(
                        //     basic_expr(Tag(), static_cast<A &&>(a))
                        // ))
                      : basic_expr(Tag(), static_cast<A &&>(a))
                    {}

                    template<typename A, typename B, typename ...C
                      , BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == sizeof...(Children))
                    >
                    constexpr basic_expr(Tag tag, A &&a, B &&b, C &&... c)
                        noexcept(
                            noexcept(
                                tag_and_children_type(
                                    static_cast<Tag &&>(tag)
                                  , proto_children_type(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                                )
                            )
                        )
                      : tag_and_children_(
                            static_cast<Tag &&>(tag)
                          , proto_children_type(static_cast<A &&>(a), static_cast<B &&>(b), static_cast<C &&>(c)...)
                        )
                    {}

                    template<typename A, typename B, typename ...C
                      , BOOST_PROTO_ENABLE_IF(sizeof...(C) + 2 == sizeof...(Children))
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

                    ////////////////////////////////////////////////////////////////////////////////
                    // accessors
                    Tag & proto_tag() & noexcept
                    {
                        return tag_and_children_.first();
                    }

                    Tag const & proto_tag() const & noexcept
                    {
                        return tag_and_children_.first();
                    }

                    Tag && proto_tag() && noexcept
                    {
                        return static_cast<tag_and_children_type &&>(tag_and_children_).first();
                    }

                    proto_children_type & proto_args() & noexcept
                    {
                        return tag_and_children_.second();
                    }

                    proto_children_type const & proto_args() const & noexcept
                    {
                        return tag_and_children_.second();
                    }

                    proto_children_type && proto_args() && noexcept
                    {
                        return static_cast<tag_and_children_type &&>(tag_and_children_).second();
                    }

                    Domain proto_domain() const noexcept(noexcept(Domain()))
                    {
                        return Domain();
                    }

                    template<typename ...B>
                    inline auto proto_equal_to(basic_expr<Tag(B...), Domain> const &that) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto_args() == that.proto_args()
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr
                template<
                    template<typename, typename...> class DerivedExpr
                  , typename ExprDesc
                  , typename ...Rest
                  , typename Domain
                >
                struct expr<DerivedExpr<ExprDesc, Rest...>, Domain>
                  : basic_expr<ExprDesc, Domain>
                  , expr_assign<DerivedExpr<ExprDesc, Rest...>>
                  , expr_subscript<DerivedExpr<ExprDesc, Rest...>>
                  , expr_function<DerivedExpr<ExprDesc, Rest...>>
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // constructors
                    using basic_expr<ExprDesc, Domain>::basic_expr;

                    ////////////////////////////////////////////////////////////////////////////////
                    // operator=
                    using expr_assign<DerivedExpr<ExprDesc, Rest...>>::operator=;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // struct expr
                template<typename Tag, typename ...Children, typename Domain>
                struct expr<Tag(Children...), Domain>
                  : basic_expr<Tag(Children...), Domain>
                  , expr_assign<expr<Tag(Children...), Domain>>
                  , expr_subscript<expr<Tag(Children...), Domain>>
                  , expr_function<expr<Tag(Children...), Domain>>
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // constructors
                    using basic_expr<Tag(Children...), Domain>::basic_expr;

                    ////////////////////////////////////////////////////////////////////////////////
                    // operator=
                    using expr_assign<expr>::operator=;
                };
            }

            namespace result_of
            {
                template<typename Expr, std::size_t N>
                struct child
                {
                    using type = decltype(proto::v5::child<N>(std::declval<Expr>()));
                };

                template<typename Expr>
                struct left
                {
                    using type = decltype(proto::v5::left(std::declval<Expr>()));
                };

                template<typename Expr>
                struct right
                {
                    using type = decltype(proto::v5::right(std::declval<Expr>()));
                };

                template<typename Expr>
                struct value
                {
                    using type = decltype(proto::v5::value(std::declval<Expr>()));
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_expr
            template<typename T>
            struct is_expr
              : std::is_base_of<expr_base, T>
            {
                using type = is_expr;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            template<typename T>
            struct is_expr<T &>
              : std::is_base_of<expr_base, T>
            {
                using type = is_expr;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            template<typename T>
            struct is_expr<T &&>
              : std::is_base_of<expr_base, T>
            {
                using type = is_expr;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_terminal
            template<typename Expr>
            struct is_terminal
              : Expr::proto_is_terminal_type
            {
                using type = is_terminal;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            template<typename Expr>
            struct is_terminal<Expr &>
              : Expr::proto_is_terminal_type
            {
                using type = is_terminal;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            template<typename Expr>
            struct is_terminal<Expr &&>
              : Expr::proto_is_terminal_type
            {
                using type = is_terminal;
                using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
            };

            namespace result_of
            {
                ////////////////////////////////////////////////////////////////////////////////////////
                // arity_of
                template<typename Expr>
                struct arity_of
                  : Expr::proto_arity
                {
                    using type = arity_of;
                    using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
                };

                template<typename Expr>
                struct arity_of<Expr &>
                  : Expr::proto_arity
                {
                    using type = arity_of;
                    using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
                };

                template<typename Expr>
                struct arity_of<Expr &&>
                  : Expr::proto_arity
                {
                    using type = arity_of;
                    using tag = mpl::integral_c_tag; // HACK until mpl supports std::integral_constant
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _arity_of
            struct _arity_of
              : proto::v5::basic_action<_arity_of>
            {
                template<typename E, typename ...Rest>
                result_of::arity_of<E> operator()(E &&, Rest &&...) const noexcept
                {
                    return result_of::arity_of<E>();
                };
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _child
            template<std::size_t N>
            struct _child
              : basic_action<_child<N>>
            {
                template<typename E, typename ...Rest>
                auto operator()(E && e, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::v5::child<N>(e)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _value
            struct _value
              : basic_action<_value>
            {
                template<typename E, typename ...Rest>
                auto operator()(E && e, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::v5::value(e)
                )
            };
        }
    }
}

#endif
