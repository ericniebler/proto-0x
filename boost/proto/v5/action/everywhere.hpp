////////////////////////////////////////////////////////////////////////////////////////////////////
// everywhere.hpp
//
// Copyright 2013 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_EVERYWHERE_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_EVERYWHERE_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/def.hpp>
#include <boost/proto/v5/action/match.hpp>
#include <boost/proto/v5/action/pass.hpp>
#include <boost/proto/v5/grammar/expr.hpp>
#include <boost/proto/v5/grammar/placeholders.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // return_
                struct return_
                {
                    template<typename T>
                    constexpr T &operator()(T &t) const noexcept
                    {
                        return t;
                    }

                    template<typename T>
                    constexpr auto operator()(T &&t) const
                    BOOST_PROTO_AUTO_RETURN(
                        T(static_cast<T &&>(t))
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // everywhere
                template<typename ...Cases>
                struct apply_if_
                  : def<
                        match(
                            Cases...
                          , default_(
                                return_(_) // avoid returning an rvalue ref to a temporary
                            )
                        )
                    >
                {};

                template<typename ...Cases>
                struct _everywhere_
                  : def<
                        match(
                            case_( terminal(_),
                                apply_if_<Cases...>
                            )
                          , default_(
                                apply_if_<Cases...>(
                                    pass(_everywhere_<Cases...>...)
                                )
                            )
                        )
                    >
                {};
            }

            struct everywhere
            {};

            namespace extension
            {
                template<typename ...Cases>
                struct action_impl<everywhere(Cases...)>
                  : detail::_everywhere_<Cases...>
                {};
            }
        }
    }
}

#endif
