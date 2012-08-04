///////////////////////////////////////////////////////////////////////////////
// switch.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_SWITCH_HPP_INCLUDED
#define BOOST_PROTO_ACTION_SWITCH_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        /// \brief For matching one of a set of alternate grammars, which
        /// are looked up based on some property of an expression. The
        /// property on which to dispatch is specified by the \c Action
        /// template parameter, which defaults to <tt>tag_of\<_\>()</tt>.
        /// That is, when the \c Trannsform is not specified, the alternate
        /// grammar is looked up using the tag type of the current expression.
        ///
        /// When used as a basic_action, \c switch_\<\> applies the basic_action
        /// associated with the grammar that matches the expression.
        ///
        /// \note \c switch_\<\> is functionally identical to \c or_\<\> but
        /// is often more efficient. It does a fast, O(1) lookup using the
        /// result of the specified basic_action to find a sub-grammar that may
        /// potentially match the expression.
        ///
        /// An expression type \c E matches <tt>switch_\<C,T\></tt> if \c E
        /// matches <tt>C::case_\<std::result_of\<T(E)\>::type\></tt>.
        ///
        /// When applying <tt>switch_\<C,T\></tt> as a basic_action with an
        /// expression \c e of type \c E, state \c s of type \S and data
        /// \c d of type \c D, it is equivalent to
        /// <tt>C::case_\<std::result_of\<T(E,S,D)\>::type\>()(e, s, d)</tt>.
        namespace detail
        {
            template<typename Cases, typename Action>
            struct _switch_
              : basic_action<_switch_<Cases, Action>>
            {
                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    action<
                        typename Cases::template case_<
                            decltype(action<Action>()(static_cast<Args &&>(args)...))
                        >
                    >()(static_cast<Args &&>(args)...)
                )
            };

            /// INTERNAL ONLY
            // pure compile-time optimization
            template<typename Cases>
            struct _switch_<Cases, _tag_of>
              : basic_action<_switch_<Cases, _tag_of>>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && e, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<
                        typename Cases::template case_<
                            typename proto::tag_of<Expr>::type
                        >
                    >()(static_cast<Expr &&>(e), static_cast<Rest &&>(rest)...)
                )
            };
        }

        template<typename Cases>
        struct action<switch_(Cases)>
          : detail::_switch_<Cases, _tag_of>
        {};

        template<typename Cases, typename Action>
        struct action<switch_(Cases, Action)>
          : detail::_switch_<Cases, Action>
        {};
    }
}

#endif
