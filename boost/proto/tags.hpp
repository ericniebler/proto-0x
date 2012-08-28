////////////////////////////////////////////////////////////////////////////////////////////////////
// tags.hpp
// Contains the tags for all the overloadable operators in C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TAGS_HPP_INCLUDED
#define BOOST_PROTO_TAGS_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        namespace tags
        {
            struct expr_tag_base
            {};

            template<typename Tag, typename IsTerminal>
            struct expr_tag
              : expr_tag_base
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_tag);

                typedef Tag         proto_tag;
                typedef IsTerminal  proto_is_terminal;
            };

            /// Tag type for terminals; aka, leaves in the expression tree.
            struct terminal : expr_tag<terminal, std::true_type> {};

            /// Tag type for the unary + operator.
            struct unary_plus : expr_tag<unary_plus> {};

            /// Tag type for the unary - operator.
            struct negate : expr_tag<negate> {};

            /// Tag type for the unary * operator.
            struct dereference : expr_tag<dereference> {};

            /// Tag type for the unary ~ operator.
            struct complement : expr_tag<complement> {};

            /// Tag type for the unary & operator.
            struct address_of : expr_tag<address_of> {};

            /// Tag type for the unary ! operator.
            struct logical_not : expr_tag<logical_not> {};

            /// Tag type for the unary prefix ++ operator.
            struct pre_inc : expr_tag<pre_inc> {};

            /// Tag type for the unary prefix -- operator.
            struct pre_dec : expr_tag<pre_dec> {};

            /// Tag type for the unary postfix ++ operator.
            struct post_inc : expr_tag<post_inc> {};

            /// Tag type for the unary postfix -- operator.
            struct post_dec : expr_tag<post_dec> {};

            /// Tag type for the binary \<\< operator.
            struct shift_left : expr_tag<shift_left> {};

            /// Tag type for the binary \>\> operator.
            struct shift_right : expr_tag<shift_right> {};

            /// Tag type for the binary * operator.
            struct multiplies : expr_tag<multiplies> {};

            /// Tag type for the binary / operator.
            struct divides : expr_tag<divides> {};

            /// Tag type for the binary % operator.
            struct modulus : expr_tag<modulus> {};

            /// Tag type for the binary + operator.
            struct plus : expr_tag<plus> {};

            /// Tag type for the binary - operator.
            struct minus : expr_tag<minus> {};

            /// Tag type for the binary \< operator.
            struct less : expr_tag<less> {};

            /// Tag type for the binary \> operator.
            struct greater : expr_tag<greater> {};

            /// Tag type for the binary \<= operator.
            struct less_equal : expr_tag<less_equal> {};

            /// Tag type for the binary \>= operator.
            struct greater_equal : expr_tag<greater_equal> {};

            /// Tag type for the binary == operator.
            struct equal_to : expr_tag<equal_to> {};

            /// Tag type for the binary != operator.
            struct not_equal_to : expr_tag<not_equal_to> {};

            /// Tag type for the binary || operator.
            struct logical_or : expr_tag<logical_or> {};

            /// Tag type for the binary && operator.
            struct logical_and : expr_tag<logical_and> {};

            /// Tag type for the binary & operator.
            struct bitwise_and : expr_tag<bitwise_and> {};

            /// Tag type for the binary | operator.
            struct bitwise_or : expr_tag<bitwise_or> {};

            /// Tag type for the binary ^ operator.
            struct bitwise_xor : expr_tag<bitwise_xor> {};

            /// Tag type for the binary , operator.
            struct comma : expr_tag<comma> {};

            /// Tag type for the binary ->* operator.
            struct mem_ptr : expr_tag<mem_ptr> {};

            /// Tag type for the binary = operator.
            struct assign : expr_tag<assign> {};

            /// Tag type for the binary \<\<= operator.
            struct shift_left_assign : expr_tag<shift_left_assign> {};

            /// Tag type for the binary \>\>= operator.
            struct shift_right_assign : expr_tag<shift_right_assign> {};

            /// Tag type for the binary *= operator.
            struct multiplies_assign : expr_tag<multiplies_assign> {};

            /// Tag type for the binary /= operator.
            struct divides_assign : expr_tag<divides_assign> {};

            /// Tag type for the binary %= operator.
            struct modulus_assign : expr_tag<modulus_assign> {};

            /// Tag type for the binary += operator.
            struct plus_assign : expr_tag<plus_assign> {};

            /// Tag type for the binary -= operator.
            struct minus_assign : expr_tag<minus_assign> {};

            /// Tag type for the binary &= operator.
            struct bitwise_and_assign : expr_tag<bitwise_and_assign> {};

            /// Tag type for the binary |= operator.
            struct bitwise_or_assign : expr_tag<bitwise_or_assign> {};

            /// Tag type for the binary ^= operator.
            struct bitwise_xor_assign : expr_tag<bitwise_xor_assign> {};

            /// Tag type for the binary subscript operator.
            struct subscript : expr_tag<subscript> {};

            /// Tag type for the binary virtual data members.
            struct member : expr_tag<member> {};

            /// Tag type for the ternary ?: conditional operator.
            struct if_else_ : expr_tag<if_else_> {};

            /// Tag type for the n-ary function call operator.
            struct function : expr_tag<function> {};

            // Abstract expression tags
            struct nullary_expr
            {};

            struct unary_expr
            {};

            struct binary_expr
            {};

            struct nary_expr
            {};

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

        struct when
        {};

        struct match
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // tag_of
        template<typename Expr>
        struct tag_of
        {
            typedef typename Expr::proto_tag_type type;
        };

        template<typename Expr>
        struct tag_of<Expr &>
        {
            typedef typename Expr::proto_tag_type type;
        };

        template<typename Expr>
        struct tag_of<Expr &&>
        {
            typedef typename Expr::proto_tag_type type;
        };

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
                static_cast<E &&>(e).proto_tag()
            )
        };

    } // namespace proto
} // namespace boost

#endif
