////////////////////////////////////////////////////////////////////////////////////////////////////
// block.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_BLOCK_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_BLOCK_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            /// \brief For matching all of a set of grammars. When used as a
            /// basic_action, \c and_\<\> applies the actions associated with
            /// the each grammar in the set, and returns the result of the last.
            ///
            /// An expression type \c E matches <tt>and_\<B0,B1,...Bn\></tt> if \c E
            /// matches all \c Bx for \c x in <tt>[0,n)</tt>.
            ///
            /// When applying <tt>and_\<B0,B1,...Bn\></tt> as a basic_action with an
            /// expression \c e, state \c s and data \c d, it is
            /// equivalent to <tt>(B0()(e, s, d),B1()(e, s, d),...Bn()(e, s, d))</tt>.
            namespace detail
            {
                template<typename... Actions>
                struct _block
                  : basic_action<_block<Actions...>>
                {
                    template<typename ...Args>
                    auto operator()(Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(utility::functional::back())(
                            (as_action_<Actions>()(static_cast<Args &&>(args)...), utility::void_)...
                        )
                    )
                };
            }

            struct block
            {};

            namespace extension
            {
                template<typename... Actions>
                struct action_impl<block(Actions...)>
                  : detail::_block<Actions...>
                {};
            }
        }
    }
}

#endif
