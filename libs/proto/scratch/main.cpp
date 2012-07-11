////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  CopyE1 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <iostream>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>

namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

struct eval_expr
{
    template<typename E0, typename E1>
    auto operator()(proto::tag::plus, E0 && e0, E1 && e1) const
    BOOST_PROTO_AUTO_RETURN(
        static_cast<E0 &&>(e0) + static_cast<E1 &&>(e1)
    )

    template<typename E0, typename E1>
    auto operator()(proto::tag::multiplies, E0 && e0, E1 && e1) const
    BOOST_PROTO_AUTO_RETURN(
        static_cast<E0 &&>(e0) * static_cast<E1 &&>(e1)
    )

    // Other overloads...
};

struct eval
  : proto::or_<
        proto::when<proto::terminal<_>, proto::_value>
      , proto::otherwise<eval_expr(proto::_tag_of, eval(proto::pack(_))...)>
    >
{};

struct disp
{
    std::ostream & operator()(int i) const { return std::cout /*<< ch_ */ << i << std::endl; }
};

struct ignore
{
    void operator()(std::ostream &) const {}
};

int main()
{
    proto::terminal<int> i{42};
    proto::otherwise<ignore(disp(proto::pack(_))...)>()(i);

    std::cout << eval()(i * 2 + 1) << std::endl;

    void done();
    done();
}

void done()
{
    std::cout << "Press <RETURN> to continue...";
    char ch = 0;
    std::cin.get(ch);
}
