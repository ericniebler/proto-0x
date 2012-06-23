///////////////////////////////////////////////////////////////////////////////
// matches.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

typedef proto::terminal<int> int_;
typedef proto::terminal<std::string> string_;
struct MyIntWrap : int_ {};

void test_matches()
{
    static_assert(proto::matches<int_, int_>::value, "");
    static_assert(proto::matches<MyIntWrap, int_>::value, "");
    static_assert(proto::matches<MyIntWrap &, int_>::value, "");
    static_assert(proto::matches<MyIntWrap &, MyIntWrap>::value, "");
    static_assert(!proto::matches<int_, proto::unary_plus<_>>::value, "");
    static_assert(!proto::matches<int_, string_>::value, "");
    static_assert(proto::matches<proto::function<int_, int_, int_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(proto::matches<proto::function<int_>, proto::function<int_, proto::vararg<string_>>>::value, "");
    static_assert(proto::matches<proto::function<int_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(!proto::matches<proto::function<int_, int_, string_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(!proto::matches<int_, proto::function<int_, int_>>::value, "");
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test the matches metafunction");

    test->add(BOOST_TEST_CASE(&test_matches));

    return test;
}
