///////////////////////////////////////////////////////////////////////////////
// pack_expansion.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/proto.hpp>
#include <boost/typeof/typeof.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

struct do_eval
{
    template<typename Left, typename Right>
    auto operator()(proto::tag::plus, Left left, Right right) const
    BOOST_PROTO_AUTO_RETURN(
        left + right
    )

    template<typename Left, typename Right>
    auto operator()(proto::tag::multiplies, Left left, Right right) const
    BOOST_PROTO_AUTO_RETURN(
        left * right
    )
};

struct eval
  : proto::or_<
        proto::when<proto::terminal<_>, proto::_value>
      , proto::otherwise<do_eval(proto::tag_of<_>(), eval(proto::pack(_))...)>
    >
{};

void test_call_pack()
{
    proto::terminal<int> i {42};
    int res = eval()(i + 2);
    BOOST_CHECK_EQUAL(res, 44);
    res = eval()(i * 2);
    BOOST_CHECK_EQUAL(res, 84);
    res = eval()(i * 2 + 4);
    BOOST_CHECK_EQUAL(res, 88);
}

struct make_pair
  : proto::when<
        proto::binary_expr<_, proto::terminal<int>, proto::terminal<int> >
      , std::pair<int, int>(proto::_value(proto::pack(_))...)
    >
{};

void test_make_pack()
{
    proto::terminal<int> i {42};
    std::pair<int, int> p = make_pair()(i + 43);
    BOOST_CHECK_EQUAL(p.first, 42);
    BOOST_CHECK_EQUAL(p.second, 43);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test immediate evaluation of proto parse trees");

    test->add(BOOST_TEST_CASE(&test_call_pack));
    test->add(BOOST_TEST_CASE(&test_make_pack));

    return test;
}
