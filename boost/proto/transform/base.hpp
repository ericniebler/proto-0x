///////////////////////////////////////////////////////////////////////////////
// base.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform_base
        struct transform_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform
        // CRTP base class for all transforms
        template<typename Tfx>
        struct transform
          : transform_base
        {
            typedef Tfx proto_transform_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_transform
        template<typename T>
        struct is_transform
          : std::is_base_of<transform_base, T>
        {};

        template<typename T>
        struct is_transform<T &>
          : std::is_base_of<transform_base, T>
        {};

        template<typename T>
        struct is_transform<T &&>
          : std::is_base_of<transform_base, T>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename T, int>
        struct as_transform
          : T::proto_transform_type
        {};

        template<typename T, int I>
        struct as_transform<as_transform<T>, I>
          : as_transform<T>
        {};
    }
}

#endif
