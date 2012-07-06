///////////////////////////////////////////////////////////////////////////////
// virtual_member.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/utility/addressof.hpp>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;

struct foo_tag {};
struct bar_tag {};

template<typename Tag, typename Args>
struct MyExpr;

struct MyDomain
  : proto::domain<MyDomain>
{
    struct make_expr
      : proto::make_custom_expr<MyExpr>
    {};
};

template<typename Tag, typename Args>
struct MyExpr
  : proto::basic_expr<Tag, Args, MyDomain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(MyExpr);

    typedef proto::basic_expr<Tag, Args, MyDomain> proto_basic_expr_type;
    BOOST_PROTO_INHERIT_EXPR_CTORS(MyExpr, proto_basic_expr_type);

    BOOST_PROTO_EXTENDS_MEMBERS(
        MyExpr, MyDomain,
        ((foo_tag, foo))
        ((bar_tag, bar))
    );
};

template<typename Value>
using MyTerminal = MyExpr<proto::tag::terminal, proto::args<Value>>;

void test_virtual_members()
{
    MyTerminal<int> xxx{42};
    MyTerminal<int> & r = proto::child<0>(xxx.foo);
    proto::terminal<foo_tag> & e = proto::child<1>(xxx.foo);

    static_assert(std::is_lvalue_reference<decltype(proto::child<0>(xxx.foo))>::value, "");
    static_assert(std::is_rvalue_reference<decltype(proto::child<0>(MyTerminal<int>().foo))>::value, "");

    static_assert(std::is_lvalue_reference<decltype(proto::child<1>(xxx.foo))>::value, "");
    static_assert(std::is_rvalue_reference<decltype(proto::child<1>(MyTerminal<int>().foo))>::value, "");

    BOOST_CHECK_EQUAL(42, proto::value(xxx));
    BOOST_CHECK_EQUAL(42, proto::value(r));

    // These addresses had better be the same
    BOOST_CHECK_EQUAL(boost::addressof(xxx), boost::addressof(r));
    BOOST_CHECK_EQUAL(boost::addressof(xxx.foo.proto_args().proto_child1), boost::addressof(e));

    // Check that member expressions match their grammars
    struct G : proto::member<proto::terminal<int>, proto::terminal<foo_tag>> {};
    proto::assert_matches<G>(xxx.foo);

    // Check that the pass-through transform handles virtual members correctly.
    MyExpr<proto::tag::member, proto::args<MyTerminal<int>, proto::terminal<foo_tag>>> tx = proto::as_transform<G>()(xxx.foo);
    BOOST_PROTO_IGNORE_UNUSED(tx);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("tests for virtual members");

    test->add(BOOST_TEST_CASE(&test_virtual_members));

    return test;
}
