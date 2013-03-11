////////////////////////////////////////////////////////////////////////////////////////////////////
// make.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/cxx11/proto.hpp>
#include <boost/mpl/identity.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

namespace test_make
{
    template<typename T>
    struct type2type {};

    template<typename T>
    struct wrapper
    {
        T t_;
        explicit wrapper(T const & t = T()) : t_(t) {}
    };

    template<typename T>
    struct careful
    {
        using not_there = typename T::not_there;
    };

    // Test that when no substitution is done, we don't instantiate templates
    struct MakeTest1
      : proto::def< proto::make( type2type< careful<int> >() ) >
    {};

    void make_test1()
    {
        proto::literal<int> i{42};
        type2type< careful<int> > res = MakeTest1()(i);
        BOOST_PROTO_IGNORE_UNUSED(res);
    }

    // Test that when substitution is done, and there is no nested ::type
    // typedef, the result is the wrapper
    struct MakeTest2
      : proto::def< proto::make( wrapper< proto::_value >() ) >
    {};

    void make_test2()
    {
        proto::literal<int> i{42};
        wrapper<int> res = MakeTest2()(i);
        BOOST_CHECK_EQUAL(res.t_, 0);
    }

    // Test that when substitution is done, and there is no nested ::type
    // typedef, the result is the wrapper
    struct MakeTest3
      : proto::def< proto::make( wrapper< proto::_value >(proto::_value) ) >
    {};

    void make_test3()
    {
        proto::literal<int> i{42};
        wrapper<int> res = MakeTest3()(i);
        BOOST_CHECK_EQUAL(res.t_, 42);
    }

    // Test that when substitution is done, and there is a nested ::type
    // typedef, the result is that type.
    struct MakeTest4
      : proto::def< proto::make( mpl::identity< proto::_value >(proto::_value) ) >
    {};

    void make_test4()
    {
        proto::literal<int> i{42};
        int res = MakeTest4()(i);
        BOOST_CHECK_EQUAL(res, 42);
    }

    struct MakeTest5
      : proto::def< proto::make( wrapper< proto::_state(_, proto::_env, proto::_state) >(proto::_state(_)) ) >
    {};

    void make_test5()
    {
        proto::literal<int> i{42};
        wrapper<int> res = MakeTest5()(i, proto::empty_env(), 43);
        BOOST_CHECK_EQUAL(res.t_, 43);
    }

    using proto::utility::identity;
    struct MakeTest6
      : proto::def< proto::make( wrapper< identity(proto::_state) >(proto::_state(_)) ) >
    {};

    void make_test6()
    {
        proto::literal<int> i{42};
        wrapper<int> res = MakeTest6()(i, proto::empty_env(), 43);
        BOOST_CHECK_EQUAL(res.t_, 43);
    }

    struct MakeTest7
      : proto::def< proto::make( wrapper< proto::make( int(proto::_state) ) >(proto::_state(_)) ) >
    {};

    void make_test7()
    {
        proto::literal<int> i{42};
        wrapper<int> res = MakeTest7()(i, proto::empty_env(), 43);
        BOOST_CHECK_EQUAL(res.t_, 43);
    }

    struct tfx : proto::basic_action<tfx>
    {
        template<typename E, typename...T>
        auto operator()(E && e, T &&... t) const
        BOOST_PROTO_AUTO_RETURN(
            static_cast<E &&>(e)
        )
    };

    struct MakeTest8
      : proto::def< proto::make( wrapper< tfx(_, _) >(_) ) >
    {};

    void make_test8()
    {
        proto::literal<int> i{42};
        wrapper<proto::literal<int>> res = MakeTest8()(i);
        BOOST_CHECK_EQUAL(proto::value(res.t_), 42);
    }
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test the make basic_action");

    test->add(BOOST_TEST_CASE(&test_make::make_test1));
    test->add(BOOST_TEST_CASE(&test_make::make_test2));
    test->add(BOOST_TEST_CASE(&test_make::make_test3));
    test->add(BOOST_TEST_CASE(&test_make::make_test4));
    test->add(BOOST_TEST_CASE(&test_make::make_test5));
    test->add(BOOST_TEST_CASE(&test_make::make_test6));
    test->add(BOOST_TEST_CASE(&test_make::make_test7));
    test->add(BOOST_TEST_CASE(&test_make::make_test8));

    return test;
}
