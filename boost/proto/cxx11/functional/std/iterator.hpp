////////////////////////////////////////////////////////////////////////////////////////////////////
// iterator.hpp
// Proto callables for std functions found in \<iterator\>
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUNCTIONAL_STD_ITERATOR_HPP_INCLUDED
#define BOOST_PROTO_FUNCTIONAL_STD_ITERATOR_HPP_INCLUDED

#include <iterator>
#include <boost/proto/cxx11/proto_fwd.hpp>

namespace boost { namespace proto { namespace functional { namespace std
{

    // A PolymorphicFunctionObject wrapping std::advance
    struct advance
    {
        template<typename InputIterator, typename Distance>
        void operator()(InputIterator &x, Distance n) const noexcept(noexcept(std::advance(x, n)))
        {
            ::std::advance(x, n);
        }
    };

    // A PolymorphicFunctionObject wrapping std::distance
    struct distance
    {
        template<typename InputIterator>
        auto operator()(InputIterator first, InputIterator last) const
        BOOST_PROTO_AUTO_RETURN(
            ::std::distance(first, last)
        )
    };

    // A PolymorphicFunctionObject wrapping std::next
    struct next
    {
        template<typename ForwardIterator>
        ForwardIterator operator()(
            ForwardIterator x
          , typename ::std::iterator_traits<ForwardIterator>::difference_type n = 1
        ) const noexcept(noexcept(ForwardIterator(std::next(x, n))))
        {
            return ::std::next(x, n);
        }
    };

    // A PolymorphicFunctionObject wrapping std::prior
    struct prior
    {
        template<typename BidirectionalIterator>
        BidirectionalIterator operator()(
            BidirectionalIterator x
          , typename ::std::iterator_traits<BidirectionalIterator>::difference_type n = 1
        ) const noexcept(noexcept(BidirectionalIterator(std::advance(x, -n))))
        {
            return ::std::advance(x, -n);
        }
    };

}}}}

#endif
