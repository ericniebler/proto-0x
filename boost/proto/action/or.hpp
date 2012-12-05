////////////////////////////////////////////////////////////////////////////////////////////////////
// or.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_OR_HPP_INCLUDED
#define BOOST_PROTO_ACTION_OR_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/action.hpp>
#include <boost/proto/grammar/grammar.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            struct logical_or_
            {
                template<typename A, typename B>
                std::integral_constant<bool, static_cast<bool>(A::value) || static_cast<bool>(B::value)>
                operator()(A const &, B const &) const noexcept
                {
                    return std::integral_constant<bool, static_cast<bool>(A::value) || static_cast<bool>(B::value)>();
                }

                template<typename B, bool BV = static_cast<bool>(B::value)>
                inline bool operator()(bool a, B const &) const noexcept
                {
                    return a || BV;
                }

                template<typename A, bool AV = static_cast<bool>(A::value)>
                inline bool operator()(A const &, bool b) const noexcept
                {
                    return AV || b;
                }

                inline bool operator()(bool a, bool b) const noexcept
                {
                    return a || b;
                }
            };

            struct fold_or_
            {
                static std::false_type call() noexcept
                {
                    return std::false_type();
                }

                template<typename BoolHead, typename ...BoolTail, typename Impl = fold_or_>
                static auto call(BoolHead const &bh, BoolTail const &...bt)
                BOOST_PROTO_AUTO_RETURN(
                    BOOST_PROTO_TRY_CALL(logical_or_())(bh, Impl::call(bt...))
                )
            };

            template<typename...BoolActions>
            struct _or_
              : basic_action<_or_<BoolActions...>>
            {
                template<typename ...Args>
                auto operator()(Args &&...args) const
                BOOST_PROTO_AUTO_RETURN(
                    fold_or_::call(proto::action<BoolActions>()(static_cast<Args &&>(args)...)...)
                )
            };
        }

        template<typename...BoolActions>
        struct action<or_(BoolActions...)>
          : detail::_or_<BoolActions...>
          , grammar<or_(BoolActions...)>
        {};
    }
}

#endif
