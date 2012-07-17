///////////////////////////////////////////////////////////////////////////////
// as_transform.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

typedef proto::terminal<int> int_;
typedef proto::terminal<std::string> string_;

void test_as_transform()
{
    int_ p(42);

    {
        int i = (proto::tag::data = 42)[proto::tag::data];
        BOOST_CHECK_EQUAL(i, 42);
        auto env = (proto::tag::data = 42, proto::tag::local = "hello", proto::tag::local = "goodbye");
        i = env[proto::tag::data];
        char const *loc = env[proto::tag::local];
        BOOST_CHECK_EQUAL(std::string(loc), std::string("goodbye"));
        i = env.at(1,1); // lookup with a key that doesn't exist, return default
        BOOST_CHECK_EQUAL(i, 1);

        // Look, ma! A transform!
        char const (&sz)[6] = proto::_data()(p, 42, (proto::tag::data = "hello"));
        BOOST_PROTO_IGNORE_UNUSED(sz);
    }

    {
        proto::or_<
            proto::when<proto::terminal<int>, proto::_int<42>>
          , proto::when<proto::terminal<std::string>, proto::_int<43>>
        > SimpleGrammar;

        int i = SimpleGrammar(int_(0));
        BOOST_CHECK_EQUAL(i, 42);

        i = SimpleGrammar(string_("hello!"));
        BOOST_CHECK_EQUAL(i, 43);
    }

    // Check that expressions can be used as transforms
    p = proto::as_transform<int_>()(p);
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p));
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p, p));
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p, p, p));
    proto::as_transform<proto::function<int_, int_, int_>>()(p(p, p));
}

template<typename T>
struct S
{};

// To defeat an optimization within Proto, to better exercise the proper handling
// of nested transforms with different numbers of arguments.
struct _my_expr
  : proto::transform<_my_expr>
{
    template<typename E, typename ...Rest>
    auto operator()(E && e, Rest &&...) const
    BOOST_PROTO_AUTO_RETURN(
        static_cast<E &&>(e)
    )
};

void test_as_transform_2()
{
    proto::terminal<int> i{42};
    S<int> s0 = proto::as_transform<S<proto::_state>()>()(i, 42);
    S<int> s1 = proto::as_transform<S<proto::_state()>()>()(i, 42);
    S<int> s2 = proto::as_transform<S<proto::_state(_my_expr)>()>()(i, 42);
    S<int> s3 = proto::as_transform<S<proto::_state(_my_expr, proto::_state)>()>()(i, 42, proto::tag::data = 55);
    S<int> s4 = proto::as_transform<S<proto::_state(_my_expr, proto::_state, proto::_env<>)>()>()(i, 42, proto::tag::data = 55);
    BOOST_PROTO_IGNORE_UNUSED(s0, s1, s2, s3, s4);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("basic tests for the transformation infrastructure");

    test->add(BOOST_TEST_CASE(&test_as_transform));
    test->add(BOOST_TEST_CASE(&test_as_transform_2));

    return test;
}
