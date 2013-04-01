////////////////////////////////////////////////////////////////////////////////////////////////////
// not.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_NOT_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_NOT_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                struct logical_not_
                {
                    template<typename A>
                    std::integral_constant<bool, !static_cast<bool>(A::value)>
                    operator()(A const &) const noexcept
                    {
                        return std::integral_constant<bool, !static_cast<bool>(A::value)>();
                    }

                    inline bool operator()(bool a) const noexcept
                    {
                        return !a;
                    }
                };

                template<typename BoolAction>
                struct _not_
                  : basic_action<_not_<BoolAction>>
                {
                    template<typename ...Args>
                    auto operator()(Args &&...args) const
                    BOOST_PROTO_AUTO_RETURN(
                        logical_not_()(as_action_<BoolAction>()(static_cast<Args &&>(args)...))
                    )
                };
            }

            namespace extension
            {
                template<typename BoolAction>
                struct action_impl<not_(BoolAction)>
                  : detail::_not_<BoolAction>
                {};
            }
        }
    }
}

#endif
