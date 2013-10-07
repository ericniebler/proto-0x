////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma clang diagnostic ignored "-Wmultichar"
#include <typeinfo>
#include <map>
#include <iostream>
#include <boost/proto/v5/proto.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

using namespace proto;
struct Expr
  : def<
        match(
            case_(terminal(unsigned long long),     _value)
          , case_(plus(Expr, Expr),                 plus(Expr(_left), Expr(_right))) // here
          , case_(multiplies(Expr, Expr),           multiplies(Expr(_left), Expr(_right)))
        )
    >
{};

template<typename A>
struct ExprF
  : def<
        match(
            terminal(unsigned long long)
          , plus(A, A)
          , multiplies(A, A)
        )
    >
{};

template<template<typename> class F>
struct Fix
  : F<Fix<F>>
{};

using Expr2 = Fix<ExprF>;


int main()
{
    using namespace proto::literals;
    auto x = 1_et + 2_et;

    proto::assert_matches<Expr2>(x);

    unsigned long long y = Expr()(x);
    std::cout << y << std::endl;

    void done();
    done();
}




//// The argument placeholder type
//template<typename I>
//struct placeholder
//  : I, proto::env_tag<placeholder<I>>
//{
//    using proto::env_tag<placeholder<I>>::operator=;
//};
//
//// Give each rule in the grammar a "name". This is so that we
//// can easily dispatch on it later.
//struct calc_grammar;
//
//struct divides_rule
//  : proto::def<proto::divides(calc_grammar, calc_grammar)>
//  , proto::env_tag<divides_rule>
//{
//    using proto::env_tag<divides_rule>::operator=;
//};
//
//// Use external transforms in calc_gramar
//struct calc_grammar
//  : proto::def<
//        proto::match(
//            proto::case_(
//                proto::terminal(placeholder<_>)
//              , proto::get_env(proto::_value)
//            )
//          , proto::case_(
//                proto::terminal(proto::convertible_to<double>)
//              , proto::_value
//            )
//          , proto::case_(
//                proto::plus(calc_grammar, calc_grammar)
//              , proto::eval_with(calc_grammar)
//            )
//          , proto::case_(
//                proto::minus(calc_grammar, calc_grammar)
//              , proto::eval_with(calc_grammar)
//            )
//          , proto::case_(
//                proto::multiplies(calc_grammar, calc_grammar)
//              , proto::eval_with(calc_grammar)
//            )
//            // Note that we don't specify how division nodes are
//            // handled here. proto::external is a placeholder
//            // for an actual action.
//          , proto::case_(
//                divides_rule
//              , proto::external
//            )
//        )
//    >
//{};
//
//template<typename ExprDesc>
//struct calc_expr
//  : proto::expr<calc_expr<ExprDesc>>
//{
//    using proto::expr<calc_expr<ExprDesc>>::expr;
//    using proto::expr<calc_expr<ExprDesc>>::operator=;
//};
//
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<0>>)> _1 {};
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<1>>)> _2 {};
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<2>>)> _3 {};
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<3>>)> _4 {};
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<4>>)> _5 {};
//constexpr calc_expr<proto::terminal(placeholder<mpl::int_<5>>)> _6 {};
//
//struct non_checked_division
//  : proto::def<proto::eval_with(calc_grammar)>
//{};
//
//struct division_by_zero
//  : std::exception
//{};
//
//struct do_checked_divide
//{
//    int operator()(int left, int right) const
//    {
//        if (right == 0)
//            throw division_by_zero();
//        return left / right;
//    }
//};
//
//// Use proto::external_transforms again, this time to map the divides_rule
//// to a transforms that performs checked division.
//struct checked_division
//  : proto::def<do_checked_divide(calc_grammar(proto::_left), calc_grammar(proto::_right))>
//{};
//
////namespace detail
////{
////    using namespace proto;
////
////    struct key_value_grammar
////      : def<assign(terminal(_), terminal(_))>
////    {};
////
////    struct as_env_
////    {
////        template<typename KV, typename Env
////            , BOOST_PROTO_ENABLE_IF(!matches<KV, key_value_grammar>())>
////        constexpr envs::empty_env operator()(KV &&, Env &&env) const noexcept
////        {
////            static_assert(matches<KV, key_value_grammar>(), "Expected a key/value pair");
////            return envs::empty_env();
////        }
////
////        template<typename KV, typename Env
////            , BOOST_PROTO_ENABLE_IF(matches<KV, key_value_grammar>())>
////        constexpr auto operator()(KV &&kv, Env &&env) const
////        BOOST_PROTO_AUTO_RETURN(
////            proto::envs::make_env(
////                proto::value(proto::left(static_cast<KV &&>(kv)))
////              , proto::value(proto::right(static_cast<KV &&>(kv)))
////              , static_cast<Env &&>(env)
////            )
////        )
////    };
////
////    struct make_env_
////    {
////        template<typename Env>
////        constexpr auto operator()(Env &&env) const
////        BOOST_PROTO_AUTO_RETURN(
////            Env(static_cast<Env &&>(env))
////        )
////
////        template<typename Env, typename KV, typename ...Rest, typename Impl = make_env_>
////        constexpr auto operator()(Env &&env, KV &&kv, Rest &&... rest) const
////        BOOST_PROTO_AUTO_RETURN(
////            BOOST_PROTO_TRY_CALL(Impl())(
////                BOOST_PROTO_TRY_CALL(::detail::as_env_())(
////                    static_cast<KV &&>(kv)
////                  , static_cast<Env &&>(env)
////                )
////              , static_cast<Rest &&>(rest)...
////            )
////        )
////    };
////}
////
////template<typename ...KeyValue>
////constexpr auto make_env(KeyValue &&... key_value)
////BOOST_PROTO_AUTO_RETURN(
////    BOOST_PROTO_TRY_CALL(detail::make_env_())(
////        proto::envs::empty_env()
////      , static_cast<KeyValue &&>(key_value)...
////    )
////)
//
//int main()
//{
//    //auto env = ::make_env(_1 = 1, _2 = 2, _3 = 3, _4 = 4, _5 = 5, _6 = 6, _1 = 7);
//    //std::cout << proto::detail::name_of<decltype(env)>() << std::endl;
//
//    //std::cout << env( proto::value(_1) ) << std::endl;
//    //std::cout << env( proto::value(_2) ) << std::endl;
//    //std::cout << env( proto::value(_3) ) << std::endl;
//    //std::cout << env( proto::value(_4) ) << std::endl;
//    //std::cout << env( proto::value(_5) ) << std::endl;
//    //std::cout << env( proto::value(_6) ) << std::endl;
//
//    //placeholder<mpl::int_<0>> _1_; // BUGBUG would be nice to just say _1 = 2;
//    //placeholder<mpl::int_<1>> _2_;
//    //int result1 = calc_grammar()(_1 / _2, (_1_ = 6, _2_ = 2, divides_rule() = non_checked_division()));
//    //std::cout << result1 << std::endl;
//
//    //try
//    //{
//    //    // This should throw
//    //    int result2 = calc_grammar()(_1 / _2, (_1_ = 6, _2_ = 0, divides_rule() = checked_division()));
//    //    //throw division_by_zero();
//    //}
//    //catch(division_by_zero const &e)
//    //{
//    //    std::cout << "here " << e.what() << std::endl;
//    //}
//    ////catch(...)
//    ////{
//    ////    std::cout << "oops" << std::endl;
//    ////}
//
//    void done();
//    done();
//}


