////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <string>
#include <boost/proto/proto.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

// Test that simple proto expressions are trivial.
// Note: expressions that store references are not, and cannot be, trivial because
// they are not default constructable.
typedef proto::terminal<int> int_;
typedef proto::terminal<std::string> string_;

template<typename Tag, typename Args>
struct MyExpr;

struct MyDomain
  : proto::domain<MyDomain>
{
    struct make_expr
      : proto::make_custom_expr<MyExpr, MyDomain>
    {};
};

template<typename Tag, typename Args>
struct MyExpr
  : proto::basic_expr<Tag, Args, MyDomain>
  , proto::expr_assign<MyExpr<Tag, Args>, MyDomain>
  , proto::expr_subscript<MyExpr<Tag, Args>, MyDomain>
  , proto::expr_function<MyExpr<Tag, Args>, MyDomain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(MyExpr);

    //using proto::basic_expr<Tag, Args, MyDomain>::basic_expr;
    typedef proto::basic_expr<Tag, Args, MyDomain> proto_basic_expr;
    BOOST_PROTO_INHERIT_EXPR_CTORS(MyExpr, proto_basic_expr);

    using proto::expr_assign<MyExpr, MyDomain>::operator=;
};

template<typename T>
using MyLiteral = MyExpr<proto::tag::terminal, proto::args<T>>;

int main()
{
    std::printf("hello proto-11!\n\n");

    // TEST CODE HERE

    void done();
    done();
}


//////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
