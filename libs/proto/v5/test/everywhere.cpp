////////////////////////////////////////////////////////////////////////////////////////////////////
// everything.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;

struct S_ {};
constexpr proto::expr<proto::terminal(S_)> S {};
constexpr auto omg = S(1,2)(2,3);

struct Square
  : proto::def<
        proto::everywhere(
            proto::case_(
                proto::terminal(int),
                proto::terminal(proto::multiplies(proto::_value, proto::_value))
            )
        )
    >
{};

using X = proto::custom<proto::expr<_>>;
constexpr X::function<
    X::function<
        X::terminal<S_>
      , X::terminal<int>
      , X::terminal<int>
    >
  , X::terminal<int>
  , X::terminal<int>
> omg_wtf = Square()(omg);

static_assert(proto::value(proto::child<2>(proto::child<0>(omg_wtf)))==4, "oops");

struct Flip
  : proto::def<
        proto::everywhere(
            proto::case_(
                proto::plus(_,_)
              , proto::minus(proto::_left, proto::_right)
            )
          , proto::case_(
                proto::minus(_,_)
              , proto::plus(proto::_left, proto::_right)
            )
        )
    >
{};

void test_everything()
{
    using namespace proto::literals;

    proto::expr<
        proto::plus(
            proto::minus(
                proto::plus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
              , proto::plus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
            )
          , proto::minus(
                proto::plus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
              , proto::plus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
            )
        )
    > y = ((1_et + 1_et) - (1_et + 1_et)) + ((1_et + 1_et) - (1_et + 1_et));


    proto::expr<
        proto::minus(
            proto::plus(
                proto::minus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
              , proto::minus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
            )
          , proto::plus(
                proto::minus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
              , proto::minus(
                    proto::terminal(unsigned long long)
                  , proto::terminal(unsigned long long)
                )
            )
        )
    > z = Flip()(y);

    BOOST_CHECK_EQUAL(
        proto::value(proto::left(proto::left(proto::left(z))))
      , 1
    );
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("testing the everything action");

    test->add(BOOST_TEST_CASE(&test_everything));

    return test;
}
