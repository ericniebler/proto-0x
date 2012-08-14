////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>

namespace proto = boost::proto;
using proto::_;

struct foo
{
    int m;
};

struct S
{
    int m;
};

int main()
{
    proto::literal<S> s;
    proto::value(s).m = 42;

    proto::_eval<> eval;
    auto x = eval( proto::literal<int>(42) + 36 );
    auto y = eval( 1+ 2 * (proto::literal<int>(42) + foo()) );

    //int i = eval(s ->* &S::m);
    //std::cout << "i == " << i << std::endl;

    //eval(s ->* &foo::m);

    void done();
    done();
}

void done()
{
    char ch = 0;
    std::cout << "Press <CTRL> + D to quit...";
    while(std::cin.get(ch));
}
