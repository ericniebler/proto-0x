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

namespace boost { namespace proto { namespace tags
{
    template<typename Tag, typename IsTerminal>
    struct def_tag
    {
        typedef Tag         proto_tag;
        typedef IsTerminal  proto_is_terminal;
    };

    namespace tag
    {
        /// Tag type for terminals; aka, leaves in the expression tree.
        struct terminal : def_tag<terminal, std::true_type> {};

        /// Tag type for the unary + operator.
        struct unary_plus : def_tag<unary_plus> {};

        /// Tag type for the unary - operator.
        struct negate : def_tag<negate> {};

        /// Tag type for the unary * operator.
        struct dereference : def_tag<dereference> {};

        /// Tag type for the unary ~ operator.
        struct complement : def_tag<complement> {};

        /// Tag type for the unary & operator.
        struct address_of : def_tag<address_of> {};

        /// Tag type for the unary ! operator.
        struct logical_not : def_tag<logical_not> {};

        /// Tag type for the unary prefix ++ operator.
        struct pre_inc : def_tag<pre_inc> {};

        /// Tag type for the unary prefix -- operator.
        struct pre_dec : def_tag<pre_dec> {};

        /// Tag type for the unary postfix ++ operator.
        struct post_inc : def_tag<post_inc> {};

        /// Tag type for the unary postfix -- operator.
        struct post_dec : def_tag<post_dec> {};

        /// Tag type for the binary \<\< operator.
        struct shift_left : def_tag<shift_left> {};

        /// Tag type for the binary \>\> operator.
        struct shift_right : def_tag<shift_right> {};

        /// Tag type for the binary * operator.
        struct multiplies : def_tag<multiplies> {};

        /// Tag type for the binary / operator.
        struct divides : def_tag<divides> {};

        /// Tag type for the binary % operator.
        struct modulus : def_tag<modulus> {};

        /// Tag type for the binary + operator.
        struct plus : def_tag<plus> {};

        /// Tag type for the binary - operator.
        struct minus : def_tag<minus> {};

        /// Tag type for the binary \< operator.
        struct less : def_tag<less> {};

        /// Tag type for the binary \> operator.
        struct greater : def_tag<greater> {};

        /// Tag type for the binary \<= operator.
        struct less_equal : def_tag<less_equal> {};

        /// Tag type for the binary \>= operator.
        struct greater_equal : def_tag<greater_equal> {};

        /// Tag type for the binary == operator.
        struct equal_to : def_tag<equal_to> {};

        /// Tag type for the binary != operator.
        struct not_equal_to : def_tag<not_equal_to> {};

        /// Tag type for the binary || operator.
        struct logical_or : def_tag<logical_or> {};

        /// Tag type for the binary && operator.
        struct logical_and : def_tag<logical_and> {};

        /// Tag type for the binary & operator.
        struct bitwise_and : def_tag<bitwise_and> {};

        /// Tag type for the binary | operator.
        struct bitwise_or : def_tag<bitwise_or> {};

        /// Tag type for the binary ^ operator.
        struct bitwise_xor : def_tag<bitwise_xor> {};

        /// Tag type for the binary , operator.
        struct comma : def_tag<comma> {};

        /// Tag type for the binary ->* operator.
        struct mem_ptr : def_tag<mem_ptr> {};

        /// Tag type for the binary = operator.
        struct assign : def_tag<assign> {};

        /// Tag type for the binary \<\<= operator.
        struct shift_left_assign : def_tag<shift_left_assign> {};

        /// Tag type for the binary \>\>= operator.
        struct shift_right_assign : def_tag<shift_right_assign> {};

        /// Tag type for the binary *= operator.
        struct multiplies_assign : def_tag<multiplies_assign> {};

        /// Tag type for the binary /= operator.
        struct divides_assign : def_tag<divides_assign> {};

        /// Tag type for the binary %= operator.
        struct modulus_assign : def_tag<modulus_assign> {};

        /// Tag type for the binary += operator.
        struct plus_assign : def_tag<plus_assign> {};

        /// Tag type for the binary -= operator.
        struct minus_assign : def_tag<minus_assign> {};

        /// Tag type for the binary &= operator.
        struct bitwise_and_assign : def_tag<bitwise_and_assign> {};

        /// Tag type for the binary |= operator.
        struct bitwise_or_assign : def_tag<bitwise_or_assign> {};

        /// Tag type for the binary ^= operator.
        struct bitwise_xor_assign : def_tag<bitwise_xor_assign> {};

        /// Tag type for the binary subscript operator.
        struct subscript : def_tag<subscript> {};

        /// Tag type for the binary virtual data members.
        struct member : def_tag<member> {};

        /// Tag type for the ternary ?: conditional operator.
        struct if_else_ : def_tag<if_else_> {};

        /// Tag type for the n-ary function call operator.
        struct function : def_tag<function> {};

    }
}}}

#endif
