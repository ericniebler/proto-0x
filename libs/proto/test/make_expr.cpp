////////////////////////////////////////////////////////////////////////////////////////////////////
// make_expr.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/proto.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/fusion/tuple.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

template<typename E>
struct ewrap;

struct mydomain
  : proto::domain<mydomain>
{
    struct make_expr
      : proto::make_custom_expr<ewrap>
    {};
};

template<typename E>
struct ewrap
  : proto::expr<E, mydomain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(ewrap);
    using proto_basic_expr_type = proto::expr<E, mydomain>;
    BOOST_PROTO_INHERIT_EXPR_CTORS(ewrap, proto_basic_expr_type);
};

void test_make_expr()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::make_expr(proto::terminal(), 1);
    proto::expr<proto::terminal(int &)> t2 = proto::make_expr(proto::terminal(), i);
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int)>)> p1 = proto::make_expr(proto::unary_plus(), 1);
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int &)>)> p2 = proto::make_expr(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::domains::make_expr<mydomain>(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::make_expr(proto::plus(), p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_make_expr_functional()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::make_expr()(proto::terminal(), 1);
    proto::expr<proto::terminal(int &)> t2 = proto::functional::make_expr()(proto::terminal(), i);
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int)>)> p1 = proto::functional::make_expr()(proto::unary_plus(), 1);
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int &)>)> p2 = proto::functional::make_expr()(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::domains::functional::make_expr<mydomain>()(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::make_expr()(proto::plus(), p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::unpack_expr(proto::terminal(), fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::unpack_expr(proto::terminal(), fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int)>)> p1 = proto::unpack_expr(proto::unary_plus(), fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int &)>)> p2 = proto::unpack_expr(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::domains::unpack_expr<mydomain>(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::unpack_expr(proto::plus(), fusion::make_tuple(boost::ref(p3), 0));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr_functional()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::unpack_expr()(proto::terminal(), fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::functional::unpack_expr()(proto::terminal(), fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int)>)> p1 = proto::functional::unpack_expr()(proto::unary_plus(), fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::expr<proto::terminal(int &)>)> p2 = proto::functional::unpack_expr()(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::domains::functional::unpack_expr<mydomain>()(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::unpack_expr()(proto::plus(), fusion::make_tuple(boost::ref(p3), 0));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

// Turn all terminals held by reference into ones held by value
struct ByVal
  : proto::def<
        proto::match(
            proto::case_(
                proto::terminal(proto::_)
              , proto::terminal(proto::utility::by_val(proto::_value))
            )
          , proto::case_(
                proto::_(ByVal...)
              , proto::pass
            )
        )
    >
{};

// Turn all terminals held by value into ones held by reference (not safe in general)
struct ByRef
  : proto::def<
        proto::match(
            proto::case_(
                proto::terminal(proto::_)
              , proto::terminal(proto::utility::by_ref(proto::_value))
            )
          , proto::case_(
                proto::_(ByRef...)
              , proto::pass
            )
        )
    >
{};

// turn all proto::plus nodes to minus nodes:
struct Minus
  : proto::def<
        proto::match(
            proto::case_(
                proto::terminal(proto::_)
              , proto::pass
            )
          , proto::case_(
                proto::plus(Minus, Minus)
              , proto::minus(Minus(proto::_left), Minus(proto::_right))
            )
        )
    >
{};

struct Square
  : proto::def<
        proto::match(
            proto::case_(
                proto::terminal(proto::_)
              , proto::multiplies(proto::_, proto::_)
            )
          , proto::case_(
                proto::plus(Square, Square)
              , proto::pass
            )
        )
    >
{};

void test_make_expr_transform()
{
    proto::expr<
        proto::plus(
            proto::expr<proto::terminal(int)>
          , proto::expr<proto::terminal(int)>
        )
    > t1 = ByVal()(proto::as_expr(1) + 1);

    proto::expr<
        proto::plus(
            proto::expr<proto::terminal(int &)> // BUGBUG why isn't this int const &
          , proto::expr<proto::terminal(int &)>
        )
    > t2 = ByRef()(proto::as_expr(1) + 1);

    proto::expr<
        proto::minus(
            proto::expr<proto::terminal(int)>
          , proto::expr<proto::terminal(int)>
        )
    > t3 = Minus()(proto::as_expr(1) + 1);

    proto::expr<
        proto::plus(
            proto::expr<proto::multiplies(proto::expr<proto::terminal(int)>, proto::expr<proto::terminal(int)>)>    
          , proto::expr<proto::multiplies(proto::expr<proto::terminal(int)>, proto::expr<proto::terminal(int)>)>    
        )
    > t4 = Square()(proto::as_expr(1) + 1);

    BOOST_PROTO_IGNORE_UNUSED(t1, t2, t3, t4);
}

struct length_impl {};
struct dot_impl {};

constexpr proto::expr<proto::terminal(length_impl)> length{};
constexpr proto::expr<proto::terminal(dot_impl)> dot{};

// convert length(a) < length(b) to dot(a,a) < dot(b,b)
struct Convert
  : proto::def<
        proto::match(
            proto::case_(
                proto::less(
                    proto::function(proto::terminal(length_impl), proto::_)
                  , proto::function(proto::terminal(length_impl), proto::_)
                )
              , proto::less(
                    proto::function(
                        proto::terminal(proto::make(dot_impl()))
                      , proto::_child<1>(proto::_child<0>)
                      , proto::_child<1>(proto::_child<0>)
                    )
                  , proto::function(
                        proto::terminal(proto::make(dot_impl()))
                      , proto::_child<1>(proto::_child<1>)
                      , proto::_child<1>(proto::_child<1>)
                    )
                )
            )
        )
    >
{};

void test_make_expr_transform2()
{
    auto expr = length(1) < length(2);
    void const *addr1 = boost::addressof(proto::child<1>(proto::child<0>(expr)));
    void const *addr2 = boost::addressof(proto::child<1>(proto::child<0>(Convert()(expr))));
    BOOST_CHECK_EQUAL(addr1, addr2);

    BOOST_CHECK_EQUAL(1, proto::value(proto::child<1>(proto::child<0>(expr))));
    BOOST_CHECK_EQUAL(1, proto::value(proto::child<1>(proto::child<0>(Convert()(expr)))));
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::make_expr, proto::unpack_expr and friends");

    test->add(BOOST_TEST_CASE(&test_make_expr));
    test->add(BOOST_TEST_CASE(&test_make_expr_functional));
    test->add(BOOST_TEST_CASE(&test_unpack_expr));
    test->add(BOOST_TEST_CASE(&test_unpack_expr_functional));
    test->add(BOOST_TEST_CASE(&test_make_expr_transform));
    test->add(BOOST_TEST_CASE(&test_make_expr_transform2));

    return test;
}