//namespace proto = boost::proto;
//using namespace proto;
//using namespace literals;
//
//struct map_list_of_  {};
//
//struct MapListOf : def<
//    match(
//        case_(  terminal(map_list_of_),
//            void()
//        ),
//        case_(  function(MapListOf, terminal(_), terminal(_)),
//            MapListOf(_child0),
//            assign(subscript(_data, _value(_child1)), _value(_child2))
//        )
//    )
//> {};
//
//template<typename ExprDesc>
//struct map_list_of_expr
//  : expr<map_list_of_expr<ExprDesc>, domain<_, MapListOf>>
//{
//    using expr<map_list_of_expr, domain<_, MapListOf>>::expr;
//
//    template<class K, class V, class C, class A>
//    operator std::map<K,V,C,A>() const
//    {
//        BOOST_PROTO_ASSERT_MATCHES(*this, MapListOf);
//        std::map<K,V,C,A> m;
//        MapListOf()( *this, data = m );
//        return m;
//    }
//};
//
//constexpr map_list_of_expr<terminal(map_list_of_)> map_list_of {};
//
//int main()
//{
//    std::map<int, int> m = map_list_of(1,2)(2,3)(3,4)(4,5)(5,6);
//
//    for(auto p : m)
//    {
//        std::cout << p.first << " => " << p.second << std::endl;
//    }
//
//    void done();
//    done();
//}



