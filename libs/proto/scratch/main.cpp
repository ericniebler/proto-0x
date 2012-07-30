////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>
#include <boost/mpl/identity.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

// BUGBUG what should happen when terminal<int> is used as a "transform", as
// in terminal<int>(_value). A: That shouldn't be a pass-through, right? Or
// should it?
//static_assert(!proto::is_transform<proto::terminal<int>>::value, "");

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
            proto::_env_var<fN<0>>
          , proto::apply(proto::_env_var<fN<1>>, proto::pack(_))...
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

template<typename T> struct undef;

struct S
{
    void operator()(int) const { std::cout << "hello world!\n"; }
};

int main()
{
    proto::terminal<int> i{0};

    // 0^2 + 1^2 + 2^2 + 3^2 = 0+1+4+9 = 14
    int sum_of_squares = unpack(i(1,2,3), sum(), proto::when<_, square(proto::_value)>());
    std::cout << sum_of_squares << std::endl;

    void done();
    done();
}

void done()
{
    char ch = 0;
    std::cout << "Press <CTRL> + D to quit...";
    while(std::cin.get(ch));
}
