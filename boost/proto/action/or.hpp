///////////////////////////////////////////////////////////////////////////////
// or.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_OR_HPP_INCLUDED
#define BOOST_PROTO_ACTION_OR_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/grammar/or.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        /// \brief For matching one of a set of alternate grammars. Alternates
        /// tried in order to avoid ambiguity. When used as a basic_action, \c or_\<\>
        /// applies the basic_action associated with the first grammar that matches
        /// the expression.
        ///
        /// An expression type \c E matches <tt>or_\<B0,B1,...Bn\></tt> if \c E
        /// matches any \c Bx for \c x in <tt>[0,n)</tt>.
        ///
        /// When applying <tt>or_\<B0,B1,...Bn\></tt> as a basic_action with an
        /// expression \c e of type \c E, state \c s and data \c d, it is
        /// equivalent to <tt>Bx()(e, s, d)</tt>, where \c x is the lowest
        /// number such that <tt>matches\<E,Bx\>::value</tt> is \c true.
        namespace detail
        {
            template<typename... Grammars>
            struct _or_
              : basic_action<_or_<Grammars...>>
            {
                template<typename Expr, typename ...Rest>
                auto operator()(Expr && e, Rest &&... rest) const
                BOOST_PROTO_AUTO_RETURN(
                    action<
                        typename grammar_of<
                            typename matches<Expr, proto::or_(Grammars...)>::which
                        >::type
                    >()(
                        static_cast<Expr &&>(e)
                      , static_cast<Rest &&>(rest)...
                    )
                )
            };
        }

        template<typename...Actions>
        struct action<or_(Actions...)>
          : detail::_or_<Actions...>
        {};
    }
}

#endif
