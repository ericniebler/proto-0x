///////////////////////////////////////////////////////////////////////////////
// let.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;
using proto::_a;
using proto::_b;

///////////////////////////////////////////////////////////////////////////////
// test_let_call_once
//  verify that the transform bound to the local variable is really called
//  only once.
struct once
{
    static int ctors;
    once() { ++ctors; }
};

int once::ctors = 0;

struct LetCallOnce
  : proto::action<
        proto::let(
            _a(proto::construct(once()))
          , proto::functional::make_pair(_a, _a)
        )
    >
{};

void test_let_call_once()
{
    proto::literal<int> i(0);
    std::pair<once, once> p = LetCallOnce()(i);
    BOOST_CHECK_EQUAL(1, once::ctors);
}

///////////////////////////////////////////////////////////////////////////////
// test_let_construct_action
//  verify that let works with object transforms.
struct LetConstructAction
  : proto::action<
        proto::let(
            _a(proto::_int<42>)
          , proto::construct(std::pair<_a, _a>(_a, _a))
        )
    >
{};

void test_let_construct_action()
{
    proto::literal<int> i(0);
    std::pair<int, int> p = LetConstructAction()(i);
    BOOST_CHECK_EQUAL(42, p.first);
    BOOST_CHECK_EQUAL(42, p.second);
}

///////////////////////////////////////////////////////////////////////////////
// test_let_data
//  verify that data doesn't get nuked.
struct MyData
  : proto::_data
{};

struct LetData
  : proto::action<
        proto::let(
            _a(proto::_int<42>)
          , proto::construct(std::pair<MyData, _a>(MyData, _a))
        )
    >
{};

void test_let_data()
{
    std::string hello("hello");
    proto::literal<int> i(0);
    std::pair<std::string, int> p = LetData()(i, 0, proto::data = hello);
    BOOST_CHECK_EQUAL(hello, p.first);
    BOOST_CHECK_EQUAL(42, p.second);
}

///////////////////////////////////////////////////////////////////////////////
// test_let_scope
//  verify that the local variables are scoped properly.
struct LetScope
  : proto::action<
        proto::let(
            _a(proto::_value)
          , proto::functional::make_pair(
                _a
              , proto::let(
                    _a(proto::_state)
                  , proto::functional::make_pair(_a, proto::_data)
                )
            )
        )
    >
{};

void test_let_scope()
{
    std::string hello("hello");
    proto::literal<short> i((short)42);
    std::pair<short, std::pair<float, std::string> > p = LetScope()(i, 3.14f, proto::data = hello);
    BOOST_CHECK_EQUAL(42, p.first);
    BOOST_CHECK_EQUAL(3.14f, p.second.first);
    BOOST_CHECK_EQUAL(hello, p.second.second);
}

///////////////////////////////////////////////////////////////////////////////
// test_let_scope2
//  verify that the local variables are scoped properly.
struct LetScope2
  : proto::action<
        proto::let(
            _a(proto::_value)
          , proto::functional::make_pair(
                _a
              , proto::let(
                    _b(proto::_state)
                  , proto::functional::make_pair(_a, _b)
                )
            )
        )
    >
{};

void test_let_scope2()
{
    char const * sz = "";
    proto::literal<short> i((short)42);
    std::pair<short, std::pair<short, float> > p2 = LetScope2()(i, 3.14f, proto::data = sz);
    BOOST_CHECK_EQUAL(42, p2.first);
    BOOST_CHECK_EQUAL(42, p2.second.first);
    BOOST_CHECK_EQUAL(3.14f, p2.second.second);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test let basic action");

    test->add(BOOST_TEST_CASE(&test_let_call_once));
    test->add(BOOST_TEST_CASE(&test_let_construct_action));
    test->add(BOOST_TEST_CASE(&test_let_data));
    test->add(BOOST_TEST_CASE(&test_let_scope));
    test->add(BOOST_TEST_CASE(&test_let_scope2));

    return test;
}
