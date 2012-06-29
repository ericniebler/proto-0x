///////////////////////////////////////////////////////////////////////////////
// matches.hpp
// Contains the definition of the matches\<\> Boolean metafunction and the
// facilities for building Proto grammars.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_MATCHES_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include <boost/mpl/print.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_
            template<typename Expr, typename BasicExpr, typename Grammar, typename EnableIf = void>
            struct matches_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // get_basic_expr
            template<typename Expr>
            typename Expr::proto_basic_expr_type get_basic_expr(Expr const &);

            ////////////////////////////////////////////////////////////////////////////////////////
            // tag_matches
            template<typename Tag0, typename Tag1>
            struct tag_matches
              : std::false_type
            {};

            template<typename Tag>
            struct tag_matches<Tag, Tag>
              : std::true_type
            {};

            template<typename Tag>
            struct tag_matches<Tag, proto::_>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // array_matches
            template<typename T, typename U>
            struct array_matches
              : std::false_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T *>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T const *>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct array_matches<T[M], T[proto::N]>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // array_matches
            template<typename T, typename U>
            struct lambda_matches
              : std::false_type
            {};

            template<typename T>
            struct lambda_matches<T, proto::_>
              : std::true_type
            {};

            template<typename T>
            struct lambda_matches<T, T>
              : std::true_type
            {};

            template<typename T, std::size_t M, typename U>
            struct lambda_matches<T[M], U>
              : array_matches<T[M], U>
            {};

            template<typename T, std::size_t M>
            struct lambda_matches<T[M], proto::_>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct lambda_matches<T[M], T[M]>
              : std::true_type
            {};

            template<template<typename...> class T, typename ...Expr0, typename ...Grammar0>
            struct lambda_matches<T<Expr0...>, T<Grammar0...>>
              : utility::and_<lambda_matches<Expr0, Grammar0>...>
            {};

            // How terminal_matches<> handles references and cv-qualifiers.
            // The cv and ref matter *only* if the grammar has a top-level ref.
            //
            // Expr       |   Grammar    |  Matches?
            // -------------------------------------
            // T              T             yes
            // T &            T             yes
            // T const &      T             yes
            // T              T &           no
            // T &            T &           yes
            // T const &      T &           no
            // T              T const &     no
            // T &            T const &     no
            // T const &      T const &     yes

            template<typename T, typename U>
            struct is_cv_ref_compatible
              : std::true_type
            {};

            template<typename T, typename U>
            struct is_cv_ref_compatible<T, U &>
              : std::false_type
            {};

            template<typename T, typename U>
            struct is_cv_ref_compatible<T &, U &>
              : std::integral_constant<bool, std::is_const<T>::value == std::is_const<U>::value>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // terminal_matches
            template<typename T, typename U>
            struct terminal_matches
              : utility::and_<
                    is_cv_ref_compatible<T, U>
                    // TODO: instantiate fewer templates here. Make this lazy?
                  , lambda_matches<
                        typename std::remove_const<typename std::remove_reference<T>::type>::type
                      , typename std::remove_const<typename std::remove_reference<U>::type>::type
                    >
                >
            {};

            template<typename T, std::size_t M>
            struct terminal_matches<T(&)[M], T(&)[proto::N]>
              : std::true_type
            {};

            template<typename T, std::size_t M>
            struct terminal_matches<T(&)[M], T *>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T &, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T const &, T>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, proto::_>
              : std::true_type
            {};

            template<typename T>
            struct terminal_matches<T, exact<T> >
              : std::true_type
            {};

            template<typename T, typename U>
            struct terminal_matches<T, convertible_to<U> >
              : std::is_convertible<T, U>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_vararg
            template<typename ...T>
            struct is_vararg
              : std::false_type
            {};

            template<typename Grammar>
            struct is_vararg<vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename Grammar>
            struct is_vararg<T0, vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename T1, typename Grammar>
            struct is_vararg<T0, T1, vararg<Grammar>>
              : std::true_type
            {};

            template<typename T0, typename T1, typename T2, typename ...Rest>
            struct is_vararg<T0, T1, T2, Rest...>
              : is_vararg<Rest...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // vararg_matches
            template<typename Args0, typename ...Args1>
            struct vararg_matches
              : std::false_type
            {};

            template<typename Head0, typename ...Tail0, typename Head1, typename ...Tail1>
            struct vararg_matches<args<Head0, Tail0...>, Head1, Tail1...>
              : utility::and_<
                    matches_<Head0, Head0, Head1>
                  , vararg_matches<args<Tail0...>, Tail1...>
                >
            {};

            template<typename Head0, typename ...Tail0, typename Grammar>
            struct vararg_matches<args<Head0, Tail0...>, vararg<Grammar>>
              : utility::and_<
                    matches_<Head0, Head0, Grammar>
                  , matches_<Tail0, Tail0, Grammar>...
                >
            {};

            template<typename Grammar>
            struct vararg_matches<args<>, vararg<Grammar>>
              : std::true_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // matches_
            template<typename Expr, typename BasicExpr, typename Grammar, typename EnableIf>
            struct matches_
              : matches_<
                    Expr
                  , decltype(detail::get_basic_expr(std::declval<BasicExpr>()))
                  , typename Grammar::proto_grammar_type
                >
            {
                typedef Grammar proto_grammar_type;
            };

            template<typename Expr, typename BasicExpr, typename Grammar>
            struct matches_<exprs::virtual_<Expr>, exprs::virtual_<BasicExpr>, Grammar, void>
              : matches_<Expr, BasicExpr, Grammar>
            {};

            // Handle (probably-)non-variadic non-terminal matches
            template<
                typename Expr
              , template<typename...> class Expr0
              , template<typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) == sizeof...(Args1)
                >::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , matches_<Args0, Args0, Args1>...
                >
            {
                typedef Expr1<Tag1, args<Args1...>, Rest1...> proto_grammar_type;
            };

            // Handle (most definitely) variadic non-terminal matches
            template<
                typename Expr
              , template<typename...> class Expr0
              , template<typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<
                    !Tag0::proto_is_terminal::value &&
                    sizeof...(Args0) != sizeof...(Args1)
                >::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , is_vararg<Args1...>
                  , vararg_matches<args<Args0...>, Args1...>
                >
            {
                typedef Expr1<Tag1, args<Args1...>, Rest1...> proto_grammar_type;
            };

            // Handle terminal matches.
            template<
                typename Expr
              , template<typename...> class Expr0
              , template<typename...> class Expr1
              , typename Tag0, typename Value0, typename ...Rest0
              , typename Tag1, typename Value1, typename ...Rest1
            >
            struct matches_<
                Expr
              , Expr0<Tag0, args<Value0>, Rest0...>
              , Expr1<Tag1, args<Value1>, Rest1...>
              , typename std::enable_if<Tag0::proto_is_terminal::value>::type
            >
              : utility::and_<
                    tag_matches<Tag0, Tag1>
                  , terminal_matches<Value0, Value1>
                >
            {
                typedef Expr1<Tag1, args<Value1>, Rest1...> proto_grammar_type;
            };

            // Cover the case where a terminal is matched against a non-terminal
            // grammar.
            template<
                typename Expr
              , template<typename...> class Expr0
              , template<typename...> class Expr1
              , typename Tag0, typename ...Args0, typename ...Rest0
              , typename Tag1, typename ...Args1, typename ...Rest1
            >
            struct matches_<
                Expr
              , Expr0<Tag0, args<Args0...>, Rest0...>
              , Expr1<Tag1, args<Args1...>, Rest1...>
              , typename std::enable_if<Tag0::proto_is_terminal::value>::type
            >
              : std::false_type
            {
                typedef Expr1<Tag1, args<Args1...>, Rest1...> proto_grammar_type;
            };

            // Handle wildcard matches
            template<typename Expr, typename BasicExpr>
            struct matches_<Expr, BasicExpr, proto::_, void>
              : std::true_type
            {
                typedef proto::_ proto_grammar_type;
            };

            // Necessary for full variadic non-terminal match support
            template<typename Expr, typename BasicExpr, typename Grammar>
            struct matches_<Expr, BasicExpr, proto::vararg<Grammar>, void>
              : matches_<Expr, BasicExpr, Grammar>
            {
                typedef proto::vararg<Grammar> proto_grammar_type;
            };

            // Handle proto::or_
            template<typename Expr, typename BasicExpr, typename ...Grammars>
            struct matches_<Expr, BasicExpr, proto::or_<Grammars...>, void>
              : utility::or_<matches_<Expr, BasicExpr, Grammars>...>
            {
                typedef proto::or_<Grammars...> proto_grammar_type;
            };

            // Handle proto::and_
            template<typename Expr, typename BasicExpr, typename ...Grammars>
            struct matches_<Expr, BasicExpr, proto::and_<Grammars...>, void>
              : utility::and_<matches_<Expr, BasicExpr, Grammars>...>
            {
                typedef proto::and_<Grammars...> proto_grammar_type;
            };

            // Handle proto::not_
            template<typename Expr, typename BasicExpr, typename Grammar>
            struct matches_<Expr, BasicExpr, proto::not_<Grammar>, void>
              : std::integral_constant<bool, !matches_<Expr, BasicExpr, Grammar>::value>
            {
                typedef proto::not_<Grammar> proto_grammar_type;
            };

            // Handle proto::if_
            template<typename Expr, typename BasicExpr, typename If, typename Then, typename Else>
            struct matches_<Expr, BasicExpr, proto::if_<If, Then, Else>, void>
              : std::conditional<
                    static_cast<bool>(
                        std::remove_reference<
                            decltype(as_transform<If>()(std::declval<Expr>()))
                        >::type::value
                    )
                  , matches_<Expr, BasicExpr, Then>
                  , matches_<Expr, BasicExpr, Else>
                >::type
            {
                typedef proto::if_<If, Then, Else> proto_grammar_type;
            };

            // Handle proto::switch_
            template<typename Expr, typename BasicExpr, typename Cases, typename Transform>
            struct matches_<Expr, BasicExpr, proto::switch_<Cases, Transform>, void>
              : matches_<
                    Expr
                  , BasicExpr
                  , typename Cases::template case_<
                        decltype(as_transform<Transform>()(std::declval<Expr>()))
                    >::proto_grammar_type
                >
            {
                typedef proto::switch_<Cases, Transform> proto_grammar_type;
            };
        }

        /// \brief A Boolean metafunction that evaluates whether a given
        /// expression type matches a grammar.
        ///
        /// <tt>matches\<Expr,Grammar\></tt> inherits (indirectly) from
        /// \c std::true_type if <tt>Expr::proto_basic_expr_type</tt> matches
        /// <tt>Grammar::proto_grammar_type</tt>, and from \c std::false_type
        /// otherwise.
        ///
        /// Non-terminal expressions are matched against a grammar
        /// according to the following rules:
        ///
        /// \li The wildcard pattern, \c _, matches any expression.
        /// \li An expression <tt>expr\<AT, args\<A0,A1,...An\> \></tt>
        ///     matches a grammar <tt>expr\<BT, args\<B0,B1,...Bn\> \></tt>
        ///     if \c BT is \c _ or \c AT, and if \c Ax matches \c Bx for
        ///     each \c x in <tt>[0,n)</tt>.
        /// \li An expression <tt>expr\<AT, args\<A0,...An,U0,...Um\> \></tt>
        ///     matches a grammar <tt>expr\<BT, args\<B0,...Bn,vararg\<V\> \> \></tt>
        ///     if \c BT is \c _ or \c AT, and if \c Ax matches \c Bx
        ///     for each \c x in <tt>[0,n)</tt> and if \c Ux matches \c V
        ///     for each \c x in <tt>[0,m)</tt>.
        /// \li An expression \c E matches <tt>or_\<B0,B1,...Bn\></tt> if \c E
        ///     matches some \c Bx for \c x in <tt>[0,n)</tt>.
        /// \li An expression \c E matches <tt>and_\<B0,B1,...Bn\></tt> if \c E
        ///     matches all \c Bx for \c x in <tt>[0,n)</tt>.
        /// \li An expression \c E matches <tt>if_\<T,U,V\></tt> if
        ///     <tt>std::result_of\<when\<_,T\>(E)\>::type::value</tt>
        ///     is \c true and \c E matches \c U; or, if
        ///     <tt>std::result_of\<when\<_,T\>(E)\>::type::value</tt>
        ///     is \c false and \c E matches \c V. (Note: \c U defaults to \c _
        ///     and \c V defaults to \c not_\<_\>.)
        /// \li An expression \c E matches <tt>not_\<T\></tt> if \c E does
        ///     not match \c T.
        /// \li An expression \c E matches <tt>switch_\<C,T\></tt> if
        ///     \c E matches <tt>C::case_\<std::result_of\<T(E)\>::type\></tt>.
        ///     (Note: T defaults to <tt>tag_of\<_\>()</tt>.)
        ///
        /// A terminal expression <tt>expr\<AT,args\<A\> \></tt> matches
        /// a grammar <tt>expr\<BT,args\<B\> \></tt> if \c BT is \c AT or
        /// \c proto::_ and if one of the following is true:
        ///
        /// \li \c B is the wildcard pattern, \c _
        /// \li \c A is \c B
        /// \li \c A is <tt>B &</tt>
        /// \li \c A is <tt>B const &</tt>
        /// \li \c B is <tt>exact\<A\></tt>
        /// \li \c B is <tt>convertible_to\<X\></tt> and
        ///     <tt>std::is_convertible\<A,X\>::value</tt> is \c true.
        /// \li \c A is <tt>X[M]</tt> or <tt>X(&)[M]</tt> and
        ///     \c B is <tt>X[proto::N]</tt>.
        /// \li \c A is <tt>X(&)[M]</tt> and \c B is <tt>X(&)[proto::N]</tt>.
        /// \li \c A is <tt>X[M]</tt> or <tt>X(&)[M]</tt> and
        ///     \c B is <tt>X*</tt>.
        /// \li \c B lambda-matches \c A (see below).
        ///
        /// A type \c B lambda-matches \c A if one of the following is true:
        ///
        /// \li \c B is \c A
        /// \li \c B is the wildcard pattern, \c _
        /// \li \c B is <tt>T\<B0,B1,...Bn\></tt> and \c A is
        ///     <tt>T\<A0,A1,...An\></tt> and for each \c x in
        ///     <tt>[0,n)</tt>, \c Ax and \c Bx are types
        ///     such that \c Ax lambda-matches \c Bx
        template<typename Expr, typename Grammar>
        struct matches
          : detail::matches_<Expr, Expr, Grammar>
        {};

        /// \brief A wildcard grammar element that matches any expression,
        /// and a transform that returns the current expression unchanged.
        ///
        /// The wildcard type, \c _, is a grammar element such that
        /// <tt>matches\<E,_\>::value</tt> is \c true for any expression
        /// type \c E.
        ///
        /// The wildcard can also be used as a stand-in for a template
        /// argument when matching terminals. For instance, the following
        /// is a grammar that will match any <tt>std::complex\<\></tt>
        /// terminal:
        ///
        /// \code
        /// BOOST_MPL_ASSERT((
        ///     matches<
        ///         terminal<std::complex<double> >::type
        ///       , terminal<std::complex< _ > >
        ///     >
        /// ));
        /// \endcode
        ///
        /// When used as a transform, \c _ returns the current expression
        /// unchanged. For instance, in the following, \c _ is used with
        /// the \c fold\<\> transform to fold the children of a node:
        ///
        /// \code
        /// struct CountChildren
        ///   : or_<
        ///         // Terminals have no children
        ///         when<terminal<_>, mpl::int_<0>()>
        ///         // Use fold<> to count the children of non-terminals
        ///       , otherwise<
        ///             fold<
        ///                 _ // <-- fold the current expression
        ///               , mpl::int_<0>()
        ///               , mpl::plus<_state, mpl::int_<1> >()
        ///             >
        ///         >
        ///     >
        /// {};
        /// \endcode
        struct _
          : transform<_>
        {
            typedef _ proto_grammar_type;

            template<typename Expr, typename ...Rest>
            auto operator()(Expr && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Expr &&>(e)
            )
        };

        /// \brief Inverts the set of expressions matched by a grammar. When
        /// used as a transform, \c not_\<\> returns the current expression
        /// unchanged.
        ///
        /// If an expression type \c E does not match a grammar \c G, then
        /// \c E \e does match <tt>not_\<G\></tt>. For example,
        /// <tt>not_\<terminal\<_\> \></tt> will match any non-terminal.
        template<typename Grammar>
        struct not_
          : transform<not_<Grammar>>
        {
            typedef not_ proto_grammar_type;

            template<typename Expr, typename ...Rest>
            auto operator()(Expr && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                static_cast<Expr &&>(e)
            )
        };

        /// \brief Used to select one grammar or another based on the result
        /// of a compile-time Boolean. When used as a transform, \c if_\<\>
        /// selects between two transforms based on a compile-time Boolean.
        ///
        /// When <tt>if_\<If,Then,Else\></tt> is used as a grammar, \c If
        /// must be a Proto transform and \c Then and \c Else must be grammars.
        /// An expression type \c E matches <tt>if_\<If,Then,Else\></tt> if
        /// <tt>std::result_of\<when\<_,If\>(E)\>::type::value</tt>
        /// is \c true and \c E matches \c U; or, if
        /// <tt>std::result_of\<when\<_,If\>(E)\>::type::value</tt>
        /// is \c false and \c E matches \c V.
        ///
        /// The template parameter \c Then defaults to \c _
        /// and \c Else defaults to \c not\<_\>, so an expression type \c E
        /// will match <tt>if_\<If\></tt> if and only if
        /// <tt>std::result_of\<when\<_,If\>(E)\>::type::value</tt>
        /// is \c true.
        ///
        /// \code
        /// // A grammar that only matches integral terminals,
        /// // using is_integral<> from Boost.Type_traits.
        /// struct IsIntegral
        ///   : and_<
        ///         terminal<_>
        ///       , if_< is_integral<_value>() >
        ///     >
        /// {};
        /// \endcode
        ///
        /// When <tt>if_\<If,Then,Else\></tt> is used as a transform, \c If,
        /// \c Then and \c Else must be Proto transforms. When applying
        /// the transform to an expression \c E, state \c S and data \c V,
        /// if <tt>boost::result_of\<when\<_,If\>(E,S,V)\>::type::value</tt>
        /// is \c true then the \c Then transform is applied; otherwise
        /// the \c Else transform is applied.
        ///
        /// \code
        /// // Match a terminal. If the terminal is integral, return
        /// // mpl::true_; otherwise, return mpl::false_.
        /// struct IsIntegral2
        ///   : when<
        ///         terminal<_>
        ///       , if_<
        ///             is_integral<_value>()
        ///           , std::true_type()
        ///           , std::false_type()
        ///         >
        ///     >
        /// {};
        /// \endcode
        template<typename If, typename Then /* = _*/, typename Else /*= not_<_>*/>
        struct if_
          : transform<if_<If, Then, Else>>
        {
            typedef if_ proto_grammar_type;

            template<typename Expr, typename ...Rest>
            auto operator()(Expr && e, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                typename std::conditional<
                    static_cast<bool>(
                        std::remove_reference<
                            decltype(as_transform<If>()(static_cast<Expr &&>(e)))
                        >::type::value
                    )
                  , as_transform<Then>
                  , as_transform<Else>
                >::type()(static_cast<Expr &&>(e), static_cast<Rest &&>(rest)...)
            )
        };

        /// \brief For matching one of a set of alternate grammars. Alternates
        /// tried in order to avoid ambiguity. When used as a transform, \c or_\<\>
        /// applies the transform associated with the first grammar that matches
        /// the expression.
        ///
        /// An expression type \c E matches <tt>or_\<B0,B1,...Bn\></tt> if \c E
        /// matches any \c Bx for \c x in <tt>[0,n)</tt>.
        ///
        /// When applying <tt>or_\<B0,B1,...Bn\></tt> as a transform with an
        /// expression \c e of type \c E, state \c s and data \c d, it is
        /// equivalent to <tt>Bx()(e, s, d)</tt>, where \c x is the lowest
        /// number such that <tt>matches\<E,Bx\>::value</tt> is \c true.
        template<typename... Grammars>
        struct or_
          : transform<or_<Grammars...>>
        {
            typedef or_ proto_grammar_type;

            template<typename Expr, typename ...Rest>
            auto operator()(Expr && e, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                as_transform<typename matches<Expr, or_>::which::proto_grammar_type>()(
                    static_cast<Expr &&>(e)
                  , static_cast<Rest &&>(rest)...
                )
            )
        };

        /// \brief For matching all of a set of grammars. When used as a
        /// transform, \c and_\<\> applies the transforms associated with
        /// the each grammar in the set, and returns the result of the last.
        ///
        /// An expression type \c E matches <tt>and_\<B0,B1,...Bn\></tt> if \c E
        /// matches all \c Bx for \c x in <tt>[0,n)</tt>.
        ///
        /// When applying <tt>and_\<B0,B1,...Bn\></tt> as a transform with an
        /// expression \c e, state \c s and data \c d, it is
        /// equivalent to <tt>(B0()(e, s, d),B1()(e, s, d),...Bn()(e, s, d))</tt>.
        template<typename... Grammars>
        struct and_
          : transform<and_<Grammars...>>
        {
            typedef and_ proto_grammar_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                utility::back(as_transform<Grammars>()(static_cast<Args &&>(args)...)...)
            )
        };

        /// \brief For matching one of a set of alternate grammars, which
        /// are looked up based on some property of an expression. The
        /// property on which to dispatch is specified by the \c Transform
        /// template parameter, which defaults to <tt>tag_of\<_\>()</tt>.
        /// That is, when the \c Trannsform is not specified, the alternate
        /// grammar is looked up using the tag type of the current expression.
        ///
        /// When used as a transform, \c switch_\<\> applies the transform
        /// associated with the grammar that matches the expression.
        ///
        /// \note \c switch_\<\> is functionally identical to \c or_\<\> but
        /// is often more efficient. It does a fast, O(1) lookup using the
        /// result of the specified transform to find a sub-grammar that may
        /// potentially match the expression.
        ///
        /// An expression type \c E matches <tt>switch_\<C,T\></tt> if \c E
        /// matches <tt>C::case_\<std::result_of\<T(E)\>::type\></tt>.
        ///
        /// When applying <tt>switch_\<C,T\></tt> as a transform with an
        /// expression \c e of type \c E, state \c s of type \S and data
        /// \c d of type \c D, it is equivalent to
        /// <tt>C::case_\<std::result_of\<T(E,S,D)\>::type\>()(e, s, d)</tt>.
        template<typename Cases, typename Transform>
        struct switch_
          : transform<switch_<Cases, Transform>>
        {
            typedef switch_ proto_grammar_type;

            template<typename Expr, typename ...Rest>
            auto operator()(Expr && e, Rest &&... rest) const
            BOOST_PROTO_AUTO_RETURN(
                typename Cases::template case_<
                    decltype(as_transform<Transform>()(static_cast<Expr &&>(e)))
                >()(static_cast<Expr &&>(e), static_cast<Rest &&>(rest)...)
            )
        };

        /// \brief For forcing exact matches of terminal types.
        ///
        /// By default, matching terminals ignores references and
        /// cv-qualifiers. For instance, a terminal expression of
        /// type <tt>terminal\<int const &\>::type</tt> will match
        /// the grammar <tt>terminal\<int\></tt>. If that is not
        /// desired, you can force an exact match with
        /// <tt>terminal\<exact\<int\> \></tt>. This will only
        /// match integer terminals where the terminal is held by
        /// value.
        template<typename T>
        struct exact
        {};

        /// \brief For matching terminals that are convertible to
        /// a type.
        ///
        /// Use \c convertible_to\<\> to match a terminal that is
        /// convertible to some type. For example, the grammar
        /// <tt>terminal\<convertible_to\<int\> \></tt> will match
        /// any terminal whose argument is convertible to an integer.
        ///
        /// \note The trait \c is_convertible\<\> from Boost.Type_traits
        /// is used to determinal convertibility.
        template<typename T>
        struct convertible_to
        {};

        /// \brief For matching a Grammar to a variable number of
        /// sub-expressions.
        ///
        /// An expression type <tt>expr\<AT, listN\<A0,...An,U0,...Um\> \></tt>
        /// matches a grammar <tt>expr\<BT, listM\<B0,...Bn,vararg\<V\> \> \></tt>
        /// if \c BT is \c _ or \c AT, and if \c Ax matches \c Bx
        /// for each \c x in <tt>[0,n)</tt> and if \c Ux matches \c V
        /// for each \c x in <tt>[0,m)</tt>.
        ///
        /// For example:
        ///
        /// \code
        /// // Match any function call expression, irregardless
        /// // of the number of function arguments:
        /// struct Function
        ///   : function< vararg<_> >
        /// {};
        /// \endcode
        ///
        /// When used as a transform, <tt>vararg\<G\></tt> applies
        /// <tt>G</tt>'s transform.
        template<typename Grammar>
        struct vararg
          : Grammar
        {
            typedef vararg proto_grammar_type;
        };
    }
}

#endif // BOOST_PROTO_MATCHES_HPP_INCLUDED
