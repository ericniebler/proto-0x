////////////////////////////////////////////////////////////////////////////////////////////////////
// and.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_AND_HPP_INCLUDED
#define BOOST_PROTO_ACTION_AND_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct logical_and_
            {
                template<typename A, typename B>
                std::integral_constant<bool, static_cast<bool>(A::value) && static_cast<bool>(B::value)>
                operator()(A const &, B const &) const noexcept
                {
                    return std::integral_constant<bool, static_cast<bool>(A::value) && static_cast<bool>(B::value)>();
                }

                template<typename B, bool BV = static_cast<bool>(B::value)>
                inline bool operator()(bool a, B const &) const noexcept
                {
                    return a && BV;
                }

                template<typename A, bool AV = static_cast<bool>(A::value)>
                inline bool operator()(A const &, bool b) const noexcept
                {
                    return AV && b;
                }

                inline bool operator()(bool a, bool b) const noexcept
                {
                    return a && b;
                }
            };

            struct fold_and_
            {
                static std::true_type call() noexcept
                {
                    return std::true_type();
                }

                template<typename BoolHead, typename ...BoolTail, typename Impl = fold_and_>
                static auto call(BoolHead const &bh, BoolTail const &...bt)
                BOOST_PROTO_AUTO_RETURN(
                    BOOST_PROTO_TRY_CALL(logical_and_())(bh, Impl::call(bt...))
                )
            };

            template<typename...BoolActions>
            struct _and_
              : basic_action<_and_<BoolActions...>>
            {
                template<typename ...Args>
                auto operator()(Args &&...args) const
                BOOST_PROTO_AUTO_RETURN(
                    fold_and_::call(as_action_<BoolActions>()(static_cast<Args &&>(args)...)...)
                )
            };
        }

        namespace extension
        {
            template<typename...BoolActions>
            struct action_impl<and_(BoolActions...)>
              : detail::_and_<BoolActions...>
            {};
        }
    }
}

#endif
