////////////////////////////////////////////////////////////////////////////////////////////////////
// make_expr.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/v5/proto.hpp>
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
    using make_expr = proto::make_custom_expr<ewrap>;
};

template<typename E>
struct ewrap
  : proto::expr<E, mydomain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(ewrap);
    using proto::expr<E, mydomain>::expr;
};

void test_make_expr()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::make_expr(proto::terminal(), 1);
    proto::expr<proto::terminal(int &)> t2 = proto::make_expr(proto::terminal(), i);
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::make_expr(proto::unary_plus(), 1);
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::make_expr(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::make_expr<mydomain>(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::make_expr(proto::plus(), p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_make_expr_2()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::make_expr<proto::terminal>(1);
    proto::expr<proto::terminal(int &)> t2 = proto::make_expr<proto::terminal>(i);
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::make_expr<proto::unary_plus>(1);
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::make_expr<proto::unary_plus>(i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::make_expr<proto::unary_plus, mydomain>(i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::make_expr<proto::plus>(p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_make_expr_functional()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::make_expr<>()(proto::terminal(), 1);
    proto::expr<proto::terminal(int &)> t2 = proto::functional::make_expr<>()(proto::terminal(), i);
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::functional::make_expr<>()(proto::unary_plus(), 1);
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::functional::make_expr<>()(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::functional::make_expr<mydomain>()(proto::unary_plus(), i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::make_expr<>()(proto::plus(), p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_make_expr_functional_2()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::make_expr<proto::terminal>()(1);
    proto::expr<proto::terminal(int &)> t2 = proto::functional::make_expr<proto::terminal>()(i);
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::functional::make_expr<proto::unary_plus>()(1);
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::functional::make_expr<proto::unary_plus>()(i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::functional::make_expr<proto::unary_plus, mydomain>()(i);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::make_expr<proto::plus>()(p3, 0);
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::unpack_expr(proto::terminal(), fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::unpack_expr(proto::terminal(), fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::unpack_expr(proto::unary_plus(), fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::unpack_expr(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::unpack_expr<mydomain>(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::unpack_expr(proto::plus(), fusion::make_tuple(boost::ref(p3), 0));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr_2()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::unpack_expr<proto::terminal>(fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::unpack_expr<proto::terminal>(fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::unpack_expr<proto::unary_plus>(fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::unpack_expr<proto::unary_plus>(fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::unpack_expr<proto::unary_plus, mydomain>(fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::unpack_expr<proto::plus>(fusion::make_tuple(boost::ref(p3), 0));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr_functional()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::unpack_expr<>()(proto::terminal(), fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::functional::unpack_expr<>()(proto::terminal(), fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::functional::unpack_expr<>()(proto::unary_plus(), fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::functional::unpack_expr<>()(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::functional::unpack_expr<mydomain>()(proto::unary_plus(), fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::unpack_expr<>()(proto::plus(), fusion::make_tuple(boost::ref(p3), 0));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(proto::left(p4))), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(proto::left(p4))), &i);
}

void test_unpack_expr_functional_2()
{
    int i = 42;
    proto::expr<proto::terminal(int)> t1 = proto::functional::unpack_expr<proto::terminal>()(fusion::make_tuple(1));
    proto::expr<proto::terminal(int &)> t2 = proto::functional::unpack_expr<proto::terminal>()(fusion::make_tuple(boost::ref(i)));
    proto::expr<proto::unary_plus(proto::terminal(int))> p1 = proto::functional::unpack_expr<proto::unary_plus>()(fusion::make_tuple(1));
    proto::expr<proto::unary_plus(proto::terminal(int &))> p2 = proto::functional::unpack_expr<proto::unary_plus>()(fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p2)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p2)), &i);
    BOOST_PROTO_IGNORE_UNUSED(t1, t2, p1, p2);

    using p3_type = ewrap<proto::unary_plus(ewrap<proto::terminal(int &)>)>;
    p3_type p3 = proto::functional::unpack_expr<proto::unary_plus, mydomain>()(fusion::make_tuple(boost::ref(i)));
    BOOST_CHECK_EQUAL(proto::value(proto::child<0>(p3)), 42);
    BOOST_CHECK_EQUAL(&proto::value(proto::child<0>(p3)), &i);

    ewrap<proto::plus(p3_type &, ewrap<proto::terminal(int)>)> p4 = proto::functional::unpack_expr<proto::plus>()(fusion::make_tuple(boost::ref(p3), 0));
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
            proto::terminal(int)
          , proto::terminal(int)
        )
    > t1 = ByVal()(proto::as_expr(1) + 1);

    proto::expr<
        proto::plus(
            proto::terminal(int &)
          , proto::terminal(int &)
        )
    > t2 = ByRef()(proto::as_expr(1) + 1);

    proto::expr<
        proto::minus(
            proto::terminal(int)
          , proto::terminal(int)
        )
    > t3 = Minus()(proto::as_expr(1) + 1);

    proto::expr<
        proto::plus(
            proto::multiplies(proto::terminal(int), proto::terminal(int))
          , proto::multiplies(proto::terminal(int), proto::terminal(int))
        )
    > t4 = Square()(proto::as_expr(1) + 1);

    BOOST_PROTO_IGNORE_UNUSED(t1, t2, t3, t4);
}

template<typename ExprDesc>
struct by_ref_expr;

struct by_ref_domain
  : proto::domain<by_ref_domain>
{
    using make_expr = proto::make_custom_expr<by_ref_expr>;
    using store_child = proto::utility::by_ref;
};

template<typename ExprDesc>
struct by_ref_expr
  : proto::basic_expr<ExprDesc, by_ref_domain>
  , proto::expr_function<by_ref_expr<ExprDesc>>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(by_ref_expr);
    using proto::basic_expr<ExprDesc, by_ref_domain>::basic_expr;
};

struct length_impl {};
struct dot_impl {};

constexpr by_ref_expr<proto::terminal(length_impl)> length{};
constexpr by_ref_expr<proto::terminal(dot_impl)> dot{};

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
                        proto::terminal(dot_impl())
                      , proto::_child<1>(proto::_child<0>)
                      , proto::_child<1>(proto::_child<0>)
                    )
                  , proto::function(
                        proto::terminal(dot_impl())
                      , proto::_child<1>(proto::_child<1>)
                      , proto::_child<1>(proto::_child<1>)
                    )
                )
            )
        )
    >
{};

void test_make_expr_transform_2()
{
    auto expr = proto::deep_copy(length(1) < length(2));
    // Test that intermediate nodes are stored by value, even though the
    // store_child policy is by_ref
    by_ref_expr<proto::less(
        by_ref_expr<proto::function(
            proto::expr<proto::terminal(dot_impl)> // BUGBUG would be nice if this were by_ref_expr
          , by_ref_expr<proto::terminal(int)> &
          , by_ref_expr<proto::terminal(int)> &
        )>
      , by_ref_expr<proto::function(
            proto::expr<proto::terminal(dot_impl)> // BUGBUG would be nice if this were by_ref_expr
          , by_ref_expr<proto::terminal(int)> &
          , by_ref_expr<proto::terminal(int)> &
        )>
    )> res = Convert()(expr);

    void const *addr1 = boost::addressof(proto::child<1>(proto::child<0>(expr)));
    void const *addr2 = boost::addressof(proto::child<1>(proto::child<0>(res)));
    BOOST_CHECK_EQUAL(addr1, addr2);

    BOOST_CHECK_EQUAL(1, proto::value(proto::child<1>(proto::child<0>(expr))));
    BOOST_CHECK_EQUAL(1, proto::value(proto::child<1>(proto::child<0>(res))));
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::make_expr, proto::unpack_expr and friends");

    test->add(BOOST_TEST_CASE(&test_make_expr));
    test->add(BOOST_TEST_CASE(&test_make_expr_2));
    test->add(BOOST_TEST_CASE(&test_make_expr_functional));
    test->add(BOOST_TEST_CASE(&test_make_expr_functional_2));
    test->add(BOOST_TEST_CASE(&test_unpack_expr));
    test->add(BOOST_TEST_CASE(&test_unpack_expr_2));
    test->add(BOOST_TEST_CASE(&test_unpack_expr_functional));
    test->add(BOOST_TEST_CASE(&test_unpack_expr_functional_2));
    test->add(BOOST_TEST_CASE(&test_unpack_expr_functional));
    test->add(BOOST_TEST_CASE(&test_make_expr_transform));
    test->add(BOOST_TEST_CASE(&test_make_expr_transform_2));

    return test;
}
