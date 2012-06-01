#include <cstdio>
#include <typeinfo>
#include <boost/proto/proto.hpp>
namespace proto = boost::proto;

struct B : proto::term<int> {};

static_assert(std::is_trivial<proto::term<int>>::value, "not trivial!");
static_assert(std::is_trivial<B>::value, "not trivial!");
static_assert(std::is_trivial<proto::expr<proto::tag::terminal, proto::term<int>>>::value, "not trivial!");
constexpr proto::expr<proto::tag::terminal, proto::term<int>> i_(42);

//template<typename Expr>
//struct MyExpr;
//
//struct MyDomain
//  : proto::domain<proto::generator<proto::expr_wrap<MyExpr>>>
//{};
//
//template<typename Expr>
//struct MyExpr
//  : Expr
//{
//    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(MyExpr);
//
//    constexpr explicit MyExpr(Expr e)
//      : Expr(static_cast<Expr &&>(e))
//    {};
//};

template<typename T> struct undefined;

int main()
{
    typedef proto::literal<int> int_;
    int_ p(42);
    int_ const pc(42);

    typedef proto::expr<proto::tag::function, proto::args<int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_>> ints_;
    ints_ is(p,p,p,p,p,p,p,p,p,p,p,p,p,p);
    p = proto::child<13>(is);

    // quick sanity check for as_expr
    auto a1 = proto::as_expr(42);
    auto a2 = proto::as_expr(p);
    std::printf("type of 'proto::as_expr(42)' = '%s'\n", typeid(a1).name());
    std::printf("type of 'proto::as_expr(p)'  = '%s'\n", typeid(a2).name());
    static_assert(std::is_same<decltype(a1), decltype(a2)>::value, "not the same!");

    int i = proto::value(p);

    auto x = p(1,2,3,"hello");
    std::printf("type of 'p(1,2,3,\"hello\")'  = '%s'\n", typeid(x).name());
    std::printf("type of '\"hello\"'  = '%s'\n", typeid(proto::value(proto::child<4>(x))).name());

    auto y0 = (p=p);
    auto y1 = (p=pc);
    static_assert(std::is_same<decltype(y0.proto_tag()), proto::tag::terminal>::value, "");
    static_assert(std::is_same<decltype(y1.proto_tag()), proto::tag::terminal>::value, "");

    auto y2 = (p='c');
    static_assert(std::is_same<decltype(y2.proto_tag()), proto::tag::assign>::value, "");

    int_().proto_args();

    static_assert(sizeof(proto::literal<int>) == sizeof(int), "sizeof(proto::literal<int>) != sizeof(int)");

    //typedef int_ const cint_;
    //cint_(42)[p];

    //MyExpr<proto::literal<int, MyDomain>> iii_(proto::literal<int, MyDomain>(42));
    //auto jjj_ = iii_[42];
    //std::printf("iii_[42] has type '%s'\n", typeid(jjj_).name());
    //std::printf("child<0>(iii_[42]) has type '%s'\n", typeid(proto::child<0>(jjj_)).name());
    //std::printf("child<1>(iii_[42]) has type '%s'\n", typeid(proto::child<1>(jjj_)).name());

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
