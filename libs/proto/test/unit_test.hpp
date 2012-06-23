///////////////////////////////////////////////////////////////////////////////
// unit_test.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TEST_UNIT_TEST_HPP_INCLUDED
#define BOOST_PROTO_TEST_UNIT_TEST_HPP_INCLUDED

#include <boost/test/unit_test.hpp>

namespace detail
{
    inline void ignore_unit_test()
    {
        BOOST_PROTO_IGNORE_UNUSED(boost::test_tools::check_is_close);
        BOOST_PROTO_IGNORE_UNUSED(boost::test_tools::check_is_small);
    }
}

#endif
