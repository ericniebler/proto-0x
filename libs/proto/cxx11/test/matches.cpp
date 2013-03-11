////////////////////////////////////////////////////////////////////////////////////////////////////
// matches.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <boost/proto/cxx11/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

using int_ = proto::literal<int>;
using string_ = proto::literal<std::string>;
struct MyIntWrap : int_ {};

void test_matches()
{
    static_assert(proto::matches<int_, proto::terminal(int)>::value, "");
    static_assert(proto::matches<MyIntWrap, proto::terminal(int)>::value, "");
    static_assert(proto::matches<MyIntWrap &, proto::terminal(int)>::value, "");
    static_assert(!proto::matches<int_, proto::unary_plus(_)>::value, "");
    static_assert(!proto::matches<int_, proto::terminal(std::string)>::value, "");
    static_assert(proto::matches<proto::exprs::function<int_, int_, int_>, proto::function(proto::terminal(int)...)>::value, "");
    static_assert(proto::matches<proto::exprs::function<int_>, proto::function(proto::terminal(int), proto::terminal(std::string)...)>::value, "");
    static_assert(proto::matches<proto::exprs::function<int_>, proto::function(proto::terminal(int)...)>::value, "");
    static_assert(!proto::matches<proto::exprs::function<int_, int_, string_>, proto::function(proto::terminal(int)...)>::value, "");
    static_assert(!proto::matches<int_, proto::function(proto::terminal(int), proto::terminal(int))>::value, "");
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test the matches metafunction");

    test->add(BOOST_TEST_CASE(&test_matches));

    return test;
}