/*
#include <cstdio>
#include <utility>
#include <typeinfo>
#include <type_traits>
#include <memory>
#include <cstdlib>
#include <boost/proto/v5/proto.hpp>
#include <boost/proto/v5/debug.hpp>

namespace proto = boost::proto;
using proto::_;

template<typename T>
struct placeholder
  : proto::env_tag<placeholder<T>>
{
    using proto::env_tag<placeholder<T>>::operator=;
};

static_assert(std::is_trivial<placeholder<std::integral_constant<std::size_t, 0>>>::value, "");

// So placeholder terminals can be pretty-printed with display_expr
template<typename T>
std::ostream & operator << (std::ostream & s, placeholder<T>)
{
    return s << "_" << T::value + 1;
}

template<std::size_t I>
using placeholder_c = placeholder<std::integral_constant<std::size_t, I>>;

struct lambda_eval
  : proto::def<
        proto::match(
            proto::case_( proto::terminal(placeholder<_>),
                proto:get_env(proto::_value)
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
struct lambda_expr;

struct lambda_domain
  : proto::domain<lambda_domain>
{
    using make_expr = proto::make_custom_expr<lambda_expr<_>>;
    using store_child = proto::utility::by_ref;
};

template<typename ExprDesc>
struct lambda_expr
  : proto::basic_expr<ExprDesc>
  , proto::expr_assign<lambda_expr<ExprDesc>>
  , proto::expr_subscript<lambda_expr<ExprDesc>>
{
    using proto_domain_type = lambda_domain;
    using proto::basic_expr<ExprDesc>::basic_expr;
    using proto::expr_assign<lambda_expr>::operator=;

    template<typename ...T>
    auto operator()(T &&... t) const
    BOOST_PROTO_AUTO_RETURN(
        lambda_eval_(proto::utility::make_indices<sizeof...(T)>(), *this, std::forward<T>(t)...)
    )
};

template<typename T>
using lambda_var = lambda_expr<proto::terminal(T)>;

namespace
{
    constexpr auto const & _1 = proto::utility::static_const<lambda_var<placeholder_c<0>>>::value;
    constexpr auto const & _2 = proto::utility::static_const<lambda_var<placeholder_c<1>>>::value;
    constexpr auto const & _3 = proto::utility::static_const<lambda_var<placeholder_c<2>>>::value;
}

static_assert(std::is_trivial<lambda_var<placeholder_c<0>>>::value, "_1 should be trivial");
BOOST_PROTO_IGNORE_UNUSED(_1, _2, _3);

using make_minus = proto::functional::make_expr<proto::minus>;

struct Invert
  : proto::def<
        proto::match(
            proto::case_(   proto::terminal(_),
                            proto::_                            )
          , proto::case_(   proto::plus(_,_),
                            proto::minus(Invert(proto::_left),
                                         Invert(proto::_right)) )
          , proto::case_(   _(Invert...),
                            proto::passthru                         )
        )
    >
{};

int main()
{
    std::printf("*** \n");
    std::printf("*** This program demonstrates how to build a lambda library with Proto.\n");
    std::printf("*** \n");

    std::cout << "expr:\n"
              << proto::pretty_expr(_1 + 42 * _2, 4)
              << std::endl;

    // Create a lambda
    auto fun = proto::deep_copy(_1 + 42 * _2);

    std::cout << "deep-copied expr:\n"
              << proto::pretty_expr(fun, 4)
              << std::endl;

    std::cout << "Invert'ed expr:\n"
              << proto::pretty_expr(Invert()(fun), 4)
              << std::endl;

    // call the lambda
    int i = fun(8, 2);

    // print the result
    std::printf("the lambda '_1 + 42 * _2' yields '%d' when called with 8 and 2.\n", i);

    void done();
    done();
}
//*/

#include <iostream>
void done()
{
    char ch = 0;
    std::cout << "CTRL+D to end...";
    std::cin >> ch;
}
