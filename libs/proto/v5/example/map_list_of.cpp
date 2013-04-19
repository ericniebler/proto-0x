////////////////////////////////////////////////////////////////////////////////////////////////////
// map_list_of.cpp
// Implement the map_list_of utility from Boost.Assign.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <map>
#include <iostream>
#include <boost/proto/v5/proto.hpp>
#include <boost/proto/v5/debug.hpp>
using namespace boost::proto;

struct map_insert
{
    template<class M, class K, class V>
    void operator()(M & m, K k, V v) const
    {
        m[ k ] = v;
    }
};

struct map_list_of_
{};

struct MapListOf
  : def< match(
        case_(  terminal(map_list_of_),
            void()
        ),
        case_(  function(MapListOf, terminal(_), terminal(_)),
            MapListOf(_0),
            map_insert(_data, _value(_1), _value(_2))
        )
    )>
{};

template<typename Expr>
struct map_list_of_expr;

struct map_list_of_domain
  : domain<map_list_of_domain, MapListOf>
{
    using make_expr = make_custom_expr<map_list_of_expr>;
};

template<typename ExprDesc>
struct map_list_of_expr
  : expr<ExprDesc, map_list_of_domain>
{
    using expr<ExprDesc, map_list_of_domain>::expr;

    template< class K, class V, class C, class A>
    operator std::map<K, V, C, A> () const
    {
        assert_matches<MapListOf>(*this);
        std::map<K, V, C, A> map;
        MapListOf()(std::move(*this), data = map);
        return map;
    }
};

constexpr map_list_of_expr<terminal(map_list_of_)> map_list_of{};
static_assert(std::is_trivial<decltype(map_list_of)>::value, "");

int main()
{
    // Initialize a map with a list of key/value pairs
    std::map<int, int> vals = map_list_of(1,2)(2,3)(3,4)(4,5)(5,6);

    // Print out the key/value pairs
    for(auto const &p : vals)
    {
        std::cout << p.first << " : " << p.second << std::endl;
    }
}
