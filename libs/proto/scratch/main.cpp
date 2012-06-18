#define BOOST_PROTO_ASSERT_VALID_DOMAIN(DOM) typedef DOM DOM ## _

#include <cstdio>
#include <typeinfo>
#include <string>
#include <boost/assert.hpp>
#include <boost/proto/args.hpp>
#include <boost/proto/proto.hpp>
namespace proto = boost::proto;
using proto::_;

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
typedef proto::terminal<std::string> string_;
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

    bool b0 = (int_(42) + 42) == (42 + int_(42));
    if(!b0)
        std::printf("%s\n", "***ERROR 7**** (int_(42) + 42) is not equal to (42 + int_(42))");
    bool b1 = (int_(42) + int_(42)) == (int_(42) + int_(43));
    if(b1)
        std::printf("%s\n", "***ERROR 7**** (int_(42) + int_(42)) is equal to (int_(42) + int_(43))");

    // test for nothrow operations
    typedef proto::not_equal_to<int_, int_> int_ne_int;
    int_ne_int inei(int_(42), int_(42));
    constexpr int_ne_int cinei(int_(42), int_(42));
    static_assert(noexcept(int_ne_int()), "not noexcept default constructor");
    static_assert(noexcept(int_ne_int(cinei)), "not noexcept copy constructor");
    static_assert(noexcept(int_ne_int(int_ne_int())), "not noexcept move constructor");
    static_assert(noexcept(inei = cinei), "not noexcept copy assign");
    static_assert(noexcept(inei = int_ne_int()), "not noexcept move assign");

    // Verify that the constexpr ctors can be called even when the
    // contained objects have non-constexpr constructors
    string_ str("hellohellohellohellohellohellohellohello!");

    {
        int i = (proto::tag::data = 42)[proto::tag::data];
        auto env = (proto::tag::data = 42, proto::tag::locals = "hello", proto::tag::locals = "goodbye");
        i = env[proto::tag::data];
        char const *loc = env[proto::tag::locals];
        std::printf("%s\n", loc); // should print "goodbye"
        i = env.at(1,1); // lookup with a key that doesn't exist, return default

        // Look, ma! A transform!
        char const (&sz)[6] = proto::_data()(p, 42, (proto::tag::data = "hello"));
    }

    {
        proto::or_<
            proto::when<proto::terminal<int>, proto::int_<42>>
          , proto::when<proto::terminal<std::string>, proto::int_<43>>
        > SimpleGrammar;

        int i = SimpleGrammar(int_(0));
        std::printf("This should be 42: %d\n", i);

        i = SimpleGrammar(string_("hello!"));
        std::printf("This should be 43: %d\n", i);
    }

    p = proto::as_transform<int_>()(p);
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p));
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p, p));
    proto::as_transform<proto::function<int_, int_, proto::vararg<int_>>>()(p(p, p, p));
    proto::as_transform<proto::function<int_, int_, int_>>()(p(p, p));

    void done();
    done();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// deduce_domain tests
struct D0 : proto::domain<D0>
{
};

struct D1 : proto::domain<D1, proto::_, D0>
{
};

struct D2 : proto::domain<D2, proto::_, D0>
{
};

struct D3 : proto::domain<D3>
{
};

struct DD0 : proto::domain<DD0, proto::_, proto::default_domain>
{
};

struct DD1 : proto::domain<DD1, proto::_, proto::default_domain>
{
};

struct DD2 : proto::domain<DD2, proto::_, proto::default_domain>
{
};

struct DD3 : proto::domain<DD3, proto::_, DD2>
{
};

struct DD4 : proto::domain<DD4, proto::_, DD2>
{
};

void test_common_domain()
{
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, proto::default_domain, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, proto::default_domain, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, D0, proto::default_domain>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, proto::default_domain>::type, proto::default_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<DD0, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, DD0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, DD0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, D0, DD0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, DD0, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, proto::default_domain, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, proto::default_domain>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, proto::default_domain, DD0>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<proto::default_domain, DD0, proto::default_domain>::type, DD0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, proto::default_domain>::type, DD0>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D0>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D1>::type, D1>::value, "");

    // Very tricky to get right
    static_assert(std::is_same<proto::detail::common_domain<D2, D2, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D1>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D2>::type, D0>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D1, D1>::type, D1>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D0, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D3, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D0, D3>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D1, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D3, D0, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D3, D0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D3, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D0, D1, D3>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D0, D3>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<D3, D1, D2>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D3, D2, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D3, D2>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D3, D1>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D2, D1, D3>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<D1, D2, D3>::type, proto::detail::not_a_domain>::value, "");

    // These should be ambiguous.
    static_assert(std::is_same<proto::detail::common_domain<DD1, DD0, DD0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD1, DD0>::type, proto::detail::not_a_domain>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD0, DD0, DD1>::type, proto::detail::not_a_domain>::value, "");

    static_assert(std::is_same<proto::detail::common_domain<DD3, DD2, DD2>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD2, DD3, DD2>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD2, DD2, DD3>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD3, DD4, DD4>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD4, DD3, DD4>::type, DD2>::value, "");
    static_assert(std::is_same<proto::detail::common_domain<DD4, DD4, DD3>::type, DD2>::value, "");
}

struct MyIntWrap : int_ {};

void test_matches()
{
    static_assert(proto::matches<int_, int_>::value, "");
    static_assert(proto::matches<MyIntWrap, int_>::value, "");
    static_assert(proto::matches<MyIntWrap &, int_>::value, "");
    static_assert(proto::matches<MyIntWrap &, MyIntWrap>::value, "");
    static_assert(!proto::matches<int_, proto::unary_plus<_>>::value, "");
    static_assert(!proto::matches<int_, string_>::value, "");
    static_assert(proto::matches<proto::function<int_, int_, int_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(proto::matches<proto::function<int_>, proto::function<int_, proto::vararg<string_>>>::value, "");
    static_assert(proto::matches<proto::function<int_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(!proto::matches<proto::function<int_, int_, string_>, proto::function<proto::vararg<int_>>>::value, "");
    static_assert(!proto::matches<int_, proto::function<int_, int_>>::value, "");
}

//////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
