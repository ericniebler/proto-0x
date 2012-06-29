////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <utility>
#include <type_traits>
#include <functional>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>

namespace proto = boost::proto;
using proto::_;

int main()
{
    proto::terminal<int> i{1};
    auto seq = i(i,i,i,i);

    typedef proto::reverse_fold<_, proto::_int<0>, std::plus<int>(proto::_state, proto::_value)> sum;
    int j = sum()(seq);

    std::printf("sum is: %d\n", j);

    void done();
    done();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
