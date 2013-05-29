////////////////////////////////////////////////////////////////////////////////////////////////////
// external.hpp
// Contains definition of external actions.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_EXTERNAL_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_EXTERNAL_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/action/apply.hpp>
#include <boost/proto/v5/action/env.hpp>
#include <boost/proto/v5/action/call.hpp>
#include <boost/proto/v5/functional/cxx/construct.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            struct external
            {};

            namespace extension
            {
                template<typename RuleName>
                struct action_impl<case_(RuleName, external)>
                  : def<apply(get_env(call(functional::cxx::construct<RuleName>())))>
                {};
            }
        }
    }
}

#endif
