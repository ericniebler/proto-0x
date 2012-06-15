///////////////////////////////////////////////////////////////////////////////
// when.hpp
// Associate a custom transform with a grammar rule
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_WHEN_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_WHEN_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // when
        template<typename Grammar, typename Transform>
        struct when
          : transform<when<Grammar, Transform>>
        {
            typedef typename Grammar::proto_grammar_type proto_grammar_type;

            template<typename ...T>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                as_transform<Transform>()(static_cast<T &&>(t)...)
            )
        };
    }
}

#endif
