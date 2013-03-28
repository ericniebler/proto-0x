////////////////////////////////////////////////////////////////////////////////////////////////////
// protect.hpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"
namespace proto=boost::proto;
using proto::_;

struct MinusToPlus
  : proto::def<
        proto::match(
            proto::case_(
                proto::minus(MinusToPlus, MinusToPlus)
              , proto::plus(MinusToPlus(proto::_left), MinusToPlus(proto::_right))
            )
          , proto::case_(
                _(MinusToPlus...)
              , proto::pass
            )
          , proto::case_(
                proto::terminal(_)
              , proto::pass
            )
        )
    >
{};

void test_pass()
{
    using namespace proto::literals;
    auto x = - (1_et - 2);
    proto::expr<
        proto::negate(
            proto::plus(
                proto::terminal(unsigned long long)
              , proto::terminal(int)
            )
        )
    > y = MinusToPlus()(x);
    BOOST_PROTO_IGNORE_UNUSED(y);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::_pass");

    test->add(BOOST_TEST_CASE(&test_pass));

    return test;
}
