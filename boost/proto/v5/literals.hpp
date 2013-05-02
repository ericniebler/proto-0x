////////////////////////////////////////////////////////////////////////////////////////////////////
// literals.hpp
//
// Copyright 2012 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_LITERALS_HPP_INCLUDED
#define BOOST_PROTO_V5_LITERALS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/expr.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            // handy user-defined literal operators for building expressions
            namespace literals
            {
                inline constexpr literal<char const *> operator "" _et(char const *ntbs, std::size_t) noexcept
                {
                    return literal<char const *>(ntbs);
                }

                inline constexpr literal<wchar_t const *> operator "" _et(wchar_t const *ntws, std::size_t) noexcept
                {
                    return literal<wchar_t const *>(ntws);
                }

                inline constexpr literal<char16_t const *> operator "" _et(char16_t const *ntws, std::size_t) noexcept
                {
                    return literal<char16_t const *>(ntws);
                }

                inline constexpr literal<char32_t const *> operator "" _et(char32_t const *ntws, std::size_t) noexcept
                {
                    return literal<char32_t const *>(ntws);
                }

                inline constexpr literal<char> operator "" _et(char ch) noexcept
                {
                    return literal<char>(ch);
                }

                inline constexpr literal<wchar_t> operator "" _et(wchar_t ch) noexcept
                {
                    return literal<wchar_t>(ch);
                }

                inline constexpr literal<char16_t> operator "" _et(char16_t ch) noexcept
                {
                    return literal<char16_t>(ch);
                }

                inline constexpr literal<char32_t> operator "" _et(char32_t ch) noexcept
                {
                    return literal<char32_t>(ch);
                }

                inline constexpr literal<unsigned long long> operator "" _et(unsigned long long l) noexcept
                {
                    return literal<unsigned long long>(l);
                }

                inline constexpr literal<long double> operator "" _et(long double d) noexcept
                {
                    return literal<long double>(d);
                }
            }
        }
    }
}

#endif
