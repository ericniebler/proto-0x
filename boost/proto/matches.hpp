////////////////////////////////////////////////////////////////////////////////////////////////////
// matches.hpp
// Contains the definition of the matches\<\> Boolean metafunction and the
// facilities for building Proto grammars.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_MATCHES_HPP_INCLUDED
#define BOOST_PROTO_MATCHES_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct ERROR_UNKNOWN_GRAMMAR_PATTERN
            {};
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
          : matches<Expr, typename grammar_of<Grammar>::type>
        {};

        template<typename Expr, typename Ret, typename ...Args>
        struct matches<Expr, Ret(*)(Args...)>
          : matches<Expr, Ret(Args...)>
        {};

        template<typename Expr, typename Ret, typename ...Args>
        struct matches<Expr, Ret(*)(Args......)>
          : matches<Expr, Ret(Args......)>
        {};

        // Defined in grammar/expr.hpp
        template<typename Expr, typename Tag, typename ...Grammars>
        struct matches<Expr, Tag(Grammars...)>;

        // Defined in grammar/expr.hpp
        template<typename Expr, typename Tag, typename ...Grammars>
        struct matches<Expr, Tag(Grammars......)>;

        template<typename T>
        struct fuzzy
        {};

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
    }
}

#endif // BOOST_PROTO_MATCHES_HPP_INCLUDED
