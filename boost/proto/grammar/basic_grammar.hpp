////////////////////////////////////////////////////////////////////////////////////////////////////
// grammar.hpp
// Helpers for building Proto grammars.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_GRAMMAR_BASIC_GRAMMAR_HPP_INCLUDED
#define BOOST_PROTO_GRAMMAR_BASIC_GRAMMAR_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // grammar_base
        struct grammar_base
        {};

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            struct not_a_grammar
            {};

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

            template<typename T>
            struct as_grammar_impl_
            {
                using type = T;
            };

            template<typename Ret, typename ...Args>
            struct as_grammar_impl_<Ret(Args...)>
            {
                using G0 = typename normalize_grammar_<Ret(Args...)>::type;
                struct G1 : extension::grammar_impl<G0>, grammar_base {};
                using type =
                    typename std::conditional<
                        std::is_base_of<not_a_grammar, G1>::value
                      , extension::grammar_impl<G0>
                      , G1
                    >::type;
            };

            template<typename Ret, typename ...Args>
            struct as_grammar_impl_<Ret(Args......)>
            {
                using G0 = typename normalize_grammar_<Ret(Args......)>::type;
                struct G1 : extension::grammar_impl<G0>, grammar_base {};
                using type =
                    typename std::conditional<
                        std::is_base_of<G1, not_a_grammar>::value
                      , extension::grammar_impl<G0>
                      , G1
                    >::type;
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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // basic_grammar
        // CRTP base class for all grammars
        template<typename Grammar>
        struct basic_grammar
          : grammar_base
        {};

        //////////////////////////////////////////////////////////////////////////////////////////////
        //// grammar
        //template<typename Grammar>
        //struct grammar
        //  : Grammar
        //{};

        //template<typename Grammar>
        //struct grammar<grammar<Grammar>>
        //  : Grammar
        //{};

        //template<typename Ret, typename ...Args>
        //struct grammar<Ret(Args...)>
        //  : grammar_base
        //{
        //    struct foo : extension::grammar_impl<typename detail::normalize_grammar_<Ret(Args...)>::type> {};
        //    static_assert(!std::is_base_of<detail::not_a_grammar, foo>::value, "");

        //    template<typename Expr, typename G = typename detail::normalize_grammar_<Ret(Args...)>::type>
        //    using apply = typename extension::grammar_impl<G>::template apply<Expr>;
        //};

        //template<typename Ret, typename ...Args>
        //struct grammar<Ret(Args......)>
        //  : grammar_base
        //{
        //    struct foo : extension::grammar_impl<typename detail::normalize_grammar_<Ret(Args......)>::type> {};
        //    static_assert(!std::is_base_of<detail::not_a_grammar, foo>::value, "");

        //    template<typename Expr, typename G = typename detail::normalize_grammar_<Ret(Args......)>::type>
        //    using apply = typename extension::grammar_impl<G>::template apply<Expr>;
        //};

        //template<typename Ret, typename ...Args>
        //struct grammar<Ret(*)(Args...)>
        //  : grammar<Ret(Args...)>
        //{};

        //template<typename Ret, typename ...Args>
        //struct grammar<Ret(*)(Args......)>
        //  : grammar<Ret(Args......)>
        //{};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_grammar
        template<typename T>
        struct is_grammar
          : std::is_base_of<grammar_base, detail::as_grammar_<T>>
        {};

        template<typename T>
        struct is_grammar<T &>
          : is_grammar<T>
        {};

        template<typename T>
        struct is_grammar<T &&>
          : is_grammar<T>
        {};
    }
}

#endif
