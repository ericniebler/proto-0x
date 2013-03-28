////////////////////////////////////////////////////////////////////////////////////////////////////
// fold.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/int.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/proto/v5/proto.hpp>
#include <boost/fusion/include/for_each.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;
namespace proto = boost::proto;
using proto::_;

template<typename Int>
struct placeholder
  : Int
{};

namespace
{
    constexpr auto const & _1 = proto::utility::static_const<proto::literal<placeholder<mpl::int_<0>>>>::value;
    constexpr auto const & _2 = proto::utility::static_const<proto::literal<placeholder<mpl::int_<1>>>>::value;
    constexpr auto const & _3 = proto::utility::static_const<proto::literal<placeholder<mpl::int_<2>>>>::value;
}

BOOST_PROTO_IGNORE_UNUSED(_1, _2, _3);

// The lambda grammar, with the transforms for calculating the max arity
template<typename Fold, int I = 0>
struct lambda_arity
  : proto::def<
        proto::if_(
            proto::matches_( proto::terminal( placeholder<_> ) )
          , proto::make( mpl::next< proto::_value >() )
          , proto::if_(
                proto::matches_( proto::terminal(_) )
              , proto::make( mpl::int_<0>() )
              , Fold(
                    _
                  , proto::make( mpl::int_<0>() )
                  , proto::make( mpl::max<lambda_arity<Fold>, proto::_state>() )
                )
            )
        )
    >
{};

void test_fold()
{
    auto result0 = lambda_arity<proto::fold>()(proto::literal<int>(42) + 36);
    static_assert(decltype(result0)::value == 0, "");

    auto result1 = lambda_arity<proto::fold>()(proto::literal<int>(42) + _1);
    static_assert(decltype(result1)::value == 1, "");

    auto result2 = lambda_arity<proto::fold>()(_2 + _1);
    static_assert(decltype(result2)::value == 2, "");

    auto result3 = lambda_arity<proto::fold>()(_2 + _1 * _3);
    static_assert(decltype(result3)::value == 3, "");
}

void test_reverse_fold()
{
    auto result0 = lambda_arity<proto::reverse_fold>()(proto::literal<int>(42) + 36);
    static_assert(decltype(result0)::value == 0, "");

    auto result1 = lambda_arity<proto::reverse_fold>()(proto::literal<int>(42) + _1);
    static_assert(decltype(result1)::value == 1, "");

    auto result2 = lambda_arity<proto::reverse_fold>()(_2 + _1);
    static_assert(decltype(result2)::value == 2, "");

    auto result3 = lambda_arity<proto::reverse_fold>()(_2 + _1 * _3);
    static_assert(decltype(result3)::value == 3, "");
}

using fusion::nil;
using fusion::cons;

template<typename RecursiveFold, int I = 0>
struct to_cons_list
  : proto::def<
        RecursiveFold(
            _
          , proto::make( nil() )
          , proto::make( cons<proto::_value, proto::_state>(proto::_value, proto::_state) )
        )
    >
{};

void test_recursive_fold()
{
    cons<char, cons<char const(&)[6], cons<int>>> list0 =
        to_cons_list<proto::recursive_fold>()( proto::literal<int>(42) >> "hello" >> 'a' );

    BOOST_CHECK_EQUAL(list0.car, 'a');
    BOOST_CHECK_EQUAL(std::strcmp(list0.cdr.car, "hello"), 0);
    BOOST_CHECK_EQUAL(list0.cdr.cdr.car, 42);
}

void test_reverse_recursive_fold()
{
    cons<int, cons<char const(&)[6], cons<char>>> list0 =
        to_cons_list<proto::reverse_recursive_fold>()( proto::literal<int>(42) >> "hello" >> 'a' );

    BOOST_CHECK_EQUAL(list0.car, 42);
    BOOST_CHECK_EQUAL(std::strcmp(list0.cdr.car, "hello"), 0);
    BOOST_CHECK_EQUAL(list0.cdr.cdr.car, 'a');
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test fold algorithms");

    test->add(BOOST_TEST_CASE(&test_fold));
    test->add(BOOST_TEST_CASE(&test_reverse_fold));
    //test->add(BOOST_TEST_CASE(&test_recursive_fold));
    //test->add(BOOST_TEST_CASE(&test_reverse_recursive_fold));

    return test;
}
