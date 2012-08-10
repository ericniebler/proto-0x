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
using proto::_a;
using proto::_b;

///////////////////////////////////////////////////////////////////////////////
// test_let_call_once
//  verify that the transform bound to the local variable is really called
//  only once.
struct once
{
    static int ctors;
    once() { ++ctors; }
};

int once::ctors = 0;

struct wrap_a
  : proto::action<
        proto::let(
            _b(proto::construct(int()))
          , proto::functional::make_pair(_a, _a)
        )
    >
{};

struct LetCallOnce
  : proto::action<
        proto::let(
            _a(proto::construct(once()))
          , wrap_a
          //, proto::functional::make_pair(wrap_a, _a)
        )
    >
{};

void test_let_call_once()
{
    proto::literal<int> i(0);
    std::pair<once, once> p = LetCallOnce()(i);

    std::cout << "Should be 1: " << once::ctors << std::endl;
}

int main()
{
    test_let_call_once();

    void done();
    done();
}

void done()
{
    char ch = 0;
    std::cout << "Press <CTRL> + D to quit...";
    while(std::cin.get(ch));
}