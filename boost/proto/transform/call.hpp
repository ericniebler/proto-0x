///////////////////////////////////////////////////////////////////////////////
// call.hpp
// Helpers for building callable transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_CALL_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_CALL_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/mpl/identity.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>
#include <boost/proto/transform/protect.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // call_2_
            template<bool NoPad, typename ...Results>
            struct call_2_
            {
                template<typename Tfx, typename ...Args>
                auto operator()(
                    Tfx &&tfx
                  , Results &&... results
                  , utility::first<utility::any, Results>...
                  , Args &&... args
                ) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Tfx &&>(tfx)(
                        static_cast<Results &&>(results)...
                      , static_cast<Args &&>(args)...
                    )
                )
            };

            template<typename ...Results>
            struct call_2_<true, Results...>
            {
                template<typename Tfx, typename ...Ts>
                auto operator()(Tfx &&tfx, Results &&... results, Ts &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Tfx &&>(tfx)(static_cast<Results &&>(results)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_1_
            template<typename ...Tfxs>
            struct call_1_
            {
                // Handle transforms
                template<
                    typename Tfx
                  , typename ...Args
                  , BOOST_PROTO_ENABLE_IF(is_transform<Tfx>::value)
                >
                auto operator()(Tfx &&tfx, Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    call_2_<
                        (sizeof...(Args) <= sizeof...(Tfxs))
                      , decltype(as_transform<Tfxs>()(static_cast<Args &&>(args)...))...
                    >()(
                        static_cast<Tfx &&>(tfx)
                      , as_transform<Tfxs>()(static_cast<Args &&>(args)...)...
                      , static_cast<Args &&>(args)...
                    )
                )

                // Handle callables
                template<
                    typename Fun
                  , typename ...Args
                  , BOOST_PROTO_ENABLE_IF(!is_transform<Fun>::value)
                >
                auto operator()(Fun &&fun, Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<Fun &&>(fun)(as_transform<Tfxs>()(static_cast<Args &&>(args)...)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // _call
            template<typename Ret, typename ...Tfxs>
            struct _call
              : transform<_call<Ret, Tfxs...>>
            {
                template<typename ...Args, typename Fun = Ret>
                auto operator()(Args &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    call_1_<Tfxs...>()(Fun(), static_cast<Args &&>(t)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(*)(Tfxs...), I>
          : as_transform<Ret(Tfxs...), I>
        {};

        // Handle callable transforms
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(Tfxs...), I>
          : detail::_call<Ret, Tfxs...>
        {};
    }
}

#endif
