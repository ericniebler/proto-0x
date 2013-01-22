////////////////////////////////////////////////////////////////////////////////////////////////////
// logical_ops.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

using int_ = proto::literal<int>;
using cttrue_ = proto::make(std::true_type());
using ctfalse_ = proto::make(std::false_type());
using true_ = proto::_true;
using false_ = proto::_false;

namespace
{
    constexpr auto const & _0 = proto::utility::static_const<int_>::value;
}

void test_and()
{
    // strictly run-time boolean logic tests
    {
        auto r0 = proto::def<proto::and_(true_, true_)>()(_0);
        static_assert(std::is_same<decltype(r0), bool>::value, "");
        BOOST_CHECK(r0);

        auto r1 = proto::def<proto::and_(true_, false_)>()(_0);
        static_assert(std::is_same<decltype(r1), bool>::value, "");
        BOOST_CHECK(!r1);

        auto r2 = proto::def<proto::and_(false_, true_)>()(_0);
        static_assert(std::is_same<decltype(r2), bool>::value, "");
        BOOST_CHECK(!r2);

        auto r3 = proto::def<proto::and_(false_, false_)>()(_0);
        static_assert(std::is_same<decltype(r3), bool>::value, "");
        BOOST_CHECK(!r3);
    }

    // strictly compile-time boolean logic tests
    {
        std::true_type r0 = proto::def<proto::and_(cttrue_, cttrue_)>()(_0);

        std::false_type r1 = proto::def<proto::and_(cttrue_, ctfalse_)>()(_0);

        std::false_type r2 = proto::def<proto::and_(ctfalse_, cttrue_)>()(_0);

        std::false_type r3 = proto::def<proto::and_(ctfalse_, ctfalse_)>()(_0);
 
        BOOST_PROTO_IGNORE_UNUSED(r0, r1, r2, r3);
    }

    // mixed run-time/compile-time boolean logic tests
    {
        auto r0 = proto::def<proto::and_(cttrue_, true_)>()(_0);
        static_assert(std::is_same<decltype(r0), bool>::value, "");
        BOOST_CHECK(r0);

        auto r1 = proto::def<proto::and_(cttrue_, false_)>()(_0);
        static_assert(std::is_same<decltype(r1), bool>::value, "");
        BOOST_CHECK(!r1);

        auto r2 = proto::def<proto::and_(ctfalse_, true_)>()(_0);
        static_assert(std::is_same<decltype(r2), bool>::value, "");
        BOOST_CHECK(!r2);

        auto r3 = proto::def<proto::and_(ctfalse_, false_)>()(_0);
        static_assert(std::is_same<decltype(r3), bool>::value, "");
        BOOST_CHECK(!r3);

        auto r4 = proto::def<proto::and_(true_, cttrue_)>()(_0);
        static_assert(std::is_same<decltype(r4), bool>::value, "");
        BOOST_CHECK(r4);

        auto r5 = proto::def<proto::and_(true_, ctfalse_)>()(_0);
        static_assert(std::is_same<decltype(r5), bool>::value, "");
        BOOST_CHECK(!r5);

        auto r6 = proto::def<proto::and_(false_, cttrue_)>()(_0);
        static_assert(std::is_same<decltype(r6), bool>::value, "");
        BOOST_CHECK(!r6);

        auto r7 = proto::def<proto::and_(false_, ctfalse_)>()(_0);
        static_assert(std::is_same<decltype(r7), bool>::value, "");
        BOOST_CHECK(!r7);
    }

    // A nice, long, purely compile-time computation
    std::false_type r0 = proto::def<proto::and_(cttrue_ , cttrue_ , cttrue_ , ctfalse_, cttrue_ )>()(_0);
    BOOST_PROTO_IGNORE_UNUSED(r0);

    // A nice, long, mixed runtime/compile-time computation
    auto r1 = proto::def<proto::and_(cttrue_ , cttrue_ , cttrue_ , false_, cttrue_ )>()(_0);
    static_assert(std::is_same<decltype(r1), bool>::value, "");
    BOOST_CHECK(!r1);
}

