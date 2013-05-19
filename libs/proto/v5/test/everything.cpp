////////////////////////////////////////////////////////////////////////////////////////////////////
// everything.cpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;

struct map_list_of_
{};

constexpr proto::expr<proto::terminal(map_list_of_)> map_list_of {};

namespace Impl
{
    using namespace proto;
    struct Sum
      : def<
            everything(
                plus
              , match( case_(terminal(int), _value)
                     , case_(terminal(map_list_of_), _int<100>)
                )
            )
        >
    {};
}
using Impl::Sum;

void test_everything()
{
    int sum = Sum()(map_list_of(1,2)(2,3)(3,4)(4,5)(5,6));
    BOOST_CHECK_EQUAL(sum, 135);

    using namespace proto::literals;
    proto::nothing nil_ = Sum()("hello"_et + 3.14);
    BOOST_PROTO_IGNORE_UNUSED(nil_);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("testing the everything action");

    test->add(BOOST_TEST_CASE(&test_everything));

    return test;
}
