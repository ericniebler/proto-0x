////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <iostream>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>
#include <boost/fusion/include/list.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

struct display
{
    template<typename T>
    void operator()(T const &t) const
    {
        std::cout << t << '\t';
    }
};

int main()
{
    using fusion::cons;
    proto::terminal<int> i{42};
    cons<int, cons<int, cons<int>>> j =
        proto::_reverse_recursive_fold<
            _
          , fusion::nil()
          , cons<proto::_value, proto::_state>(proto::_value, proto::_state)
        >()(i + 1 + 2);
    fusion::for_each(j, display());
    std::cout << '\n';

    void done();
    done();
}

void done()
{
    std::cout << "Press <RETURN> to continue...";
    char ch = 0;
    std::cin.get(ch);
}
