///////////////////////////////////////////////////////////////////////////////
// mpl.hpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

template<class E>
struct my_expr
  : proto::expr<my_expr<E>>
{
    using tag = fusion::fusion_sequence_tag;
    using proto::expr<my_expr<E>>::expr;
};

// Test that we can call mpl algorithms on proto expression types, and get proto expression types back
void test_mpl()
{
    my_expr<proto::terminal(int)> i;
    auto e = i + i;

    static_assert(
        std::is_same<
            mpl::pop_back<decltype(e)>::type
          , my_expr<proto::plus(my_expr<proto::terminal(int)>&)>
        >::value
      , ""
    );
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto mpl integration via fusion");

    test->add(BOOST_TEST_CASE(&test_mpl));

    return test;
}
