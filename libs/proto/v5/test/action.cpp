////////////////////////////////////////////////////////////////////////////////////////////////////
// action.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

using int_ = proto::literal<int>;
using string_ = proto::literal<std::string>;

struct scope_type
  : proto::env_var_tag<scope_type>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(scope_type);
    using proto::env_var_tag<scope_type>::operator=;
};

namespace
{
    constexpr auto const & scope = proto::utility::static_const<scope_type>::value;
}

void test_action()
{
    int_ p(42);

    {
        int i = (proto::data = 42)(proto::data);
        BOOST_CHECK_EQUAL(i, 42);
        auto env = (proto::data = 42, scope = "hello", scope = "goodbye");
        i = env(proto::data);
        char const *loc = env(scope);
        BOOST_CHECK_EQUAL(std::string(loc), std::string("goodbye"));

        // Look, ma! A basic_action!
        char const (&sz)[6] = proto::_data()(p, (proto::data = "hello"));
        BOOST_PROTO_IGNORE_UNUSED(sz);
    }

    {
        proto::def<
            proto::match(
                proto::case_(proto::terminal(int), proto::_int<42>)
              , proto::case_(proto::terminal(std::string), proto::_int<43>)
            )
        > SimpleAction;

        int i = SimpleAction(int_(0));
        BOOST_CHECK_EQUAL(i, 42);

        i = SimpleAction(string_("hello!"));
        BOOST_CHECK_EQUAL(i, 43);
    }

    // Check the pass-through basic_action
    using X = proto::terminal(int);
    proto::def<proto::passthru(X, X, X...)>()(p(p));
    proto::def<proto::passthru(X, X, X...)>()(p(p, p));
    proto::def<proto::passthru(X, X, X...)>()(p(p, p, p));
}

template<typename T>
struct S
{};

// To defeat an optimization within Proto, to better exercise the proper handling
// of nested actions with different numbers of arguments.
struct _my_expr
  : proto::basic_action<_my_expr>
{
    template<typename E, typename ...Rest>
    auto operator()(E && e, Rest &&...) const
    BOOST_PROTO_AUTO_RETURN(
        static_cast<E &&>(e)
    )
};

void test_action_2()
{
    proto::literal<int> i{42};
    S<int> s0 = proto::def<proto::make(S<proto::_state>())>()(i, proto::empty_env(), 42);
    S<int> s1 = proto::def<proto::make(S<proto::_state()>())>()(i, proto::empty_env(), 42);
    S<int> s2 = proto::def<proto::make(S<proto::_state(_my_expr)>())>()(i, proto::empty_env(), 42);
    S<int> s3 = proto::def<proto::make(S<proto::_state(_my_expr, proto::_env)>())>()(i, proto::data = 55, 42);
    S<int> s4 = proto::def<proto::make(S<proto::_state(_my_expr, proto::_env, proto::_state)>())>()(i, proto::data = 55, 42);
    BOOST_PROTO_IGNORE_UNUSED(s0, s1, s2, s3, s4);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("basic tests for the action infrastructure");

    test->add(BOOST_TEST_CASE(&test_action));
    test->add(BOOST_TEST_CASE(&test_action_2));

    return test;
}
