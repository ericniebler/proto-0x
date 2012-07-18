///////////////////////////////////////////////////////////////////////////////
// apply.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/proto.hpp>
#include "./unit_test.hpp"

namespace proto = boost::proto;
using proto::_;

template<int I>
struct fN
  : proto::tags::def<fN<I>>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(fN);
    using proto::tags::def<fN>::operator=;
};

namespace
{
    constexpr fN<0> const & _f0 = proto::utility::static_const<fN<0>>::value;
    constexpr fN<1> const & _f1 = proto::utility::static_const<fN<1>>::value;
}

struct eval_unpack
  : proto::as_transform<
        proto::apply(
            proto::_env<fN<0>>
          , proto::apply(proto::_env<fN<1>>, proto::pack(_))...
        )
    >
{};

template<typename E, typename F0, typename F1>
auto unpack(E && e, F0 && f0, F1 && f1)
BOOST_PROTO_AUTO_RETURN(
    eval_unpack()(
        static_cast<E &&>(e)
      , 0
      , (_f0 = static_cast<F0 &&>(f0), _f1 = static_cast<F1 &&>(f1))
    )
)

struct square
{
    template<typename T>
    T operator()(T t) const
    {
        return t * t;
    }
};

struct sum
{
    template<typename T>
    T operator()(T t) const
    {
        return t;
    }

    template<typename T, typename ...U>
    T operator()(T t, U ...u) const
    {
        return t + (*this)(u...);
    }
};

void test_apply()
{
    proto::terminal<int> i{0};

    // 0^2 + 1^2 + 2^2 + 3^2 = 0+1+4+9 = 14
    proto::as_transform<square(proto::_value)> square_;
    int sum_of_squares = unpack(i(1,2,3), sum(), square_);
    BOOST_CHECK_EQUAL(sum_of_squares, 14);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("tests for proto::apply");

    test->add(BOOST_TEST_CASE(&test_apply));

    return test;
}
