///////////////////////////////////////////////////////////////////////////////
// pack_expansion.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
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
  : proto::grammar<
        proto::algorithms::match(
            proto::case_(proto::tag::terminal(_),
                proto::_value
            )
          , proto::case_(_,
                do_eval(proto::construct(proto::tag_of<_>()), eval(proto::pack(_))...)
            )
        )
    >
{};

void test_call_pack()
{
    proto::terminal<int> i {42};
    int res = eval()(i);
    BOOST_CHECK_EQUAL(res, 42);
    res = eval()(i + 2);
    BOOST_CHECK_EQUAL(res, 44);
    res = eval()(i * 2);
    BOOST_CHECK_EQUAL(res, 84);
    res = eval()(i * 2 + 4);
    BOOST_CHECK_EQUAL(res, 88);
}

struct make_pair
  : proto::action<proto::construct(std::pair<int, int>(proto::_value(proto::pack(_))...))>
{};

void test_make_pack()
{
    proto::terminal<int> i {42};
    std::pair<int, int> p = make_pair()(i + 43);
    BOOST_CHECK_EQUAL(p.first, 42);
    BOOST_CHECK_EQUAL(p.second, 43);
}

struct do_accept_pairs
{
    void disp(std::ostream &) const {}

    template<typename Head, typename ...Tail>
    void disp(std::ostream & sout, Head h, Tail ...t) const
    {
        sout << '(' << h.first << ',' << h.second << ')';
        disp(sout, t...);
    }

    template<typename ...Firsts, typename ...Seconds>
    void operator()(std::ostream & sout, std::pair<Firsts, Seconds>... pairs) const
    {
        disp(sout, pairs...);
    }
};

struct front
{
    template<typename Head, typename ...Tail>
    Head && operator()(Head && h, Tail &&...) const
    {
        return static_cast<Head &&>(h);
    }
};

// Test expanding multiple packs in parallel
struct accept_pairs
  : proto::action<
        do_accept_pairs(
            proto::_data
          , proto::functional::make_pair(
                proto::_value(proto::pack(proto::_child<0>))
              , proto::_value(proto::pack(proto::_child<1>))
            )...
        )
    >
{};

// Test expanding multiple packs in parallel *and* a nested pack expansion.
struct accept_pairs_2
  : proto::action<
        do_accept_pairs(
            proto::_data
          , proto::functional::make_pair(
                proto::_value(proto::pack(front(proto::pack(_)...)))
              , proto::_value(proto::pack(proto::_child<1>))
            )...
        )
    >
{};

void test_multiple_packs()
{
    proto::terminal<int> i{42};
    std::ostringstream sout;
    accept_pairs()( i(1,2) + i("hello","world"), 0, proto::tag::data = sout);
    BOOST_CHECK_EQUAL( sout.str(), std::string("(42,42)(1,hello)(2,world)") );

    sout.str("");
    accept_pairs_2()(i(1,2) + i("this","that"), 0, proto::tag::data = sout);
    BOOST_CHECK_EQUAL( sout.str(), std::string("(42,42)(1,this)(2,that)") );
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test for proto::pack, pack expansion, and unpack patterns");

    test->add(BOOST_TEST_CASE(&test_call_pack));
    test->add(BOOST_TEST_CASE(&test_make_pack));
    test->add(BOOST_TEST_CASE(&test_multiple_packs));

    return test;
}
