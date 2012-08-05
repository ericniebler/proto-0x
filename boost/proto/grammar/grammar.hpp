///////////////////////////////////////////////////////////////////////////////
// grammar.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_GRAMMAR_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/tags.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // expr_pattern
            template<typename Fun>
            struct expr_pattern
            {};

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

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(Args...)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args...)>
              , Ret(Args...)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(*)(Args...)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args...)>
              , Ret(Args...)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(Args......)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args......)>
              , Ret(Args......)
            >
        {};

        template<typename Ret, typename ...Args>
        struct grammar_of<Ret(*)(Args......)>
          : std::conditional<
                proto::is_tag<Ret>::value
              , detail::expr_pattern<Ret(Args......)>
              , Ret(Args......)
            >
        {};
    }
}

#endif
