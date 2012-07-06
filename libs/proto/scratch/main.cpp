////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <iostream>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>
#include <boost/fusion/include/list.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

namespace detail
{
    struct unpack_impl_
    {
        template<typename F0, typename F1, typename ...Rest>
        static auto unpack(proto::args<> a, F0 const & f0, F1 const & f1, Rest &&... rest)
        BOOST_PROTO_AUTO_RETURN(
            f0(static_cast<Rest &&>(rest)...)
        )

        #define M0(Z, N, DATA) f1(DATA.BOOST_PP_CAT(proto_child, N))

#define BOOST_PP_LOCAL_MACRO(N)\
        template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F0, typename F1, typename ...Rest>                                       \
        static auto unpack(proto::args<BOOST_PP_ENUM_PARAMS(N, T)> & a, F0 const & f0, F1 const & f1, Rest &&... rest)                  \
        BOOST_PROTO_AUTO_RETURN(                                                                                                        \
            f0(f1(static_cast<Rest &&>(rest))..., BOOST_PP_ENUM(N, M0, a))                                                              \
        )                                                                                                                               \
                                                                                                                                        \
        template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F0, typename F1, typename ...Rest>                                       \
        static auto unpack(proto::args<BOOST_PP_ENUM_PARAMS(N, T)> const & a, F0 const & f0, F1 const & f1, Rest &&... rest)            \
        BOOST_PROTO_AUTO_RETURN(                                                                                                        \
            f0(f1(static_cast<Rest &&>(rest))..., BOOST_PP_ENUM(N, M0, a))                                                              \
        )                                                                                                                               \
                                                                                                                                        \
        template<BOOST_PP_ENUM_PARAMS(N, typename T), typename F0, typename F1, typename ...Rest>                                       \
        static auto unpack(proto::args<BOOST_PP_ENUM_PARAMS(N, T)> && a, F0 const & f0, F1 const & f1, Rest &&... rest)                 \
        BOOST_PROTO_AUTO_RETURN(                                                                                                        \
            f0(f1(static_cast<Rest &&>(rest))..., BOOST_PP_ENUM(N, M0, (static_cast<proto::args<BOOST_PP_ENUM_PARAMS(N, T)> &&>(a))))   \
        )                                                                                                                               \
        /**/

        #define BOOST_PP_LOCAL_LIMITS (1, BOOST_PROTO_ARGS_UNROLL_MAX)
        #include BOOST_PP_LOCAL_ITERATE()
        #undef M0

        template<typename ...T, typename F0, typename F1, typename ...Rest, typename Impl = unpack_impl_>
        static auto unpack(proto::args<T...> & a, F0 const & f0, F1 const & f1, Rest &&... rest)
        BOOST_PROTO_AUTO_RETURN(
            Impl::unpack(a.proto_args_tail, f0, f1, BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, a.proto_child), static_cast<Rest &&>(rest)...)
        )

        template<typename ...T, typename F0, typename F1, typename ...Rest, typename Impl = unpack_impl_>
        static auto unpack(proto::args<T...> const & a, F0 const & f0, F1 const & f1, Rest &&... rest)
        BOOST_PROTO_AUTO_RETURN(
            Impl::unpack(a.proto_args_tail, f0, f1, BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, a.proto_child), static_cast<Rest &&>(rest)...)
        )

        template<typename ...T, typename F0, typename F1, typename ...Rest, typename Impl = unpack_impl_>
        static auto unpack(proto::args<T...> && a, F0 const & f0, F1 const & f1, Rest &&... rest)
        BOOST_PROTO_AUTO_RETURN(
            Impl::unpack(static_cast<proto::args<T...> &&>(a).proto_args_tail, f0, f1, BOOST_PP_ENUM_PARAMS(BOOST_PROTO_ARGS_UNROLL_MAX, static_cast<proto::args<T...> &&>(a).proto_child), static_cast<Rest &&>(rest)...)
        )
    };
}

template<typename E, typename F0, typename F1>
auto unpack(E && e, F0 const & f0, F1 const & f1)
BOOST_PROTO_AUTO_RETURN(
    detail::unpack_impl_::unpack(static_cast<E &&>(e), f0, f1)
)

struct ignore
{
    template<typename...T>
    void operator()(T &&...) const
    {}
};

//then<_map<_, and_<display_expr(_), _void>, ignore>

// Really hard, but nice:
// f0(tag_of<_>(), _unpack(f1(_child<0,2>)))

int main()
{
    typedef proto::and_<proto::functional::display_expr(_), proto::_void> display_expr;
    proto::terminal<int> i{42};
    ::unpack(i(1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9), ignore(), display_expr());

    void done();
    done();
}

void done()
{
    std::cout << "Press <RETURN> to continue...";
    char ch = 0;
    std::cin.get(ch);
}
