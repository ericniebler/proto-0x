////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <string>
#include <memory>
#include <functional>
#include <boost/proto/proto.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

template<typename T>
struct placeholder
  : T
{};

struct LambdaEval
  : proto::or_<
        proto::when<
            proto::terminal<placeholder<_>>
          , proto::_data
        >
      , proto::when<
            proto::terminal<int>
          , proto::_value
        >
      , proto::otherwise<
            proto::_eval<LambdaEval>
        >
    >
{};

template<typename Tag, typename Args>
struct LambdaExpr;

struct LambdaDomain
  : proto::domain<LambdaDomain>
{
    struct make_expr
      : proto::make_custom_expr<LambdaExpr, LambdaDomain>
    {};
};

template<typename Tag, typename Args>
struct LambdaExpr
  : proto::basic_expr<Tag, Args, LambdaDomain>
  , proto::expr_assign<LambdaExpr<Tag, Args>, LambdaDomain>
  , proto::expr_subscript<LambdaExpr<Tag, Args>, LambdaDomain>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(LambdaExpr);

    //using proto::basic_expr<Tag, Args, LambdaDomain>::basic_expr;
    typedef proto::basic_expr<Tag, Args, LambdaDomain> proto_basic_expr;
    BOOST_PROTO_INHERIT_EXPR_CTORS(LambdaExpr, proto_basic_expr);

    using proto::expr_assign<LambdaExpr, LambdaDomain>::operator=;
};

template<typename T>
using LambdaLiteral = LambdaExpr<proto::tag::terminal, proto::args<T>>;

constexpr LambdaLiteral<placeholder<mpl::int_<0>>> _1;

int main()
{
    std::printf("hello proto-11!\n\n");

    int i = LambdaEval()(_1 + 42, 0, proto::tag::data = 8);
    std::printf("result is %d\n", i);

    void done();
    done();
}

//////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
