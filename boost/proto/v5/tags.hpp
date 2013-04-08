////////////////////////////////////////////////////////////////////////////////////////////////////
// tags.hpp
// Contains the tags for all the overloadable operators in C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_TAGS_HPP_INCLUDED
#define BOOST_PROTO_CXX11_TAGS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

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
                    // If the Tag action creates an expression, use functional::make_expr instead
                    template<
                        typename ...Ts
                      , typename ThisTag = Tag
                      , typename MakeExpr =
                            functional::make_expr<
                                ThisTag
                              , domains::safe_domain_adaptor<deduce_domain>
                            >
                      , BOOST_PROTO_ENABLE_IF(
                            ThisTag::proto_arity_type::matches(sizeof...(Ts))
                        )
                      , BOOST_PROTO_ENABLE_IF(
                            v5::is_expr<typename std::result_of<ThisTag(Ts...)>::type>::value
                        )
                    >
                    auto operator()(Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<Ts &&>(ts)...)
                    )

                    // If the Tag action doesn't create an expression, just use it as-is.
                    template<
                        typename ...Ts
                      , typename ThisTag = Tag
                      , BOOST_PROTO_ENABLE_IF(
                            ThisTag::proto_arity_type::matches(sizeof...(Ts))
                        )
                      , BOOST_PROTO_ENABLE_IF(
                            !v5::is_expr<typename std::result_of<ThisTag(Ts...)>::type>::value
                        )
                    >
                    auto operator()(Ts &&...ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        BOOST_PROTO_TRY_CALL(ThisTag())(static_cast<Ts &&>(ts)...)
                    )
                };
            }

            namespace tags
            {
                struct nullary
                {
                    static constexpr bool matches(std::size_t)
                    {
                        return false;
                    }
                };

                struct unary
                {
                    static constexpr bool matches(std::size_t N)
                    {
                        return 1 == N;
                    }
                };

                struct binary
                {
                    static constexpr bool matches(std::size_t N)
                    {
                        return 2 == N;
                    }
                };

                struct ternary
                {
                    static constexpr bool matches(std::size_t N)
                    {
                        return 3 == N;
                    }
                };

                struct nary
                {
                    static constexpr bool matches(std::size_t N)
                    {
                        return 1 <= N;
                    }
                };

                struct expr_tag_tag
                {};

                struct expr_tag_base
                {
                    using proto_tag_type = expr_tag_tag;
                };

                template<typename Tag, typename Arity>
                struct expr_tag
                  : expr_tag_base
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_tag);
                    using proto_arity_type = Arity;
                    using proto_action_type = detail::safe_tag_action<Tag>;
                    using proto_grammar_type = expr_tag<Tag, Arity>;
                    using proto_is_terminal_type =
                        std::integral_constant<bool, std::is_same<Arity, nullary>::value>;
                };

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
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<T &&>(t))
                    )
                };

                /// Tag type for the unary + operator.
                struct unary_plus
                  : expr_tag<unary_plus, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        + static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary - operator.
                struct negate
                  : expr_tag<negate, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        - static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary * operator.
                struct dereference
                  : expr_tag<dereference, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        * static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary ~ operator.
                struct complement
                  : expr_tag<complement, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        ~ static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary & operator.
                struct address_of
                  : expr_tag<address_of, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        & static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary ! operator.
                struct logical_not
                  : expr_tag<logical_not, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        ! static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary prefix ++ operator.
                struct pre_inc
                  : expr_tag<pre_inc, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        ++ static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary prefix -- operator.
                struct pre_dec
                  : expr_tag<pre_dec, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        -- static_cast<T &&>(t)
                    )
                };

                /// Tag type for the unary postfix ++ operator.
                struct post_inc
                  : expr_tag<post_inc, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) ++
                    )
                };

                /// Tag type for the unary postfix -- operator.
                struct post_dec
                  : expr_tag<post_dec, unary>
                {
                    template<typename T>
                    auto operator()(T && t) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) --
                    )
                };

                /// Tag type for the binary \<\< operator.
                struct shift_left
                  : expr_tag<shift_left, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) << static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \>\> operator.
                struct shift_right
                  : expr_tag<shift_right, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) >> static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary * operator.
                struct multiplies
                  : expr_tag<multiplies, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) * static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary / operator.
                struct divides
                  : expr_tag<divides, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) / static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary % operator.
                struct modulus
                  : expr_tag<modulus, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) % static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary + operator.
                struct plus
                  : expr_tag<plus, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) + static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary - operator.
                struct minus
                  : expr_tag<minus, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) - static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \< operator.
                struct less
                  : expr_tag<less, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) < static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \> operator.
                struct greater
                  : expr_tag<greater, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) > static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \<= operator.
                struct less_equal
                  : expr_tag<less_equal, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) <= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \>= operator.
                struct greater_equal
                  : expr_tag<greater_equal, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) >= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary == operator.
                struct equal_to
                  : expr_tag<equal_to, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) == static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary != operator.
                struct not_equal_to
                  : expr_tag<not_equal_to, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) != static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary || operator.
                struct logical_or
                  : expr_tag<logical_or, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) || static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary && operator.
                struct logical_and
                  : expr_tag<logical_and, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) && static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary & operator.
                struct bitwise_and
                  : expr_tag<bitwise_and, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) & static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary | operator.
                struct bitwise_or
                  : expr_tag<bitwise_or, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) | static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary ^ operator.
                struct bitwise_xor
                  : expr_tag<bitwise_xor, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) ^ static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary , operator.
                struct comma
                  : expr_tag<comma, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) , static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary ->* operator.
                struct mem_ptr
                  : expr_tag<mem_ptr, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) ->* static_cast<U &&>(u)
                    )

                    template<typename Obj, typename Type, typename Class
                      , BOOST_PROTO_ENABLE_IF(!proto::v5::is_expr<Obj>::value)
                    >
                    auto operator()(Obj && obj, Type Class::*pm) const
                    BOOST_PROTO_AUTO_RETURN(
                        (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pm)
                    )

                    template<typename Obj, typename Type, typename Class, typename ...Args
                      , BOOST_PROTO_ENABLE_IF(!proto::v5::is_expr<Obj>::value)
                    >
                    auto operator()(Obj && obj, Type (Class::*pmf)(Args...)) const
                    BOOST_PROTO_AUTO_RETURN(
                        detail::mem_fun_binder_<Obj, Type (Class::*)(Args...)>(static_cast<Obj &&>(obj), pmf)
                    )
                };

                /// Tag type for the binary = operator.
                struct assign
                  : expr_tag<assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) = static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \<\<= operator.
                struct shift_left_assign
                  : expr_tag<shift_left_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) <<= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary \>\>= operator.
                struct shift_right_assign
                  : expr_tag<shift_right_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) >>= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary *= operator.
                struct multiplies_assign
                  : expr_tag<multiplies_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) *= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary /= operator.
                struct divides_assign
                  : expr_tag<divides_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) /= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary %= operator.
                struct modulus_assign
                  : expr_tag<modulus_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) %= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary += operator.
                struct plus_assign
                  : expr_tag<plus_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) += static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary -= operator.
                struct minus_assign
                  : expr_tag<minus_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) -= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary &= operator.
                struct bitwise_and_assign
                  : expr_tag<bitwise_and_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) &= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary |= operator.
                struct bitwise_or_assign
                  : expr_tag<bitwise_or_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) |= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary ^= operator.
                struct bitwise_xor_assign
                  : expr_tag<bitwise_xor_assign, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) -= static_cast<U &&>(u)
                    )
                };

                /// Tag type for the binary subscript operator.
                struct subscript
                  : expr_tag<subscript, binary>
                {
                    template<typename T, typename U>
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) [ static_cast<U &&>(u) ]
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
                    auto operator()(T && t, U && u) const
                    BOOST_PROTO_AUTO_RETURN(
                        MakeExpr()(static_cast<T &&>(t), static_cast<U &&>(u))
                    )
                };

                /// Tag type for the ternary ?: conditional operator.
                struct if_else_
                  : expr_tag<if_else_, ternary>
                {
                    template<typename T, typename U, typename V>
                    auto operator()(T && t, U && u, V && v) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<T &&>(t) ? static_cast<U &&>(u) : static_cast<V &&>(v)
                    )
                };

                /// Tag type for the n-ary function call operator.
                struct function
                  : expr_tag<function, nary>
                {
                    template<typename Fun, typename ...T>
                    auto operator()(Fun && fun, T &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        static_cast<Fun &&>(fun)(static_cast<T &&>(t)...)
                    )

                    template<typename Type, typename Class, typename Obj>
                    auto operator()(Type Class::*pm, Obj && obj) const
                    BOOST_PROTO_AUTO_RETURN(
                        (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pm)
                    )

                    template<typename Type, typename Class, typename ...Args, typename Obj, typename ...T>
                    auto operator()(Type (Class::*pmf)(Args...), Obj && obj, T &&... t) const
                    BOOST_PROTO_AUTO_RETURN(
                        (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pmf)(static_cast<T &&>(t)...)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // env_var_tag
                template<typename Tag>
                struct env_var_tag
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(env_var_tag);

                    // So that tag objects of type (derived from) env_var_tag can be used
                    // to create basic_action environments like (data=x, myvar=y),
                    // where "data" and "myvar" are tags.
                    template<typename V, BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<env_var_tag, V>::value))>
                    env<Tag, V> operator=(V && v) const
                    {
                        return env<Tag, V>(static_cast<V &&>(v));
                    }
                };

                /// Tag type for the data parameter in the basic_action environment
                struct data_type
                  : env_var_tag<data_type>
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(data_type);
                    using env_var_tag<data_type>::operator=;
                };

                namespace
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // data
                    constexpr auto const & data = utility::static_const<data_type>::value;
                }

                BOOST_PROTO_IGNORE_UNUSED(data);

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
                auto operator()(E && e, Rest &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    proto::v5::tag_of(static_cast<E &&>(e))
                )
            };
        }
    } // namespace proto
} // namespace boost

#endif
