////////////////////////////////////////////////////////////////////////////////////////////////////
// lambda.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include <boost/proto/v5/debug.hpp>

namespace proto = boost::proto;
using proto::_;

template<typename T>
struct placeholder
  : proto::env_tag<placeholder<T>>
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(placeholder);
    using proto::env_tag<placeholder<T>>::operator=;

    // So placeholder terminals can be pretty-printed with display_expr
    friend std::ostream & operator << (std::ostream & s, placeholder<T>)
    {
        return s << "_" << T::value + 1;
    }
};

template<std::size_t I>
using placeholder_c = placeholder<std::integral_constant<std::size_t, I>>;

struct lambda_eval
  : proto::def<
        proto::match(
            proto::case_( proto::terminal(placeholder<_>),
                proto::get_env(proto::_value)
            )
          , proto::case_( proto::terminal(_),
                proto::_value
            )
          , proto::default_(
                proto::eval_with(lambda_eval)
            )
        )
    >
{};

template<std::size_t ...I, typename E, typename ...T>
inline auto lambda_eval_(proto::utility::indices<I...>, E && e, T &&... t)
BOOST_PROTO_AUTO_RETURN(
    lambda_eval()(
        std::forward<E>(e)
      , proto::make_env(placeholder_c<I>() = std::forward<T>(t)...)
    )
)

template<typename ExprDesc>
struct lambda_expr
  : proto::basic_expr<lambda_expr<ExprDesc>>
  , proto::expr_assign<lambda_expr<ExprDesc>>
  , proto::expr_subscript<lambda_expr<ExprDesc>>
{
    using proto::basic_expr<lambda_expr>::basic_expr;
    using proto::expr_assign<lambda_expr>::operator=;

    template<typename ...T>
    auto operator()(T &&... t) const
    BOOST_PROTO_AUTO_RETURN(
        lambda_eval_(
            proto::utility::make_indices<sizeof...(T)>()
          , *this, std::forward<T>(t)...
        )
    )
};

template<typename T>
using lambda_var = proto::custom<lambda_expr<_>>::terminal<T>;

namespace
{
    constexpr auto const & _1 = proto::utility::static_const<lambda_var<placeholder_c<0>>>::value;
    constexpr auto const & _2 = proto::utility::static_const<lambda_var<placeholder_c<1>>>::value;
    constexpr auto const & _3 = proto::utility::static_const<lambda_var<placeholder_c<2>>>::value;
}

static_assert(std::is_trivial<lambda_var<placeholder_c<0>>>::value, "_1 should be trivial");
BOOST_PROTO_IGNORE_UNUSED(_1, _2, _3);

int main()
{
    std::printf("*** \n");
    std::printf("*** This program demonstrates how to build a lambda library with Proto.\n");
    std::printf("*** \n");

    // Create a lambda
    auto fun = _1 + 42 * _2;

    // pretty-print the expression
    proto::display_expr(fun);

    // Call the lambda
    int i = fun(8, 2);

    // print the result
    std::printf("The lambda '_1 + 42 * _2' yields '%d' when called with 8 and 2.\n", i);
}
