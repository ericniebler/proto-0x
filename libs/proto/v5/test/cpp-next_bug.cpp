////////////////////////////////////////////////////////////////////////////////////////////////////
// cpp-next_bug.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <boost/proto/v5/proto.hpp>
#include <boost/proto/v5/debug.hpp>
#include "./unit_test.hpp"
namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

namespace linear_algebra
{
    // A trait that returns true only for std::vector
    template<typename T>
    struct is_std_vector
      : mpl::false_
    {};

    template<typename T, typename A>
    struct is_std_vector<std::vector<T, A> >
      : mpl::true_
    {};

    // A type used as a domain for linear algebra expressions
    struct linear_algebra_domain
      : proto::domain<linear_algebra_domain>
    {
        using make_expr = proto::make_custom_expr<proto::expr<_, linear_algebra_domain>>;
    };

    // Define all the operator overloads for combining std::vectors
    #define BOOST_PROTO_OPERATOR_TRAITS (is_std_vector, linear_algebra_domain)
    #include BOOST_PROTO_DEFINE_OPERATORS()

    // Take any expression and turn each node
    // into a subscript expression, using the
    // state as the RHS.
    struct Distribute
      : proto::def<
            proto::match(
                proto::case_(
                    proto::terminal(_)
                  , proto::subscript(_, proto::_state)
                )
              , proto::case_(
                    proto::plus(Distribute, Distribute)
                  , proto::passthru
                )
            )
        >
    {};

    struct Optimize
      : proto::def<
            proto::match(
                proto::case_(
                    proto::subscript(Distribute, proto::terminal(_))
                  , Distribute(proto::_left, proto::empty_env(), proto::_right)
                )
              , proto::case_(
                    proto::plus(Optimize, Optimize)
                  , proto::passthru
                )
              , proto::case_(
                    proto::terminal(_)
                  , proto::passthru
                )
            )
        >
    {};
}

static constexpr int celems = 4;
static constexpr int value[celems] = {1,2,3,4};
std::vector<int> A(value, value+celems), B(A);

void test1()
{
    using namespace linear_algebra;
    proto::_eval eval;
    int result = eval(Optimize()((A + B)[3]));
    proto::assert_matches<Optimize>((A + B)[3]);
    proto::assert_matches_not<Optimize>((A - B)[3]);
    BOOST_CHECK_EQUAL(8, result);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test for a problem reported on the cpp-next.com blog");

    test->add(BOOST_TEST_CASE(&test1));

    return test;
}