void test_or()
{
    // strictly run-time boolean logic tests
    {
        auto r0 = proto::def<proto::or_(true_, true_)>()(_0);
        static_assert(std::is_same<decltype(r0), bool>::value, "");
        BOOST_CHECK(r0);

        auto r1 = proto::def<proto::or_(true_, false_)>()(_0);
        static_assert(std::is_same<decltype(r1), bool>::value, "");
        BOOST_CHECK(r1);

        auto r2 = proto::def<proto::or_(false_, true_)>()(_0);
        static_assert(std::is_same<decltype(r2), bool>::value, "");
        BOOST_CHECK(r2);

        auto r3 = proto::def<proto::or_(false_, false_)>()(_0);
        static_assert(std::is_same<decltype(r3), bool>::value, "");
        BOOST_CHECK(!r3);
    }

    // strictly compile-time boolean logic tests
    {
        std::true_type r0 = proto::def<proto::or_(cttrue_, cttrue_)>()(_0);

        std::true_type r1 = proto::def<proto::or_(cttrue_, ctfalse_)>()(_0);

        std::true_type r2 = proto::def<proto::or_(ctfalse_, cttrue_)>()(_0);

        std::false_type r3 = proto::def<proto::or_(ctfalse_, ctfalse_)>()(_0);
 
        BOOST_PROTO_IGNORE_UNUSED(r0, r1, r2, r3);
    }

    // mixed run-time/compile-time boolean logic tests
    {
        auto r0 = proto::def<proto::or_(cttrue_, true_)>()(_0);
        static_assert(std::is_same<decltype(r0), bool>::value, "");
        BOOST_CHECK(r0);

        auto r1 = proto::def<proto::or_(cttrue_, false_)>()(_0);
        static_assert(std::is_same<decltype(r1), bool>::value, "");
        BOOST_CHECK(r1);

        auto r2 = proto::def<proto::or_(ctfalse_, true_)>()(_0);
        static_assert(std::is_same<decltype(r2), bool>::value, "");
        BOOST_CHECK(r2);

        auto r3 = proto::def<proto::or_(ctfalse_, false_)>()(_0);
        static_assert(std::is_same<decltype(r3), bool>::value, "");
        BOOST_CHECK(!r3);

        auto r4 = proto::def<proto::or_(true_, cttrue_)>()(_0);
        static_assert(std::is_same<decltype(r4), bool>::value, "");
        BOOST_CHECK(r4);

        auto r5 = proto::def<proto::or_(true_, ctfalse_)>()(_0);
        static_assert(std::is_same<decltype(r5), bool>::value, "");
        BOOST_CHECK(r5);

        auto r6 = proto::def<proto::or_(false_, cttrue_)>()(_0);
        static_assert(std::is_same<decltype(r6), bool>::value, "");
        BOOST_CHECK(r6);

        auto r7 = proto::def<proto::or_(false_, ctfalse_)>()(_0);
        static_assert(std::is_same<decltype(r7), bool>::value, "");
        BOOST_CHECK(!r7);
    }

    // A nice, long, purely compile-time computation
    std::true_type r0 = proto::def<proto::or_(ctfalse_, ctfalse_, ctfalse_, cttrue_, ctfalse_)>()(_0);
    BOOST_PROTO_IGNORE_UNUSED(r0);

    // A nice, long, mixed runtime/compile-time computation
    auto r1 = proto::def<proto::or_(ctfalse_, ctfalse_, ctfalse_, true_, ctfalse_)>()(_0);
    static_assert(std::is_same<decltype(r1), bool>::value, "");
    BOOST_CHECK(r1);
}

// Negating a bool should yield a bool. Negating a std::false_type should yield
// a std::true_type, and vice versa.
void test_not()
{
    auto x = proto::def<proto::not_(true_)>()(_0);
    static_assert(std::is_same<decltype(x), bool>::value, "");
    BOOST_CHECK(!x);

    std::false_type y = proto::def<proto::not_(cttrue_)>()(_0);

    std::true_type z = proto::def<proto::not_(ctfalse_)>()(_0);

    BOOST_PROTO_IGNORE_UNUSED(x, y, z);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("basic tests for the logical operators");

    test->add(BOOST_TEST_CASE(&test_and));
    test->add(BOOST_TEST_CASE(&test_or));
    test->add(BOOST_TEST_CASE(&test_not));

    return test;
}
