////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  CopyE1 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <iostream>
#include <type_traits>
#include <sstream>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/functional/std/utility.hpp>

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
    std::ostream & operator()(int i) const { return std::cout << i << std::endl; }
};

struct ignore
{
    void operator()(std::ostream &) const {}
};

/*
Bartosz:
   eval_( []() { return 10; } + 20 );
   eval_( []() { return "Hello "; } + "World!");
*/

struct accept_pairs
{
    void disp(std::ostream &) const {}
    template<typename Head, typename ...Tail>

    void disp(std::ostream & sout, Head h, Tail ...t) const
    {
        sout << '(' << h.first << ',' << h.second << ')';
        disp(sout, t...);
    }

    template<typename ...Firsts, typename ...Seconds>
    void operator()(std::ostream & sout, std::pair<Firsts, Seconds>... pairs) const
    {
        disp(sout, pairs...);
    }
};

struct do_accept_pairs
  : proto::otherwise<
        accept_pairs(
            proto::_data
          , proto::functional::make_pair(
                proto::_value(proto::pack(proto::_child<0>))
              , proto::_value(proto::pack(proto::_child<1>))
            )...
        )
    >
{};

int main()
{
    proto::terminal<int> i{42};
    proto::otherwise<ignore(disp(proto::pack(_))...)>()(i);

    std::cout << eval()(i * 2 + 1) << std::endl;

    std::ostringstream sout;
    do_accept_pairs()( i(1,2) + i("hello","world"), 0, proto::tag::data = sout);
    std::cout << sout.str() << std::endl;

    void done();
    done();
}

void done()
{
    char ch = 0;
    std::cout << "Press <CTRL> + D to quit...";
    while(std::cin.get(ch));
}
