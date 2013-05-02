////////////////////////////////////////////////////////////////////////////////////////////////////
// grammar.hpp
// Helpers for building Proto grammars.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_GRAMMAR_BASIC_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_V5_GRAMMAR_BASIC_GRAMMAR_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                ////////////////////////////////////////////////////////////////////////////////////
                struct not_a_grammar
                {};

                ////////////////////////////////////////////////////////////////////////////////////
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

                ////////////////////////////////////////////////////////////////////////////////////
                // as_grammar_impl_
                template<typename T>
                struct as_grammar_impl_
                {
                    using type = T;
                };

                template<typename Ret, typename ...Args>
                struct as_grammar_impl_<Ret(Args...)>
                {
                    using type =
                        extension::grammar_impl<typename normalize_grammar_<Ret(Args...)>::type>;
                };

                template<typename Ret, typename ...Args>
                struct as_grammar_impl_<Ret(Args......)>
                {
                    using type =
                        extension::grammar_impl<typename normalize_grammar_<Ret(Args......)>::type>;
                };

                template<typename Ret, typename ...Args>
                struct as_grammar_impl_<Ret(*)(Args...)>
                  : as_grammar_impl_<Ret(Args...)>
                {};

                template<typename Ret, typename ...Args>
                struct as_grammar_impl_<Ret(*)(Args......)>
                  : as_grammar_impl_<Ret(Args......)>
                {};
            }

            namespace extension
            {
                template<typename T>
                struct not_a_grammar
                  : detail::not_a_grammar
                {
                    template<typename Expr>
                    struct apply
                      : std::false_type
                    {
                        static_assert(
                            utility::never<T>::value
                          , "Unknown grammar pattern used in proto::matches"
                        );
                    };
                };

                template<typename Grammar, typename Enable>
                struct grammar_impl
                  : not_a_grammar<Grammar>
                {};
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // grammar_base
            struct grammar_base
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // basic_grammar
            // CRTP base class for all primitive grammars
            template<typename Grammar>
            struct basic_grammar
              : grammar_base
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_grammar
            template<typename T>
            struct is_grammar
              : std::integral_constant<
                    bool
                  , std::is_base_of<grammar_base, T>::value ||
                    (
                        std::is_base_of<detail::def_base, T>::value &&
                       !std::is_base_of<detail::not_a_grammar, T>::value
                    )
                >
            {};

            template<typename T>
            struct is_grammar<T &>
              : is_grammar<T>
            {};

            template<typename T>
            struct is_grammar<T &&>
              : is_grammar<T>
            {};

            template<typename Ret, typename ...Args>
            struct is_grammar<Ret(Args...)>
              : std::integral_constant<
                    bool
                  , !std::is_base_of<detail::not_a_grammar, detail::as_grammar_<Ret(Args...)>>::value
                >
            {};

            template<typename Ret, typename ...Args>
            struct is_grammar<Ret(Args......)>
              : std::integral_constant<
                    bool
                  , !std::is_base_of<detail::not_a_grammar, detail::as_grammar_<Ret(Args......)>>::value
                >
            {};

            template<typename Ret, typename ...Args>
            struct is_grammar<Ret(*)(Args...)>
              : is_grammar<Ret(Args...)>
            {};

            template<typename Ret, typename ...Args>
            struct is_grammar<Ret(*)(Args......)>
              : is_grammar<Ret(Args......)>
            {};
        }
    }
}

#endif
