////////////////////////////////////////////////////////////////////////////////////////////////////
// display_expr.cpp
//
//  Copyright 2010 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/proto/v5/proto.hpp>
#include <boost/proto/v5/debug.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

struct A {};
struct B : A {};
std::ostream& operator<<( std::ostream& out, const A& ) { return out << "this is A!"; }

struct C {};

void test_display_expr()
{
    // https://svn.boost.org/trac/boost/ticket/4910
    proto::literal<int> i{0};
    std::string intname = proto::detail::name_of<int>();

    {
        std::stringstream sout;
        proto::display_expr(i + A(), sout);
        std::string Aname = proto::detail::name_of<A>();
        BOOST_CHECK_EQUAL(sout.str(), std::string(
          "plus(\n"
          "    terminal( (int) 0 ) &\n"
          "  , terminal( (" + Aname + ") this is A! )\n"
          ")\n"));
    }

    {
        std::stringstream sout;
        proto::display_expr(i + B(), sout);
        std::string Bname = proto::detail::name_of<B>();
        BOOST_CHECK_EQUAL(sout.str(), std::string(
          "plus(\n"
          "    terminal( (" + intname + ") 0 ) &\n"
          "  , terminal( (" + Bname + ") this is A! )\n"
          ")\n"));
    }

    {
        std::stringstream sout;
        std::string Cname = proto::detail::name_of<C>();
        proto::display_expr(i + C(), sout);
        BOOST_CHECK_EQUAL(sout.str(), std::string(
          "plus(\n"
          "    terminal( (" + intname + ") 0 ) &\n"
          "  , terminal( (" + Cname + ") " + Cname + " )\n"
          ")\n"));
    }
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test display_expr() function");
    test->add(BOOST_TEST_CASE(&test_display_expr));
    return test;
}
