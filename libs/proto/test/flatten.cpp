///////////////////////////////////////////////////////////////////////////////
// flatten.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <boost/proto/proto.hpp>
#include <boost/fusion/include/for_each.hpp>
#include "./unit_test.hpp"

namespace fusion = boost::fusion;
namespace proto = boost::proto;
using proto::_;

void write(std::ostream &sout, proto::tag::shift_right)
{
    sout << ">>";
}

void write(std::ostream &sout, proto::tag::bitwise_or)
{
    sout << "|";
}

template<typename T0, typename Domain>
void write(std::ostream &sout, proto::basic_expr<proto::tag::terminal, proto::args<T0>, Domain> const &op)
{
    sout << proto::value(op);
}

template<typename Tag, typename T0, typename Domain>
void write(std::ostream &sout, proto::basic_expr<Tag, proto::args<T0>, Domain> const &op)
{
    write(sout, Tag());
    write(proto::child(op));
}

template<typename Tag, typename T0, typename T1, typename Domain>
void write(std::ostream &sout, proto::basic_expr<Tag, proto::args<T0, T1>, Domain> const &op)
{
    write(sout, proto::left(op));
    write(sout, Tag());
    write(sout, proto::right(op));
}

///////////////////////////////////////////////////////////////////////////////
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
    proto::terminal<char> a_ {'a'};
    proto::terminal<char> b_ {'b'};
    proto::terminal<char> c_ {'c'};
    proto::terminal<char> d_ {'d'};
    proto::terminal<char> e_ {'e'};
    proto::terminal<char> f_ {'f'};
    proto::terminal<char> g_ {'g'};
    proto::terminal<char> h_ {'h'};
    proto::terminal<char> i_ {'i'};

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

template<typename Tag, typename Args>
struct My;

struct MyDomain
  : proto::domain<MyDomain>
{
    struct make_expr
      : proto::make_custom_expr<My>
    {};
};

template<typename Tag, typename Args>
struct My
  : proto::basic_expr<Tag, Args, MyDomain>
  , proto::expr_function<My<Tag, Args>, MyDomain>
{
    typedef proto::basic_expr<Tag, Args, MyDomain> proto_basic_expr_type;
    BOOST_PROTO_INHERIT_EXPR_CTORS(My, proto_basic_expr_type);
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(My);
};

template<typename T>
using MyTerm = My<proto::tag::terminal, proto::args<T>>;

void test_extension()
{
    MyTerm<char> a_ {'a'};
    MyTerm<char> b_ {'b'};
    MyTerm<char> c_ {'c'};
    MyTerm<char> d_ {'d'};
    MyTerm<char> e_ {'e'};
    MyTerm<char> f_ {'f'};
    MyTerm<char> g_ {'g'};
    MyTerm<char> h_ {'h'};
    MyTerm<char> i_ {'i'};

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
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto and segmented fusion integration");

    test->add(BOOST_TEST_CASE(&test_expr));
    test->add(BOOST_TEST_CASE(&test_extension));

    return test;
}
