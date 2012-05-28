#include <cstdio>
#include <typeinfo>
#include <boost/proto/proto.hpp>
namespace proto = boost::proto;

struct B : proto::term<int> {};

static_assert(std::is_trivial<proto::term<int>>::value, "not trivial!");
static_assert(std::is_trivial<B>::value, "not trivial!");
static_assert(std::is_trivial<proto::expr<proto::tag::terminal, proto::term<int>>>::value, "not trivial!");
constexpr proto::expr<proto::tag::terminal, proto::term<int>> i_(42);

struct A { A(int) {} };

template<typename T>
decltype(A(std::declval<T>())) foo(T) { return A(42); }

int foo(...) { return 69; }

template<typename Expr>
struct MyExpr;

struct MyDomain
  : proto::domain<proto::generator<proto::expr_wrap<MyExpr>>>
{};

template<typename Expr>
struct MyExpr
  : Expr
{
    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(MyExpr);

    template<typename Expr2>
    constexpr explicit MyExpr(Expr2 && e)
      : Expr(static_cast<Expr2 &&>(e))
    {};
};

int main()
{
    A a = foo(42);
    A b = foo("");

    typedef proto::literal<int> int_;
    int_ p(42);
    int_ const pc(42);

    typedef proto::expr<proto::tag::function, proto::args<int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_>> ints_;
    ints_ is(p,p,p,p,p,p,p,p,p,p,p,p,p,p);
    p = proto::child<13>(is);

    proto::as_expr(42);
    proto::as_expr(p);

    int i = proto::value(p);

    auto x = p(1,2,3,4);

    auto y0 = (p=p);
    auto y1 = (p=pc);
    static_assert(std::is_same<decltype(y0)::proto_tag, proto::tag::terminal>::value, "");
    static_assert(std::is_same<decltype(y1)::proto_tag, proto::tag::terminal>::value, "");

    auto y2 = (p='c');
    static_assert(std::is_same<decltype(y2)::proto_tag, proto::tag::assign>::value, "");

    //typedef int_ const cint_;
    //cint_(42)[p];

    MyExpr<proto::literal<int, MyDomain>> iii_(proto::literal<int, MyDomain>(42));
    auto jjj_ = iii_[42];
    std::printf("iii_[42] has type '%s'\n", typeid(jjj_).name());
    std::printf("child<0>(iii_[42]) has type '%s'\n", typeid(proto::child<0>(jjj_)).name());
    std::printf("child<1>(iii_[42]) has type '%s'\n", typeid(proto::child<1>(jjj_)).name());

    std::printf("hello proto-0x!\n");
    void done();
    done();
}

//////////////////////////////////////////////////////
void done()
{
    std::printf("Press <return> to continue...");
    std::fgetc(stdin);
}
