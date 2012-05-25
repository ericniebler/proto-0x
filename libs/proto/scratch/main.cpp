#include <cstdio>
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

int main()
{
    A a = foo(42);
    A b = foo("");

    typedef proto::expr<proto::tag::terminal, proto::term<int>> int_;
    int_ p(42);
    int_ const pc(42);

    typedef proto::expr<proto::tag::function, proto::args<int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_, int_>> ints_;
    ints_ is(p,p,p,p,p,p,p,p,p,p,p,p,p,p);
    p = proto::child<13>(is);

    proto::as_child(42);
    proto::as_child(p);

    int i = proto::value(p);

    auto x = p(1,2,3,4);

    auto y0 = (p=p);
    auto y1 = (p=pc);
    static_assert(std::is_same<decltype(y0)::proto_tag, proto::tag::terminal>::value, "");
    static_assert(std::is_same<decltype(y1)::proto_tag, proto::tag::terminal>::value, "");

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
