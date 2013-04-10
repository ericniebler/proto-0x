////////////////////////////////////////////////////////////////////////////////////////////////////
// mem_fun.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <functional>
#include <boost/proto/v5/proto.hpp>
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
    proto::literal<std::plus<int>> plus;
    int i = proto::_eval<>()(plus(1,2));
    BOOST_CHECK_EQUAL(i, 3);

    S s{42};
    std::auto_ptr<S> ps(new S{69});
    proto::literal<int S::*> pm{&S::m};
    proto::literal<void (S::*)()> pf{&S::foo};
    proto::literal<int (S::*)(int)> pb{&S::bar};

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

// other miscelaneous test cases
void test_mem_fun_2()
{
    S s{43};
    S *ps = &s;
    int &j = proto::mem_ptr()(&s, &S::m);
    int &j2 = proto::mem_ptr()(ps, &S::m);
    BOOST_CHECK_EQUAL(j, 43);
    BOOST_CHECK_EQUAL(j2, 43);

    int k = proto::mem_ptr()(&s, &S::bar)(42);
    int k2 = proto::mem_ptr()(ps, &S::bar)(42);
    BOOST_CHECK_EQUAL(k, 42);
    BOOST_CHECK_EQUAL(k2, 42);

    proto::literal<int> i;
    proto::expr<proto::mem_ptr(
        proto::expr<proto::terminal(int)> &
      , proto::terminal(int S::*)
    )> x = proto::mem_ptr()(i, &S::m);
    
    int S::*pm = &S::m;
    proto::expr<proto::mem_ptr(
        proto::expr<proto::terminal(int)> &
      , proto::terminal(int S::*&)
    )> x2 = proto::mem_ptr()(i, pm);

    auto x3 = proto::mem_ptr()(i, &S::foo);
    static_assert(proto::is_expr<decltype(x3)>::value, "");

    BOOST_PROTO_IGNORE_UNUSED(x, x2, x3);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("tests for eval with functions and member pointer ops");

    test->add(BOOST_TEST_CASE(&test_mem_fun));
    test->add(BOOST_TEST_CASE(&test_mem_fun_2));

    return test;
}
