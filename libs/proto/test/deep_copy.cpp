///////////////////////////////////////////////////////////////////////////////
// deep_copy.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <iostream>
#include <boost/utility/addressof.hpp>
#include <boost/proto/core.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;

struct moveable
{
    moveable() = default;
    moveable(moveable const &) = delete;
    moveable(moveable &&) = default;
    moveable & operator=(moveable const &) = delete;
    moveable & operator=(moveable &&) = default;
};

struct noncopyable
{
    noncopyable() = default;
    noncopyable(noncopyable const &) = delete;
    noncopyable(noncopyable &&) = delete;
    noncopyable & operator=(noncopyable const &) = delete;
    noncopyable & operator=(noncopyable &&) = delete;
};

struct noncopyable2
{
    noncopyable2() = default;
private:
    noncopyable2(noncopyable2 const &);
    noncopyable2 & operator=(noncopyable2 const &);
};

void foo() {}

void test_expr()
{
    using namespace proto;

    int i = 42;
    terminal<int &> t1 {i};
    terminal<int> r1 = deep_copy(t1);
    BOOST_CHECK_EQUAL(42, value(r1));

    plus<terminal<int>, terminal<int>> r2 = deep_copy(t1 + 24);
    BOOST_CHECK_EQUAL(42, value(left(r2)));
    BOOST_CHECK_EQUAL(24, value(right(r2)));

    char buf[16] = {'\0'};
    terminal<char (&)[16]> t3 {buf};
    terminal<char (&)[16]> r3 = deep_copy(t3);

    terminal<void(&)()> t4 {foo};
    plus<terminal<void(&)()>, terminal<int>> r4 = deep_copy(t4 + t1);
    BOOST_CHECK_EQUAL(42, value(right(r4)));
    BOOST_CHECK_EQUAL(&foo, &value(left(r4)));

    terminal<std::ostream &> cout_ {std::cout};
    shift_left<terminal<std::ostream &>, terminal<int>> r5 = deep_copy(cout_ << t1);
    BOOST_CHECK_EQUAL(42, value(right(r5)));
    BOOST_CHECK_EQUAL(boost::addressof(std::cout), boost::addressof(value(left(r5))));

    BOOST_PROTO_IGNORE_UNUSED(r3);
}

template<typename Tag, typename Args>
struct Expr;

struct Domain
  : proto::domain<Domain>
{
    struct store_value
      : proto::utility::by_ref
    {};

    struct store_child
      : proto::utility::by_ref
    {};

    struct make_expr
      : proto::make_custom_expr<Expr>
    {};
};

template<typename Tag, typename Args>
struct Expr
  : proto::basic_expr<Tag, Args, Domain>
{
    typedef proto::basic_expr<Tag, Args, Domain> proto_basic_expr_type;
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(Expr);
    BOOST_PROTO_INHERIT_EXPR_CTORS(Expr, proto_basic_expr_type);
};

template<typename T>
using Terminal = Expr<proto::tag::terminal, proto::args<T>>;

template<typename T, typename U>
using Plus = Expr<proto::tag::plus, proto::args<T, U>>;

template<typename T, typename U>
using ShiftLeft = Expr<proto::tag::shift_left, proto::args<T, U>>;

void test_custom_expr()
{
    using namespace proto;

    int i = 42;
    Terminal<int &> t1 {i};
    Terminal<int> r1 = deep_copy(t1);
    BOOST_CHECK_EQUAL(42, value(r1));

    static_assert(std::is_same<Plus<Terminal<int &> &, Terminal<int const &>>, decltype(t1 + 24)>::value, "");
    Plus<Terminal<int>, Terminal<int>> r2 = deep_copy(t1 + 24);
    BOOST_CHECK_EQUAL(42, value(left(r2)));
    BOOST_CHECK_EQUAL(24, value(right(r2)));

    char buf[16] = {'\0'};
    Terminal<char (&)[16]> t3 {buf};
    Terminal<char (&)[16]> r3 = deep_copy(t3);

    Terminal<void(&)()> t4 {foo};
    Plus<Terminal<void(&)()>, Terminal<int>> r4 = deep_copy(t4 + t1);
    BOOST_CHECK_EQUAL(42, value(right(r4)));
    BOOST_CHECK_EQUAL(&foo, &value(left(r4)));

    Terminal<std::ostream &> cout_ {std::cout};
    ShiftLeft<Terminal<std::ostream &>, Terminal<int>> r5 = deep_copy(cout_ << t1);
    BOOST_CHECK_EQUAL(42, value(right(r5)));
    BOOST_CHECK_EQUAL(boost::addressof(std::cout), boost::addressof(value(left(r5))));

    BOOST_PROTO_IGNORE_UNUSED(r3);
}

void test_moveable()
{
    using namespace proto;

    terminal<moveable> t1 {};
    terminal<moveable> r1 = deep_copy(std::move(t1));

    terminal<moveable> t2 {};
    plus<terminal<moveable>, terminal<int>> r2 = deep_copy(std::move(t2) + 24);
    BOOST_PROTO_IGNORE_UNUSED(r1, r2);
}

void test_noncopyable()
{
    using namespace proto;

    noncopyable i;
    terminal<noncopyable &> t1 {i};
    terminal<noncopyable &> r1 = deep_copy(t1);

    plus<terminal<noncopyable &>, terminal<int>> r2 = deep_copy(t1 + 24);
    BOOST_PROTO_IGNORE_UNUSED(r1, r2);
}

void test_noncopyable2()
{
    using namespace proto;

    noncopyable2 i;
    terminal<noncopyable2 &> t1 {i};
    terminal<noncopyable2 &> r1 = deep_copy(t1);

    plus<terminal<noncopyable2 &>, terminal<int>> r2 = deep_copy(t1 + 24);
    BOOST_PROTO_IGNORE_UNUSED(r1, r2);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test deep_copy of proto parse trees");

    test->add(BOOST_TEST_CASE(&test_expr));
    test->add(BOOST_TEST_CASE(&test_custom_expr));

    return test;
}
