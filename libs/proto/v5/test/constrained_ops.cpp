////////////////////////////////////////////////////////////////////////////////////////////////////
// constrained_ops.cpp
//
//  Copyright 2010 Thomas Heller
//  Copyright 2012 Eric Niebler
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/v5/core.hpp>
#include <boost/proto/v5/debug.hpp>
#include "./unit_test.hpp"

using namespace boost;

using term = proto::literal<int>;

struct equation;

struct addition:
    proto::def<
        proto::match(
            proto::terminal(proto::_)
          , proto::plus(addition, addition)
        )
    >
{};

struct equation:
    proto::def<
        proto::equal_to(addition, addition)
    >
{};

template<class ExprDesc>
struct extension;

struct my_domain:
    proto::domain<
         my_domain,
         equation,
         proto::default_domain
    >
{
    using make_expr = proto::make_custom_expr<extension>;
};

template<class ExprDesc>
struct lhs_extension;

struct my_lhs_domain:
    proto::domain<
        my_lhs_domain,
        addition,
        my_domain
    >
{
    using make_expr = proto::make_custom_expr<lhs_extension>;
};

template<class ExprDesc>
struct rhs_extension;

struct my_rhs_domain:
    proto::domain<
        my_rhs_domain,
        addition,
        my_domain
    >
{
    using make_expr = proto::make_custom_expr<rhs_extension>;
};

template<class ExprDesc>
struct extension
  : proto::basic_expr<ExprDesc, my_domain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(extension);
    using proto_basic_expr_type = proto::basic_expr<ExprDesc, my_domain>;
    BOOST_PROTO_INHERIT_EXPR_CTORS(extension, proto_basic_expr_type);

    void test() const
    {}
};

template<class ExprDesc>
struct lhs_extension
  : proto::basic_expr<ExprDesc, my_lhs_domain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(lhs_extension);
    using proto_basic_expr_type = proto::basic_expr<ExprDesc, my_lhs_domain>;
    BOOST_PROTO_INHERIT_EXPR_CTORS(lhs_extension, proto_basic_expr_type);
};

template<class ExprDesc>
struct rhs_extension
  : proto::basic_expr<ExprDesc, my_rhs_domain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(rhs_extension);
    using proto_basic_expr_type = proto::basic_expr<ExprDesc, my_rhs_domain>;
    BOOST_PROTO_INHERIT_EXPR_CTORS(rhs_extension, proto_basic_expr_type);
};

void test_constrained_ops()
{
     lhs_extension<proto::terminal(int)> const i {0};
     rhs_extension<proto::terminal(int)> const j {0};

     proto::assert_matches_not<equation>(i);              // false
     proto::assert_matches_not<equation>(j);              // false
     proto::assert_matches_not<equation>(i + i);          // false
     proto::assert_matches_not<equation>(j + j);          // false
#if 0
     proto::assert_matches_not<equation>(i + j);          // compile error (by design)
     proto::assert_matches_not<equation>(j + i);          // compile error (by design)
#endif
     proto::assert_matches<equation>(i == j);             // true
     proto::assert_matches<equation>(i == j + j);         // true
     proto::assert_matches<equation>(i + i == j);         // true
     proto::assert_matches<equation>(i + i == j + j);     // true
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test constrained EDSLs");
    test->add(BOOST_TEST_CASE(&test_constrained_ops));
    return test;
}
