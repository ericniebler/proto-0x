//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This is an example of how to specify an action externally so
// that a single grammar can be used to drive multiple differnt
// calculations. In particular, it defines a calculator grammar
// that computes the result of an expression with either checked
// or non-checked division.

#include <iostream>
#include <boost/mpl/int.hpp>
#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

// The argument placeholder type
template<typename I>
struct placeholder
  : I, proto::env_tag<placeholder<I>>
{
    using proto::env_tag<placeholder<I>>::operator=;
};

// Give each rule in the grammar a "name". This is so that we
// can easily dispatch on it later.
struct calc_grammar;
struct divides_rule
  : proto::def<proto::divides(calc_grammar, calc_grammar)>
  , proto::env_tag<divides_rule>
{
    using proto::env_tag<divides_rule>::operator=;
};

// Use external transforms in calc_gramar
struct calc_grammar
  : proto::def<
        proto::match(
            proto::case_(
                proto::terminal(placeholder<_>)
              , proto::get_env(proto::_value)
            )
          , proto::case_(
                proto::terminal(proto::convertible_to<double>)
              , proto::_value
            )
          , proto::case_(
                proto::plus(calc_grammar, calc_grammar)
              , proto::eval_with(calc_grammar)
            )
          , proto::case_(
                proto::minus(calc_grammar, calc_grammar)
              , proto::eval_with(calc_grammar)
            )
          , proto::case_(
                proto::multiplies(calc_grammar, calc_grammar)
              , proto::eval_with(calc_grammar)
            )
            // Note that we don't specify how division nodes are
            // handled here. proto::external is a placeholder
            // for an actual action.
          , proto::case_(
                divides_rule
              , proto::external
            )
        )
    >
{};

template<typename ExprDesc>
struct calc_expr
  : proto::expr<calc_expr<ExprDesc>>
{
    using proto::expr<calc_expr<ExprDesc>>::expr;
};

constexpr calc_expr<proto::terminal(placeholder<mpl::int_<0>>)> _1 {};
constexpr calc_expr<proto::terminal(placeholder<mpl::int_<1>>)> _2 {};

struct non_checked_division
  : proto::def<proto::eval_with(calc_grammar)>
{};

struct division_by_zero
  : std::exception
{};

struct do_checked_divide
{
    int operator()(int left, int right) const
    {
        if (right == 0) throw division_by_zero();
        return left / right;
    }
};

// Use proto::external_transforms again, this time to map the divides_rule
// to a transforms that performs checked division.
struct checked_division
  : proto::def<do_checked_divide(calc_grammar(proto::_left), calc_grammar(proto::_right))>
{};

void test_external_actions()
{
    placeholder<mpl::int_<0>> _1_; // BUGBUG would be nice to just say _1 = 2;
    placeholder<mpl::int_<1>> _2_;
    int result1 = calc_grammar()(_1 / _2, (_1_ = 6, _2_ = 2, divides_rule() = non_checked_division()));
    BOOST_CHECK_EQUAL(result1, 3);

    try
    {
        // This should throw
        int result2 = calc_grammar()(_1 / _2, (_1_ = 6, _2_ = 0, divides_rule() = checked_division()));
        BOOST_PROTO_IGNORE_UNUSED(result2);
        BOOST_CHECK(!"Didn't throw an exception"); // shouldn't get here!
    }
    catch(division_by_zero)
    {
        ; // OK
    }
    catch(...)
    {
        BOOST_CHECK(!"Unexpected exception"); // shouldn't get here!
    }
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test for external actions");

    test->add(BOOST_TEST_CASE(&test_external_actions));

    return test;
}
