////////////////////////////////////////////////////////////////////////////////////////////////////
// placeholders.hpp
// The proto::_ wildcard for use as a grammar element and also as a trivial action.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_PLACEHOLDERS_HPP_INCLUDED
#define BOOST_PROTO_V5_PLACEHOLDERS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/grammar/basic_grammar.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            /// \brief A wildcard grammar element that matches any expression,
            /// and a basic_action that returns the current expression unchanged.
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
            ///         expr<terminal(std::complex< double >)>
            ///       , terminal(std::complex< _ >)
            ///     >
            /// ));
            /// \endcode
            ///
            /// When used as a basic_action, \c _ returns the current expression
            /// unchanged. For instance, in the following, \c _ is used with
            /// the \c _fold\<\> basic_action to _fold the children of a node:
            ///
            /// \code
            /// struct CountChildren
            ///   : or_<
            ///         // Terminals have no children
            ///         when<literal<_>, mpl::int_<0>()>
            ///         // Use _fold<> to count the children of non-terminals
            ///       , otherwise<
            ///             _fold<
            ///                 _ // <-- _fold the current expression
            ///               , mpl::int_<0>()
            ///               , mpl::plus<_state, mpl::int_<1>>()
            ///             >
            ///         >
            ///     >
            /// {};
            /// \endcode
            struct _
              : basic_action<_>
              , basic_grammar<_>
            {
                template<typename Expr, typename ...Rest>
                constexpr auto operator()(Expr && e, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Expr &&>(e)
                )

                // Makes _ a basic grammar element:
                template<typename Expr>
                struct apply
                  : std::true_type
                {};
            };
        }
    }
}

#endif
