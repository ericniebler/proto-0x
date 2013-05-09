////////////////////////////////////////////////////////////////////////////////////////////////////
// flatten.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/proto/v5/proto.hpp>
#include <boost/fusion/include/for_each.hpp>
#include "./unit_test.hpp"

namespace fusion = boost::fusion;
namespace proto = boost::proto;
using proto::_;

void write(std::ostream &sout, proto::shift_right)
{
    sout << ">>";
}

void write(std::ostream &sout, proto::bitwise_or)
{
    sout << "|";
}

template<typename T0>
void write(std::ostream &sout, proto::exprs::basic_expr<proto::terminal(T0)> const &op)
{
    sout << proto::value(op);
}

template<typename Tag, typename T0>
void write(std::ostream &sout, proto::exprs::basic_expr<Tag(T0)> const &op)
{
    write(sout, Tag());
    write(proto::child(op));
}

template<typename Tag, typename T0, typename T1>
void write(std::ostream &sout, proto::exprs::basic_expr<Tag(T0, T1)> const &op)
{
    write(sout, proto::left(op));
    write(sout, Tag());
    write(sout, proto::right(op));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// to_string
//
struct to_string
{
    explicit to_string(std::ostream &sout)
      : sout_(sout)
    {}

    template<typename Op>
    void operator ()(Op const &op) const
    {
        this->sout_ << '(';
        write(this->sout_, op);
        this->sout_ << ')';
    }
private:
    std::ostream &sout_;
};

void test_expr()
{
    proto::literal<char> a_ {'a'};
    proto::literal<char> b_ {'b'};
    proto::literal<char> c_ {'c'};
    proto::literal<char> d_ {'d'};
    proto::literal<char> e_ {'e'};
    proto::literal<char> f_ {'f'};
    proto::literal<char> g_ {'g'};
    proto::literal<char> h_ {'h'};
    proto::literal<char> i_ {'i'};

    std::stringstream sout;

    // Test for 1-way branching "tree"
    sout.str("");
    fusion::for_each(proto::flatten(!!!!(a_ >> b_)), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)", sout.str());

    // Tests for 2-way branching trees
    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ >> c_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ | b_ | c_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ | b_ >> c_ | d_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b>>c)(d)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_ | e_ >> f_ >> g_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)(e>>f>>g)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_ | e_ >> (f_ | g_) >> h_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)(e>>f|g>>h)", sout.str());

    // Test for n-way branching tree
    sout.str("");
    fusion::for_each(proto::flatten(a_(b_(c_ >> d_, e_ | f_), g_ >> h_)(i_)), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c>>d)(e|f)(g>>h)(i)", sout.str());
}

////////////////////////////////////////////////////////////////////////
// Test that expression extensions are also valid fusion sequences

template<typename ExprDesc>
struct My;

struct MyDomain
  : proto::domain<MyDomain>
{
    using make_expr = proto::make_custom_expr<My>;
};

template<typename ExprDesc>
struct My
  : proto::basic_expr<ExprDesc>
  , proto::expr_function<My<ExprDesc>>
{
    using proto_domain_type = MyDomain;
    using proto::basic_expr<ExprDesc>::basic_expr;
};

using my = proto::custom<My>;

void test_extension()
{
    my::terminal<char> a_ {'a'};
    my::terminal<char> b_ {'b'};
    my::terminal<char> c_ {'c'};
    my::terminal<char> d_ {'d'};
    my::terminal<char> e_ {'e'};
    my::terminal<char> f_ {'f'};
    my::terminal<char> g_ {'g'};
    my::terminal<char> h_ {'h'};
    my::terminal<char> i_ {'i'};

    std::stringstream sout;

    // Test for 1-way branching "tree"
    sout.str("");
    fusion::for_each(proto::flatten(!!!!(a_ >> b_)), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)", sout.str());

    // Tests for 2-way branching trees
    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ >> c_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ | b_ | c_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ | b_ >> c_ | d_), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b>>c)(d)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_ | e_ >> f_ >> g_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)(e>>f>>g)", sout.str());

    sout.str("");
    fusion::for_each(proto::flatten(a_ >> b_ | c_ >> d_ | e_ >> (f_ | g_) >> h_), to_string(sout));
    BOOST_CHECK_EQUAL("(a>>b)(c>>d)(e>>f|g>>h)", sout.str());

    // Test for n-way branching tree
    sout.str("");
    fusion::for_each(proto::flatten(a_(b_(c_ >> d_, e_ | f_), g_ >> h_)(i_)), to_string(sout));
    BOOST_CHECK_EQUAL("(a)(b)(c>>d)(e|f)(g>>h)(i)", sout.str());
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto and segmented fusion integration");

    test->add(BOOST_TEST_CASE(&test_expr));
    test->add(BOOST_TEST_CASE(&test_extension));

    return test;
}
