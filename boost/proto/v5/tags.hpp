////////////////////////////////////////////////////////////////////////////////////////////////////
// tags.hpp
// Contains the tags for all the overloadable operators in C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_TAGS_HPP_INCLUDED
#define BOOST_PROTO_V5_TAGS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/functional/cxx/operators.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename Tag>
                typename Tag::proto_is_terminal_type is_terminal_tag_(Tag const *);

                std::false_type is_terminal_tag_(void const *);

                template<typename T>
                using is_terminal_tag = decltype(detail::is_terminal_tag_(static_cast<T *>(0)));

                template<typename Tag>
                struct safe_tag_action
                {
                    // If the Tag action creates an expression, use functional::make_expr with
                    // a safe version of the deduced domain instead to avoid dangling references.
                    template<
                        typename ...Ts
                      , typename ThisTag = Tag
                      , typename MakeExpr =
                            functional::make_expr<
                                ThisTag
                              , domains::safe_domain_adaptor<deduce_domain>
                            >
                      , BOOST_PROTO_ENABLE_IF(
                            ThisTag::proto_arity_type::equal_to(sizeof...(Ts))
                        )
                      , BOOST_PROTO_ENABLE_IF(
                            v5::is_expr<
                                decltype(BOOST_PROTO_TRY_CALL(ThisTag())(std::declval<Ts>()...))
                            >::value
                        )
                    >
                    constexpr auto operator()(Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<Ts &&>(ts)...)
                    )

                    // If the Tag action doesn't create an expression, just use it as-is.
                    template<
                        typename ...Ts
                      , typename ThisTag = Tag
                      , BOOST_PROTO_ENABLE_IF(
                            ThisTag::proto_arity_type::equal_to(sizeof...(Ts))
                        )
                      , BOOST_PROTO_ENABLE_IF(
                            !v5::is_expr<
                                decltype(BOOST_PROTO_TRY_CALL(ThisTag())(std::declval<Ts>()...))
                            >::value
                        )
                    >
                    constexpr auto operator()(Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(ThisTag())(static_cast<Ts &&>(ts)...)
                    )
                };
            }

            namespace tags
            {
                struct nullary
                {
                    static constexpr bool equal_to(std::size_t)
                    {
                        return false;
                    }
                };

                struct unary
                {
                    static constexpr bool equal_to(std::size_t N)
                    {
                        return 1 == N;
                    }
                };

                struct binary
                {
                    static constexpr bool equal_to(std::size_t N)
                    {
                        return 2 == N;
                    }
                };

                struct ternary
                {
                    static constexpr bool equal_to(std::size_t N)
                    {
                        return 3 == N;
                    }
                };

                struct nary
                {
                    static constexpr bool equal_to(std::size_t N)
                    {
                        return 1 <= N;
                    }
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // expr_tag_tag
                struct expr_tag_tag
                {};

                ////////////////////////////////////////////////////////////////////////////////////
                // expr_tag_base
                struct expr_tag_base
                {
                    using proto_tag_type = expr_tag_tag;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // expr_tag
                template<typename Tag, typename Arity, typename Action>
                struct expr_tag
                  : expr_tag_base, Action
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_tag);
                    using proto_arity_type = Arity;
                    using proto_action_type = detail::safe_tag_action<Tag>;
                    using proto_grammar_type = expr_tag<Tag, Arity, Action>;
                    using proto_is_terminal_type =
                        std::integral_constant<bool, std::is_same<Arity, nullary>::value>;
                };

                /// Tag type for the unary + operator.
                struct unary_plus
                  : expr_tag<unary_plus, unary, v5::functional::cxx::unary_plus>
                {};

                /// Tag type for the unary - operator.
                struct negate
                  : expr_tag<negate, unary, v5::functional::cxx::negate>
                {};

                /// Tag type for the unary * operator.
                struct dereference
                  : expr_tag<dereference, unary, v5::functional::cxx::dereference>
                {};

                /// Tag type for the unary ~ operator.
                struct complement
                  : expr_tag<complement, unary, v5::functional::cxx::complement>
                {};

                /// Tag type for the unary & operator.
                struct address_of
                  : expr_tag<address_of, unary, v5::functional::cxx::address_of>
                {};

                /// Tag type for the unary ! operator.
                struct logical_not
                  : expr_tag<logical_not, unary, v5::functional::cxx::logical_not>
                {};

                /// Tag type for the unary prefix ++ operator.
                struct pre_inc
                  : expr_tag<pre_inc, unary, v5::functional::cxx::pre_inc>
                {};

                /// Tag type for the unary prefix -- operator.
                struct pre_dec
                  : expr_tag<pre_dec, unary, v5::functional::cxx::pre_dec>
                {};

                /// Tag type for the unary postfix ++ operator.
                struct post_inc
                  : expr_tag<post_inc, unary, v5::functional::cxx::post_inc>
                {};

                /// Tag type for the unary postfix -- operator.
                struct post_dec
                  : expr_tag<post_dec, unary, v5::functional::cxx::post_dec>
                {};

                /// Tag type for the binary \<\< operator.
                struct shift_left
                  : expr_tag<shift_left, binary, v5::functional::cxx::shift_left>
                {};

                /// Tag type for the binary \>\> operator.
                struct shift_right
                  : expr_tag<shift_right, binary, v5::functional::cxx::shift_right>
                {};

                /// Tag type for the binary * operator.
                struct multiplies
                  : expr_tag<multiplies, binary, v5::functional::cxx::multiplies>
                {};

                /// Tag type for the binary / operator.
                struct divides
                  : expr_tag<divides, binary, v5::functional::cxx::divides>
                {};

                /// Tag type for the binary % operator.
                struct modulus
                  : expr_tag<modulus, binary, v5::functional::cxx::modulus>
                {};

                /// Tag type for the binary + operator.
                struct plus
                  : expr_tag<plus, binary, v5::functional::cxx::plus>
                {};

                /// Tag type for the binary - operator.
                struct minus
                  : expr_tag<minus, binary, v5::functional::cxx::minus>
                {};

                /// Tag type for the binary \< operator.
                struct less
                  : expr_tag<less, binary, v5::functional::cxx::less>
                {};

                /// Tag type for the binary \> operator.
                struct greater
                  : expr_tag<greater, binary, v5::functional::cxx::greater>
                {};

                /// Tag type for the binary \<= operator.
                struct less_equal
                  : expr_tag<less_equal, binary, v5::functional::cxx::less_equal>
                {};

                /// Tag type for the binary \>= operator.
                struct greater_equal
                  : expr_tag<greater_equal, binary, v5::functional::cxx::greater_equal>
                {};

                /// Tag type for the binary == operator.
                struct equal_to
                  : expr_tag<equal_to, binary, v5::functional::cxx::equal_to>
                {};

                /// Tag type for the binary != operator.
                struct not_equal_to
                  : expr_tag<not_equal_to, binary, v5::functional::cxx::not_equal_to>
                {};

                /// Tag type for the binary || operator.
                struct logical_or
                  : expr_tag<logical_or, binary, v5::functional::cxx::logical_or>
                {};

                /// Tag type for the binary && operator.
                struct logical_and
                  : expr_tag<logical_and, binary, v5::functional::cxx::logical_and>
                {};

                /// Tag type for the binary & operator.
                struct bitwise_and
                  : expr_tag<bitwise_and, binary, v5::functional::cxx::bitwise_and>
                {};

                /// Tag type for the binary | operator.
                struct bitwise_or
                  : expr_tag<bitwise_or, binary, v5::functional::cxx::bitwise_or>
                {};

                /// Tag type for the binary ^ operator.
                struct bitwise_xor
                  : expr_tag<bitwise_xor, binary, v5::functional::cxx::bitwise_xor>
                {};

                /// Tag type for the binary , operator.
                struct comma
                  : expr_tag<comma, binary, v5::functional::cxx::comma>
                {};

                /// Tag type for the binary ->* operator.
                struct mem_ptr
                  : expr_tag<mem_ptr, binary, v5::functional::cxx::mem_ptr>
                {};

                /// Tag type for the binary = operator.
                struct assign
                  : expr_tag<assign, binary, v5::functional::cxx::assign>
                {};

                /// Tag type for the binary \<\<= operator.
                struct shift_left_assign
                  : expr_tag<shift_left_assign, binary, v5::functional::cxx::shift_left_assign>
                {};

                /// Tag type for the binary \>\>= operator.
                struct shift_right_assign
                  : expr_tag<shift_right_assign, binary, v5::functional::cxx::shift_right_assign>
                {};

                /// Tag type for the binary *= operator.
                struct multiplies_assign
                  : expr_tag<multiplies_assign, binary, v5::functional::cxx::multiplies_assign>
                {};

                /// Tag type for the binary /= operator.
                struct divides_assign
                  : expr_tag<divides_assign, binary, v5::functional::cxx::divides_assign>
                {};

                /// Tag type for the binary %= operator.
                struct modulus_assign
                  : expr_tag<modulus_assign, binary, v5::functional::cxx::modulus_assign>
                {};

                /// Tag type for the binary += operator.
                struct plus_assign
                  : expr_tag<plus_assign, binary, v5::functional::cxx::plus_assign>
                {};

                /// Tag type for the binary -= operator.
                struct minus_assign
                  : expr_tag<minus_assign, binary, v5::functional::cxx::minus_assign>
                {};

                /// Tag type for the binary &= operator.
                struct bitwise_and_assign
                  : expr_tag<bitwise_and_assign, binary, v5::functional::cxx::bitwise_and_assign>
                {};

                /// Tag type for the binary |= operator.
                struct bitwise_or_assign
                  : expr_tag<bitwise_or_assign, binary, v5::functional::cxx::bitwise_or_assign>
                {};

                /// Tag type for the binary ^= operator.
                struct bitwise_xor_assign
                  : expr_tag<bitwise_xor_assign, binary, v5::functional::cxx::bitwise_xor_assign>
                {};

                /// Tag type for the binary subscript operator.
                struct subscript
                  : expr_tag<subscript, binary, v5::functional::cxx::subscript>
                {};

                /// Tag type for the ternary ?: conditional operator.
                struct if_else_
                  : expr_tag<if_else_, ternary, v5::functional::cxx::if_else>
                {};

                /// Tag type for the n-ary function call operator.
                struct function
                  : expr_tag<function, nary, v5::functional::cxx::function>
                {};

                /// Tag type for terminals; aka, leaves in the expression tree.
                struct terminal
                  : expr_tag<terminal, nullary>
                {
                    using proto_action_type = terminal;

                    template<
                        typename T
                      , typename MakeExpr =
                            functional::make_expr<
                                terminal
                              , domains::safe_domain_adaptor<deduce_domain>
                            >
                    >
                    constexpr auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<T &&>(t))
                    )
                };

                /// Tag type for the binary virtual data members.
                struct member
                  : expr_tag<member, binary>
                {
                    using proto_action_type = member;

                    template<
                        typename T
                      , typename U
                      , typename MakeExpr =
                            functional::make_expr<
                                member
                              , domains::safe_domain_adaptor<deduce_domain>
                            >
                    >
                    constexpr auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<T &&>(t), static_cast<U &&>(u))
                    )
                };

            } // namespace tags

            // Miscelaneous tags
            struct or_
            {};

            struct not_
            {};

            struct and_
            {};

            struct if_
            {};

            struct switch_
            {};

            struct case_
            {};

            struct match
            {};

            namespace result_of
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // tag_of
                template<typename T>
                struct tag_of
                {
                    using type = typename T::proto_tag_type;
                };

                template<typename T>
                struct tag_of<T &>
                {
                    using type = typename T::proto_tag_type;
                };

                template<typename T>
                struct tag_of<T &&>
                {
                    using type = typename T::proto_tag_type;
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_tag
            template<typename T>
            struct is_tag
              : std::is_base_of<expr_tag_base, T>
            {};

            template<typename T>
            struct is_tag<T &>
              : std::is_base_of<expr_tag_base, T>
            {};

            template<typename T>
            struct is_tag<T &&>
              : std::is_base_of<expr_tag_base, T>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // _tag_of
            struct _tag_of
              : proto::v5::basic_action<_tag_of>
            {
                template<typename E, typename ...Rest>
                constexpr auto operator()(E && e, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::v5::tag_of(static_cast<E &&>(e))
                )
            };
        }
    } // namespace proto
} // namespace boost

#endif
