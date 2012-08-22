////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <boost/proto/proto.hpp>
#include <boost/proto/debug.hpp>

namespace proto = boost::proto;
using proto::_;

struct foo
{
    int m;
};

struct S
{
    int m;
};

int main()
{
    proto::literal<S> s;
    proto::value(s).m = 42;

    proto::_eval<> eval;
    auto x = eval( proto::literal<int>(42) + 36 );
    //auto y = eval( 1+ 2 * (proto::literal<int>(42) + foo()) );

    //int i = eval(s ->* &S::m);
    //std::cout << "i == " << i << std::endl;

    //eval(s ->* &foo::m);

    void done();
    done();
}

void done()
{
    char ch = 0;
    std::cout << "Press <CTRL> + D to quit...";
    while(std::cin.get(ch));
}




//#include <utility>
//
//template <class T, class U> struct assert_same;
//template <class T> struct assert_same<T,T> {};
//
//enum place { _ };
//template <place...>
//struct places {};
//
//template <class P1, class P2> struct append_places;
//
//template <place...X1, place...X2>
//struct append_places<places<X1...>, places<X2...> >
//{
//    typedef places<X1...,X2...> type;
//};
//
//template <unsigned N>
//struct make_places
//  : append_places<
//        typename make_places<N/2>::type,
//        typename make_places<N-N/2>::type
//    >
//{};
//
//template <> struct make_places<0> { typedef places<> type; };
//template <> struct make_places<1> { typedef places<_> type; };
//
//inline void test_places()
//{
//  assert_same<make_places<2>::type, places<_,_> > a2;
//  assert_same<make_places<3>::type, places<_,_,_> > a3;
//  assert_same<make_places<4>::type, places<_,_,_,_> > a4;
//  assert_same<make_places<5>::type, places<_,_,_,_,_> > a5;
//}
//
//template <class...T> struct types
//{
//    typedef types type;
//};
//
//template <class T>
//struct no_decay
//{
//    typedef T type;
//};
//
//// Use as function parameter that eats any POD
//template <place ignored>
//struct eat
//{ eat(...); };
//
//template<class T>
//struct take_impl
//{
//};
//
//template<place Y, place...X>
//struct take_impl<places<Y, X...>>
//  : take_impl<places<X...>>
//{
//
//};
//
//template<unsigned N, class S>
//struct take;
//
//template<unsigned N, class ...T>
//struct take<N, types<T...>>
//  : take_impl<
//        typename append_bools<
//            typename make_bools<N, true>::type,
//            typename make_bools<sizeof...(T)-N, false>::type
//        >::type,
//        types<T...>
//    >
//{};
//
//inline void test_take()
//{
//    using seq = types<void(int),char[3],long,double>;
//
//    //assert_same<take<0,seq>::type, types<> > a0;
//    //assert_same<take<1,seq>::type, types<void(int)> > a1;
//    //assert_same<take<2,seq>::type, types<void(int),char[3]> > a2;
//    assert_same<take<3,seq>::type, types<void(int),char[3],long> > a3;
//    //assert_same<take<4,seq>::type, seq> a4;
//}
//
//int main()
//{
//}
//
//
