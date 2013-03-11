////////////////////////////////////////////////////////////////////////////////////////////////////
// if.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_IF_HPP_INCLUDED
#define BOOST_PROTO_ACTION_IF_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/tags.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            /// \brief Used to select one grammar or another based on the result
            /// of a compile-time Boolean. When used as a basic_action, \c if_\<\>
            /// selects between two actions based on a compile-time Boolean.
            ///
            /// When <tt>if_\<If,Then,Else\></tt> is used as a grammar, \c If
            /// must be a Proto basic_action and \c Then and \c Else must be grammars.
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
            ///         literal<_>
            ///       , if_< is_integral<_value>() >
            ///     >
            /// {};
            /// \endcode
            ///
            /// When <tt>if_\<If,Then,Else\></tt> is used as a basic_action, \c If,
            /// \c Then and \c Else must be Proto actions. When applying
            /// the basic_action to an expression \c E, state \c S and data \c V,
            /// if <tt>boost::result_of\<when\<_,If\>(E,S,V)\>::type::value</tt>
            /// is \c true then the \c Then basic_action is applied; otherwise
            /// the \c Else basic_action is applied.
            ///
            /// \code
            /// // Match a terminal. If the terminal is integral, return
            /// // mpl::true_; otherwise, return mpl::false_.
            /// struct IsIntegral2
            ///   : when<
            ///         literal<_>
            ///       , if_<
            ///             proto::make(is_integral<_value>())
            ///           , proto::make(std::true_type())
            ///           , proto::make(std::false_type())
            ///         >
            ///     >
            /// {};
            /// \endcode
            namespace detail
            {
                struct _void_
                  : basic_action<_void_>
                {
                    template<typename ...Args>
                    void operator()(Args &&...) const noexcept
                    {}
                };

                template<typename If, typename Then, typename Else = _void_>
                struct _if_
                  : basic_action<_if_<If, Then, Else>>
                {
                    template<typename ...Args>
                    auto operator()(Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        typename std::conditional<
                            eval_bool_action_<If, Args...>::value
                          , as_action_<Then>
                          , as_action_<Else>
                        >::type()(static_cast<Args &&>(args)...)
                    )
                };
            }

            namespace extension
            {
                template<typename If, typename Then>
                struct action_impl<if_(If, Then)>
                  : detail::_if_<If, Then>
                {};

                template<typename If, typename Then, typename Else>
                struct action_impl<if_(If, Then, Else)>
                  : detail::_if_<If, Then, Else>
                {};
            }
        }
    }
}

#endif
