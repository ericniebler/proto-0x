////////////////////////////////////////////////////////////////////////////////////////////////////
// tags.hpp
// Contains the tags for all the overloadable operators in C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TAGS_HPP_INCLUDED
#define BOOST_PROTO_TAGS_HPP_INCLUDED

#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/utility.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Tag, typename IsTerminal>
            IsTerminal is_terminal_tag_(tags::expr_tag<Tag, IsTerminal> const *);

            std::false_type is_terminal_tag_(void const *);

            template<typename T>
            using is_terminal_tag = decltype(detail::is_terminal_tag_(static_cast<T *>(0)));
        }

        namespace tags
        {
            struct expr_tag_tag
            {};

            struct expr_tag_base
            {
                using proto_tag_type = expr_tag_tag;
            };

            template<typename Tag, typename IsTerminal>
            struct expr_tag
              : expr_tag_base
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_tag);
                using proto_is_terminal_type = IsTerminal;
                using proto_grammar_type = expr_tag<Tag>;
            };

            /// Tag type for terminals; aka, leaves in the expression tree.
            struct terminal
              : expr_tag<terminal, std::true_type>
            {
                template<
                    typename T
                  , typename Terminal = terminal
                  , typename MakeExpr = proto::functional::make_expr
                >
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    MakeExpr()(Terminal(), static_cast<T &&>(t))
                )
            };

            /// Tag type for the unary + operator.
            struct unary_plus
              : expr_tag<unary_plus>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    + static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary - operator.
            struct negate
              : expr_tag<negate>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    - static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary * operator.
            struct dereference
              : expr_tag<dereference>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    * static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary ~ operator.
            struct complement
              : expr_tag<complement>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    ~ static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary & operator.
            struct address_of
              : expr_tag<address_of>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    & static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary ! operator.
            struct logical_not
              : expr_tag<logical_not>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    ! static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary prefix ++ operator.
            struct pre_inc
              : expr_tag<pre_inc>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    ++ static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary prefix -- operator.
            struct pre_dec
              : expr_tag<pre_dec>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    -- static_cast<T &&>(t)
                )
            };

            /// Tag type for the unary postfix ++ operator.
            struct post_inc
              : expr_tag<post_inc>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) ++
                )
            };

            /// Tag type for the unary postfix -- operator.
            struct post_dec
              : expr_tag<post_dec>
            {
                template<typename T>
                auto operator()(T && t) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) --
                )
            };

            /// Tag type for the binary \<\< operator.
            struct shift_left
              : expr_tag<shift_left>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) << static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \>\> operator.
            struct shift_right
              : expr_tag<shift_right>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) >> static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary * operator.
            struct multiplies
              : expr_tag<multiplies>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) * static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary / operator.
            struct divides
              : expr_tag<divides>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) / static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary % operator.
            struct modulus
              : expr_tag<modulus>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) % static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary + operator.
            struct plus
              : expr_tag<plus>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) + static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary - operator.
            struct minus
              : expr_tag<minus>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) - static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \< operator.
            struct less
              : expr_tag<less>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) < static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \> operator.
            struct greater
              : expr_tag<greater>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) > static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \<= operator.
            struct less_equal
              : expr_tag<less_equal>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) <= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \>= operator.
            struct greater_equal
              : expr_tag<greater_equal>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) >= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary == operator.
            struct equal_to
              : expr_tag<equal_to>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) == static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary != operator.
            struct not_equal_to
              : expr_tag<not_equal_to>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) != static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary || operator.
            struct logical_or
              : expr_tag<logical_or>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) || static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary && operator.
            struct logical_and
              : expr_tag<logical_and>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) && static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary & operator.
            struct bitwise_and
              : expr_tag<bitwise_and>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) & static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary | operator.
            struct bitwise_or
              : expr_tag<bitwise_or>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) | static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary ^ operator.
            struct bitwise_xor
              : expr_tag<bitwise_xor>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) ^ static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary , operator.
            struct comma
              : expr_tag<comma>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) , static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary ->* operator.
            struct mem_ptr
              : expr_tag<mem_ptr>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) ->* static_cast<U &&>(u)
                )

                template<typename Obj, typename Type, typename Class
                  , BOOST_PROTO_ENABLE_IF(!proto::is_expr<Obj>::value)
                >
                auto operator()(Obj && obj, Type Class::*pm) const
                BOOST_PROTO_AUTO_RETURN(
                    (detail::proto_get_pointer(static_cast<Obj &&>(obj), 1) ->* pm)
                )

                template<typename Obj, typename Type, typename Class, typename ...Args
                  , BOOST_PROTO_ENABLE_IF(!proto::is_expr<Obj>::value)
                >
                auto operator()(Obj && obj, Type (Class::*pmf)(Args...)) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::mem_fun_binder_<Obj, Type (Class::*)(Args...)>(static_cast<Obj &&>(obj), pmf)
                )
            };

            /// Tag type for the binary = operator.
            struct assign
              : expr_tag<assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) = static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \<\<= operator.
            struct shift_left_assign
              : expr_tag<shift_left_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) <<= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary \>\>= operator.
            struct shift_right_assign
              : expr_tag<shift_right_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) >>= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary *= operator.
            struct multiplies_assign
              : expr_tag<multiplies_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) *= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary /= operator.
            struct divides_assign
              : expr_tag<divides_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) /= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary %= operator.
            struct modulus_assign
              : expr_tag<modulus_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) %= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary += operator.
            struct plus_assign
              : expr_tag<plus_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) += static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary -= operator.
            struct minus_assign
              : expr_tag<minus_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) -= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary &= operator.
            struct bitwise_and_assign
              : expr_tag<bitwise_and_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) &= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary |= operator.
            struct bitwise_or_assign
              : expr_tag<bitwise_or_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) |= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary ^= operator.
            struct bitwise_xor_assign
              : expr_tag<bitwise_xor_assign>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) -= static_cast<U &&>(u)
                )
            };

            /// Tag type for the binary subscript operator.
            struct subscript
              : expr_tag<subscript>
            {
                template<typename T, typename U>
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) [ static_cast<U &&>(u) ]
                )
            };

            /// Tag type for the binary virtual data members.
            struct member
              : expr_tag<member>
            {
                template<typename T, typename U
                  , typename Member = member
                  , typename MakeExpr = proto::functional::make_expr
                >
                auto operator()(T && t, U && u) const
                BOOST_PROTO_AUTO_RETURN(
                    MakeExpr()(Member(), static_cast<T &&>(t), static_cast<U &&>(u))
                )
            };

            /// Tag type for the ternary ?: conditional operator.
            struct if_else_
              : expr_tag<if_else_>
            {
                template<typename T, typename U, typename V>
                auto operator()(T && t, U && u, V && v) const
                BOOST_PROTO_AUTO_RETURN(
                    static_cast<T &&>(t) ? static_cast<U &&>(u) : static_cast<V &&>(v)
                )
            };

            /// Tag type for the n-ary function call operator.
            struct function
              : expr_tag<function>
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

            ////////////////////////////////////////////////////////////////////////////////////////
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
            ////////////////////////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // _tag_of
        struct _tag_of
          : proto::basic_action<_tag_of>
        {
            template<typename E, typename ...Rest>
            auto operator()(E && e, Rest &&...) const
            BOOST_PROTO_AUTO_RETURN(
                proto::tag_of(static_cast<E &&>(e))
            )
        };

    } // namespace proto
} // namespace boost

#endif
