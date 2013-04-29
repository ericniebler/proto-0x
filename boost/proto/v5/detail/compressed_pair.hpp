////////////////////////////////////////////////////////////////////////////////////////////////////
// compressed_pair.hpp
// A subset of the functionality provided by boost::compressed_pair, but C++11-aware
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_DETAIL_COMPRESSED_PAIR_HPP_INCLUDED
#define BOOST_PROTO_CXX11_DETAIL_COMPRESSED_PAIR_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename First, typename Second>
                struct compressed_pair
                  : private First
                  , private Second
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(compressed_pair);

                    template<typename F, typename S>
                    constexpr compressed_pair(F && f, S && s)
                        noexcept(
                            noexcept(First(static_cast<F &&>(f))) &&
                            noexcept(Second(static_cast<S &&>(s)))
                        )
                      : First(static_cast<F &&>(f))
                      , Second(static_cast<S &&>(s))
                    {}

                    First & first() & noexcept
                    {
                        return *this;
                    }

                    constexpr First const & first() const & noexcept
                    {
                        return *this;
                    }

                    constexpr First && first() const && noexcept
                    {
                        return const_cast<compressed_pair &&>(*this);
                    }

                    Second & second() & noexcept
                    {
                        return *this;
                    }

                    constexpr Second const & second() const & noexcept
                    {
                        return *this;
                    }

                    constexpr Second && second() const && noexcept
                    {
                        return const_cast<compressed_pair &&>(*this);
                    }
                };
            }
        }
    }
}

#endif // BOOST_PROTO_CXX11_DETAIL_COMPRESSED_PAIR_HPP_INCLUDED
