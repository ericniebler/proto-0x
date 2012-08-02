///////////////////////////////////////////////////////////////////////////////
// protect.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"
namespace proto=boost::proto;
using proto::_;

template<typename T>
struct S
{};

template<typename T>
struct identity
{
    typedef T type;
};

struct Test
  : proto::action<proto::_protect<_>()>
{};

struct Test0
  : proto::action<proto::construct(proto::_protect<_>())>
{};

struct Test1
  : proto::action<proto::construct(identity<proto::_protect<_>>())>
{};

struct Test2
  : proto::action<proto::construct(S<proto::_protect<_>>())>
{};

struct Test3
  : proto::action<proto::construct(identity<proto::_protect<identity<_>()>>())>
{};

struct Test4
  : proto::action<proto::construct(S<proto::_protect<S<_>()>>())>
{};

struct Test5
  : proto::action<proto::construct(identity<proto::_protect<identity<identity<_>>()>>())>
{};

void test_protect()
{
    proto::terminal<int> i {42};

    proto::terminal<int> & t = Test()(i);
    proto::_protect<_> t0 = Test0()(i);
    identity<proto::_protect<_>> t1 = Test1()(i);
    S<proto::_protect<_>> t2 = Test2()(i);
    identity<proto::_protect<identity<_>()>> t3 = Test3()(i);
    S<proto::_protect<S<_>()>> t4 = Test4()(i);
    identity<proto::_protect<identity<identity<_>>()>> t5 = Test5()(i);

    BOOST_PROTO_IGNORE_UNUSED(t, t0, t1, t2, t3, t4, t5);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::_protect");

    test->add(BOOST_TEST_CASE(&test_protect));

    return test;
}
