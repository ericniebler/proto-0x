///////////////////////////////////////////////////////////////////////////////
// grammar.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED

#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // grammar_of_
            template<typename T>
            typename T::proto_grammar_type *grammar_of_(int);

            template<typename T>
            T *grammar_of_(long);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar
        template<typename Grammar>
        struct grammar
        {
            typedef Grammar proto_grammar_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar_of
        template<typename Grammar>
        struct grammar_of
          : std::remove_pointer<decltype(detail::grammar_of_<Grammar>(1))>
        {};
    }
}

#endif
