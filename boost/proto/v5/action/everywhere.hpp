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
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/match.hpp>
#include <boost/proto/v5/action/passthru.hpp>
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
                // _replace_if_
                template<typename ...Cases>
                struct _replace_if_
                  : def<
                        match(
                            case_...(
                                as_grammar_<Cases>
                              , as_action_<Cases>
                            )
                          , default_(
                                return_(_) // avoid returning an rvalue ref to a temporary
                            )
                        )
                    >
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // _everywhere_
                template<typename ...Cases>
                struct _everywhere_
                  : def<
                        match(
                            case_(
                                terminal(_)
                              , _replace_if_<Cases...>
                            )
                          , default_(
                                _replace_if_<Cases...>(
                                    passthru(_everywhere_<Cases...>...)
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
                ////////////////////////////////////////////////////////////////////////////////////
                // e.g. everywhere(case_(plus(_,_), minus(_left,_right)))
                template<typename ...Cases>
                struct action_impl<everywhere(Cases...)>
                  : detail::_everywhere_<Cases...>
                {};
            }
        }
    }
}

#endif
