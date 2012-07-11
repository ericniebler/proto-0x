///////////////////////////////////////////////////////////////////////////////
// mem_fun.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <functional>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

struct S
{
    int m;

    void foo()
    {}

    int bar(int i)
    {
        return i;
    }
};

void test_mem_fun()
{
    proto::terminal<std::plus<int>> plus;
    int i = proto::_eval<>()(plus(1,2));
    BOOST_CHECK_EQUAL(i, 3);

    S s{42};
    std::auto_ptr<S> ps(new S{69});
    proto::terminal<int S::*> pm{&S::m};
    proto::terminal<void (S::*)()> pf{&S::foo};
    proto::terminal<int (S::*)(int)> pb{&S::bar};

    i = proto::_eval<>()(pm(&s));
    BOOST_CHECK_EQUAL(i, 42);

    i = proto::_eval<>()(pm(s));
    BOOST_CHECK_EQUAL(i, 42);

    i = proto::_eval<>()(pm(ps));
    BOOST_CHECK_EQUAL(i, 69);

    proto::_eval<>()(pf(&s));
    proto::_eval<>()(pf(s));
    proto::_eval<>()(pf(ps));

    i = proto::_eval<>()(pb(&s, 1));
    BOOST_CHECK_EQUAL(i, 1);

    i = proto::_eval<>()(pb(s, 2));
    BOOST_CHECK_EQUAL(i, 2);

    i = proto::_eval<>()(pb(ps, 3));
    BOOST_CHECK_EQUAL(i, 3);

    i = proto::_eval<>()(&s ->* pm);
    BOOST_CHECK_EQUAL(i, 42);

    i = proto::_eval<>()(s ->* pm);
    BOOST_CHECK_EQUAL(i, 42);

    i = proto::_eval<>()(ps ->* pm);
    BOOST_CHECK_EQUAL(i, 69);

    proto::_eval<>()((&s ->* pf)());
    proto::_eval<>()((s ->* pf)());
    proto::_eval<>()((ps ->* pf)());

    i = proto::_eval<>()((&s ->* pb)(1));
    BOOST_CHECK_EQUAL(i, 1);

    i = proto::_eval<>()((s ->* pb)(2));
    BOOST_CHECK_EQUAL(i, 2);

    i = proto::_eval<>()((ps ->* pb)(3));
    BOOST_CHECK_EQUAL(i, 3);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("tests for eval with functions and member pointer ops");

    test->add(BOOST_TEST_CASE(&test_mem_fun));

    return test;
}
