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
                using return_ = utility::identity;

                ////////////////////////////////////////////////////////////////////////////////////
                // as_case_
                template<typename ActiveGrammar>
                struct as_case_
                {
                    using type = case_(ActiveGrammar, ActiveGrammar);
                };

                template<typename Grammar, typename ...Actions>
                struct as_case_<case_(*)(Grammar, Actions...)>
                {
                    using type = case_(*)(Grammar, Actions...);
                };

                template<typename R, typename ...As>
                struct as_case_<R(*)(As...)>
                {
                    static_assert(
                        utility::never<R(As...)>::value
                      , "In everything(...), expected either a case_(...) statement or an "
                        "active grammar."
                    );
                    using type = R(*)(As...);
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // substitute_if_
                template<typename ...Cases>
                struct substitute_if_
                  : def<
                        match(
                            typename as_case_<Cases>::type...
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
                            case_( terminal(_),
                                substitute_if_<Cases...>
                            )
                          , default_(
                                substitute_if_<Cases...>(
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
