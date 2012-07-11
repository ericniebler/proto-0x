///////////////////////////////////////////////////////////////////////////////
// bug2407.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/proto/proto.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

template<class T, class A>
struct e;

struct g
  : proto::or_<
        proto::terminal<int>
      , proto::plus<g,g>
    >
{};

struct d
  : proto::domain<d, g>
{
    struct make_expr
      : proto::make_custom_expr<e>
    {};
};

template<class T, class A>
struct e
  : proto::basic_expr<T, A, d>
{
    typedef proto::basic_expr<T, A, d> proto_basic_expr_type;
    static_assert(proto::matches<proto_basic_expr_type, g>::value, "");

    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(e);
    BOOST_PROTO_INHERIT_EXPR_CTORS(e, proto_basic_expr_type);
};

e<proto::tag::terminal, proto::args<int>> i;

template<class T, class A>
std::ostream &operator<<(std::ostream &sout, e<T, A> const &x)
{
    return sout;
}

int main()
{
    std::cout << (i+i);
}
