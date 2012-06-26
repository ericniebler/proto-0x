///////////////////////////////////////////////////////////////////////////////
/// \file tags.hpp
/// Contains the tags for all the overloadable operators in C++
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TAGS_HPP_INCLUDED
#define BOOST_PROTO_TAGS_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        namespace tags
        {
            template<typename Tag, typename IsTerminal>
            struct def
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(def);

                typedef Tag         proto_tag;
                typedef IsTerminal  proto_is_terminal;

                // So that tag objects of type (derived from) def can be used
                // to create transform environments like (data=x, locals=y),
                // where "data" and "locals" are tags.
                template<typename V, BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<def, V>::value))>
                environment<def, V> operator=(V && v) const
                {
                    return environment<def, V>{static_cast<V &&>(v)};
                }
            };

            namespace tag
            {
                /// Tag type for terminals; aka, leaves in the expression tree.
                struct terminal : def<terminal, std::true_type> {};

                /// Tag type for the unary + operator.
                struct unary_plus : def<unary_plus> {};

                /// Tag type for the unary - operator.
                struct negate : def<negate> {};

                /// Tag type for the unary * operator.
                struct dereference : def<dereference> {};

                /// Tag type for the unary ~ operator.
                struct complement : def<complement> {};

                /// Tag type for the unary & operator.
                struct address_of : def<address_of> {};

                /// Tag type for the unary ! operator.
                struct logical_not : def<logical_not> {};

                /// Tag type for the unary prefix ++ operator.
                struct pre_inc : def<pre_inc> {};

                /// Tag type for the unary prefix -- operator.
                struct pre_dec : def<pre_dec> {};

                /// Tag type for the unary postfix ++ operator.
                struct post_inc : def<post_inc> {};

                /// Tag type for the unary postfix -- operator.
                struct post_dec : def<post_dec> {};

                /// Tag type for the binary \<\< operator.
                struct shift_left : def<shift_left> {};

                /// Tag type for the binary \>\> operator.
                struct shift_right : def<shift_right> {};

                /// Tag type for the binary * operator.
                struct multiplies : def<multiplies> {};

                /// Tag type for the binary / operator.
                struct divides : def<divides> {};

                /// Tag type for the binary % operator.
                struct modulus : def<modulus> {};

                /// Tag type for the binary + operator.
                struct plus : def<plus> {};

                /// Tag type for the binary - operator.
                struct minus : def<minus> {};

                /// Tag type for the binary \< operator.
                struct less : def<less> {};

                /// Tag type for the binary \> operator.
                struct greater : def<greater> {};

                /// Tag type for the binary \<= operator.
                struct less_equal : def<less_equal> {};

                /// Tag type for the binary \>= operator.
                struct greater_equal : def<greater_equal> {};

                /// Tag type for the binary == operator.
                struct equal_to : def<equal_to> {};

                /// Tag type for the binary != operator.
                struct not_equal_to : def<not_equal_to> {};

                /// Tag type for the binary || operator.
                struct logical_or : def<logical_or> {};

                /// Tag type for the binary && operator.
                struct logical_and : def<logical_and> {};

                /// Tag type for the binary & operator.
                struct bitwise_and : def<bitwise_and> {};

                /// Tag type for the binary | operator.
                struct bitwise_or : def<bitwise_or> {};

                /// Tag type for the binary ^ operator.
                struct bitwise_xor : def<bitwise_xor> {};

                /// Tag type for the binary , operator.
                struct comma : def<comma> {};

                /// Tag type for the binary ->* operator.
                struct mem_ptr : def<mem_ptr> {};

                /// Tag type for the binary = operator.
                struct assign : def<assign> {};

                /// Tag type for the binary \<\<= operator.
                struct shift_left_assign : def<shift_left_assign> {};

                /// Tag type for the binary \>\>= operator.
                struct shift_right_assign : def<shift_right_assign> {};

                /// Tag type for the binary *= operator.
                struct multiplies_assign : def<multiplies_assign> {};

                /// Tag type for the binary /= operator.
                struct divides_assign : def<divides_assign> {};

                /// Tag type for the binary %= operator.
                struct modulus_assign : def<modulus_assign> {};

                /// Tag type for the binary += operator.
                struct plus_assign : def<plus_assign> {};

                /// Tag type for the binary -= operator.
                struct minus_assign : def<minus_assign> {};

                /// Tag type for the binary &= operator.
                struct bitwise_and_assign : def<bitwise_and_assign> {};

                /// Tag type for the binary |= operator.
                struct bitwise_or_assign : def<bitwise_or_assign> {};

                /// Tag type for the binary ^= operator.
                struct bitwise_xor_assign : def<bitwise_xor_assign> {};

                /// Tag type for the binary subscript operator.
                struct subscript : def<subscript> {};

                /// Tag type for the binary virtual data members.
                struct member : def<member> {};

                /// Tag type for the ternary ?: conditional operator.
                struct if_else_ : def<if_else_> {};

                /// Tag type for the n-ary function call operator.
                struct function : def<function> {};

                namespace
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // data
                    constexpr _data const & data = utility::static_const<_data>::value;

                    ////////////////////////////////////////////////////////////////////////////////
                    // locals
                    constexpr _locals const & locals = utility::static_const<_locals>::value;
                }
            }
        } // namespace tags

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

        namespace detail
        {
            inline void ignore_tags()
            {
                BOOST_PROTO_IGNORE_UNUSED(tag::data);
                BOOST_PROTO_IGNORE_UNUSED(tag::locals);
            }
        }

    } // namespace proto
} // namespace boost

#endif
