////////////////////////////////////////////////////////////////////////////////////////////////////
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

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // normalize_grammar_
            template<typename T, typename Enable = void>
            struct normalize_grammar_
            {
                using type = T;
            };

            template<typename Ret, typename ...Args>
            struct normalize_grammar_<Ret(Args...), utility::always_void<typename Ret::proto_grammar_type>>
            {
                using type = typename Ret::proto_grammar_type(Args...);
            };

            template<typename Ret, typename ...Args>
            struct normalize_grammar_<Ret(Args......), utility::always_void<typename Ret::proto_grammar_type>>
            {
                using type = typename Ret::proto_grammar_type(Args......);
            };
        }

        namespace extension
        {
            template<typename Grammar>
            struct grammar_impl
            {
                static_assert(
                    utility::never<Grammar>::value
                  , "Unknown grammar pattern used in proto::matches"
                );
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar_base
        struct grammar_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // basic_grammar
        // CRTP base class for all grammars
        template<typename Action>
        struct basic_grammar
          : grammar_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar
        template<typename Grammar>
        struct grammar
          : Grammar
        {};

        template<typename Ret, typename ...Args>
        struct grammar<Ret(Args...)>
        {
            template<typename Expr, typename G = typename detail::normalize_grammar_<Ret(Args...)>::type>
            struct apply
              : extension::grammar_impl<G>::template apply<Expr>
            {};
        };

        template<typename Ret, typename ...Args>
        struct grammar<Ret(Args......)>
        {
            template<typename Expr, typename G = typename detail::normalize_grammar_<Ret(Args......)>::type>
            struct apply
              : extension::grammar_impl<G>::template apply<Expr>
            {};
        };

        template<typename Ret, typename ...Args>
        struct grammar<Ret(*)(Args...)>
          : grammar<Ret(Args...)>
        {};

        template<typename Ret, typename ...Args>
        struct grammar<Ret(*)(Args......)>
          : grammar<Ret(Args......)>
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_grammar
        template<typename T>
        struct is_grammar
          : std::is_base_of<grammar_base, T>
        {};

        template<typename T>
        struct is_grammar<T &>
          : std::is_base_of<grammar_base, T>
        {};

        template<typename T>
        struct is_grammar<T &&>
          : std::is_base_of<grammar_base, T>
        {};
    }
}

#endif
