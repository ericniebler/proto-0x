#include <cstdio>
#include <typeinfo>
#include <boost/assert.hpp>
#include <boost/proto/proto.hpp>
namespace proto = boost::proto;

template<typename T>
struct undefined;

template<typename A, typename B>
struct AAA;

template<typename A>
using BBB = AAA<A, int>;

template<template<typename> class C>
struct CCC {};
CCC<BBB> ccc;

// Test that simple proto expressions are trivial.
// Note: expressions that store references are not, and cannot be, trivial because
// they are not default constructable.
typedef proto::terminal<int> int_;
static_assert(std::is_trivial<decltype(int_())>::value, "not trivial!");
static_assert(std::is_trivial<decltype(int_()(3.14))>::value, "not trivial!");

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

static_assert(std::is_trivial<MyLiteral<int>>::value, "not is trivial!");

int main()
{
    std::printf("hello proto-0x!\n\n");

    int_ p(42);
    int_ const pc(42);
    constexpr int_ i_(42);

    // Quick test for big expression nodes (>10 children)
    typedef proto::expr<proto::tag::function, proto::args<int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_>> ints_;
    ints_ is(p,p,p,p,p,p,p,p,p,p,p,p,p,p);
    p = proto::child<13>(is);

    // quick sanity check for as_expr
    auto a1 = proto::as_expr(42);
    auto a2 = proto::as_expr(p);
    std::printf("type of 'proto::as_expr(42)' = '%s'\n", typeid(a1).name());
    std::printf("type of 'proto::as_expr(p)'  = '%s'\n", typeid(a2).name());
    static_assert(std::is_same<decltype(a1), decltype(a2)>::value, "not the same!");

    // quick sanity test for proto::value
    int i = proto::value(p);

    // sanity test for stored_value and stored_child (used by as_expr when building nodes)
    proto::expr<proto::tag::function, proto::args<int_&, int_, int_, int_, proto::terminal<char const (&)[6]> >> x = p(1, 2, 3, "hello");
    std::printf("type of 'p(1,2,3,\"hello\")'  = '%s'\n", typeid(x).name());
    std::printf("type of '\"hello\"'  = '%s'\n", typeid(proto::value(proto::child<4>(x))).name());
    static_assert(std::is_same<decltype(proto::value(proto::child<4>(x))), char const (&)[6]>::value, "not the same!");

    // verify that expression nodes are Regular types.
    auto y0 = (p=p);
    auto y1 = (p=pc);
    static_assert(std::is_same<decltype(y0)::proto_tag_type, proto::tag::terminal>::value, "");
    static_assert(std::is_same<decltype(y1)::proto_tag_type, proto::tag::terminal>::value, "");

    // Verify that overloaded assignment builds an assign expression node.
    auto y2 = (p='c');
    static_assert(std::is_same<decltype(y2)::proto_tag_type, proto::tag::assign>::value, "");

    // verify that args accessor on rvalue expression is itself an rvalue
    static_assert(std::is_rvalue_reference<decltype(int_().proto_args())>::value, "isn't an rvalue reference!");

    // verify that expression nodes are no larger than they need to be.
    static_assert(sizeof(proto::terminal<int>) == sizeof(int), "sizeof(proto::terminal<int>) != sizeof(int)");
    static_assert(sizeof(proto::expr<proto::tag::function, proto::args<>>) == 1, "size of empty expr is not 1");

    // This should fail to compile:
    //typedef int_ const cint_;
    //cint_(42)[p];

    constexpr MyLiteral<int> iii_(42);
    auto jjj_ = iii_[42];
    std::printf("iii_[42] has type '%s'\n", typeid(jjj_).name());
    std::printf("child<0>(iii_[42]) has type '%s'\n", typeid(proto::child<0>(jjj_)).name());
    std::printf("child<1>(iii_[42]) has type '%s'\n", typeid(proto::child<1>(jjj_)).name());

    // Test proto_equal_to
    if(!jjj_.proto_equal_to(iii_[42]))
        std::printf("%s\n", "***ERROR 1**** iii_[42] is not equal to iii_[42]");
    if(jjj_.proto_equal_to(iii_[43]))
        std::printf("%s\n", "***ERROR 2**** iii_[42] is equal to iii_[43]");

    // Test convertibility to bool
    if(!proto::domains::make_expr<MyDomain>(proto::tag::equal_to(), jjj_, iii_[42]))
        std::printf("%s\n", "***ERROR 3**** iii_[42] is not equal to iii_[42]");
    if(proto::domains::make_expr<MyDomain>(proto::tag::equal_to(), jjj_, iii_[43]))
        std::printf("%s\n", "***ERROR 4**** iii_[42] is equal to iii_[43]");
    if(proto::domains::make_expr<MyDomain>(proto::tag::not_equal_to(), jjj_, iii_[42]))
        std::printf("%s\n", "***ERROR 5**** iii_[42] is not equal to iii_[42]");
    if(!proto::domains::make_expr<MyDomain>(proto::tag::not_equal_to(), jjj_, iii_[43]))
        std::printf("%s\n", "***ERROR 6**** iii_[42] is equal to iii_[43]");

    static_assert(std::is_convertible<proto::equal_to<int_, int_>, bool>::value, "not convertible to bool");
    static_assert(std::is_convertible<proto::not_equal_to<int_, int_>, bool>::value, "not convertible to bool");
    static_assert(!std::is_convertible<proto::equal_to<int_, proto::terminal<void *>>, bool>::value, "convertible to bool");
    static_assert(!std::is_convertible<proto::not_equal_to<int_, proto::terminal<void *>>, bool>::value, "convertible to bool");

    void done();
    done();
}

//////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
