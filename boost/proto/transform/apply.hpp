///////////////////////////////////////////////////////////////////////////////
// apply.hpp
// Treat the first argument as a callable and the others as arguments to the
// callable
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_APPLY_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_APPLY_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>
#include <boost/proto/transform/call.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Fun, typename ...Tfxs>
            struct _apply
              : transform<_apply<Fun, Tfxs...>>
            {
                template<typename ...Args>
                auto operator()(Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::call_1_<Tfxs...>()(
                        as_transform<Fun>()(static_cast<Args &&>(args)...)
                      , static_cast<Args &&>(args)...
                    )
                )
            };
        }

        template<typename Fun, typename ...Tfxs, int I>
        struct as_transform<apply(Fun, Tfxs...), I>
          : detail::_apply<Fun, Tfxs...>
        {};
    }
}

#endif
