////////////////////////////////////////////////////////////////////////////////////////////////////
// common_domain.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_PROTO_ASSERT_VALID_DOMAIN(DOM) using DOM ## _ = DOM

#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;

////////////////////////////////////////////////////////////////////////////////////////////////////
// deduce_domain tests
struct D0 : proto::domain<D0>
{
};

struct D1 : proto::domain<D1, proto::_, D0>
{
};

struct D2 : proto::domain<D2, proto::_, D0>
{
};

struct D3 : proto::domain<D3>
{
};

struct DD0 : proto::domain<DD0, proto::_, proto::default_domain>
{
};

struct DD1 : proto::domain<DD1, proto::_, proto::default_domain>
{
};

struct DD2 : proto::domain<DD2, proto::_, proto::default_domain>
{
};

struct DD3 : proto::domain<DD3, proto::_, DD2>
{
};

struct DD4 : proto::domain<DD4, proto::_, DD2>
{
};

void test_common_domain()
{
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, proto::default_domain, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, proto::default_domain, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, D0, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, proto::default_domain>::type, proto::default_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<DD0, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, DD0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, DD0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, D0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, DD0, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, proto::default_domain, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, proto::default_domain>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, DD0, proto::default_domain>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, proto::default_domain>::type, DD0>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D1>::type, D1>::value, "");

    // Very tricky to get right
    static_assert(std::is_same<proto::detail::common_domain<D2, D2, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D1>::type, D1>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D0, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D3, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D3>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D1, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D3, D0, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D3, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D3, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D3>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D3>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D1, D2>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D3, D2, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D3, D2>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D3, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D3>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D3>::type, proto::detail::not_a_domain>::value, "");

    // These should be ambiguous.
    static_assert(std::is_same<proto::detail::common_domain<DD1, DD0, DD0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD1, DD0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, DD1>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<DD3, DD2, DD2>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD2, DD3, DD2>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD2, DD2, DD3>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD3, DD4, DD4>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD4, DD3, DD4>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD4, DD4, DD3>::type, DD2>::value, "");
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("tests for common_domain");

    test->add(BOOST_TEST_CASE(&test_common_domain));

    return test;
}
