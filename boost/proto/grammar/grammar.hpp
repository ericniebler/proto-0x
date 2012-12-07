////////////////////////////////////////////////////////////////////////////////////////////////////
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
            struct grammar_of_impl_
            {
                template<
                    typename T
                  , typename U = typename T::proto_grammar_type
                  , typename Impl = grammar_of_impl_
                >
                static auto call(int) -> decltype(Impl::template call<U>(1));

                template<typename T>
                static T *call(long);
            };
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
          : std::remove_pointer<decltype(detail::grammar_of_impl_::call<Grammar>(1))>
        {};
    }
}

#endif
