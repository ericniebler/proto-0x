///////////////////////////////////////////////////////////////////////////////
// tags.hpp
/// Contains the tags for all the overloadable operators in C++
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
            struct tag_base
            {};

            template<typename Tag, typename IsTerminal>
            struct basic_tag
              : tag_base
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(basic_tag);

                typedef Tag         proto_tag;
                typedef IsTerminal  proto_is_terminal;

                // So that tag objects of type (derived from) basic_tag can be used
                // to create basic_action environments like (data=x, local=y),
                // where "data" and "local" are tags.
                template<typename V, BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<basic_tag, V>::value))>
                env<basic_tag, V> operator=(V && v) const
                {
                    return env<basic_tag, V>{static_cast<V &&>(v)};
                }
            };

            namespace tag
            {
                /// Tag type for terminals; aka, leaves in the expression tree.
                struct terminal : basic_tag<terminal, std::true_type> {};

                /// Tag type for the unary + operator.
                struct unary_plus : basic_tag<unary_plus> {};

                /// Tag type for the unary - operator.
                struct negate : basic_tag<negate> {};

                /// Tag type for the unary * operator.
                struct dereference : basic_tag<dereference> {};

                /// Tag type for the unary ~ operator.
                struct complement : basic_tag<complement> {};

                /// Tag type for the unary & operator.
                struct address_of : basic_tag<address_of> {};

                /// Tag type for the unary ! operator.
                struct logical_not : basic_tag<logical_not> {};

                /// Tag type for the unary prefix ++ operator.
                struct pre_inc : basic_tag<pre_inc> {};

                /// Tag type for the unary prefix -- operator.
                struct pre_dec : basic_tag<pre_dec> {};

                /// Tag type for the unary postfix ++ operator.
                struct post_inc : basic_tag<post_inc> {};

                /// Tag type for the unary postfix -- operator.
                struct post_dec : basic_tag<post_dec> {};

                /// Tag type for the binary \<\< operator.
                struct shift_left : basic_tag<shift_left> {};

                /// Tag type for the binary \>\> operator.
                struct shift_right : basic_tag<shift_right> {};

                /// Tag type for the binary * operator.
                struct multiplies : basic_tag<multiplies> {};

                /// Tag type for the binary / operator.
                struct divides : basic_tag<divides> {};

                /// Tag type for the binary % operator.
                struct modulus : basic_tag<modulus> {};

                /// Tag type for the binary + operator.
                struct plus : basic_tag<plus> {};

                /// Tag type for the binary - operator.
                struct minus : basic_tag<minus> {};

                /// Tag type for the binary \< operator.
                struct less : basic_tag<less> {};

                /// Tag type for the binary \> operator.
                struct greater : basic_tag<greater> {};

                /// Tag type for the binary \<= operator.
                struct less_equal : basic_tag<less_equal> {};

                /// Tag type for the binary \>= operator.
                struct greater_equal : basic_tag<greater_equal> {};

                /// Tag type for the binary == operator.
                struct equal_to : basic_tag<equal_to> {};

                /// Tag type for the binary != operator.
                struct not_equal_to : basic_tag<not_equal_to> {};

                /// Tag type for the binary || operator.
                struct logical_or : basic_tag<logical_or> {};

                /// Tag type for the binary && operator.
                struct logical_and : basic_tag<logical_and> {};

                /// Tag type for the binary & operator.
                struct bitwise_and : basic_tag<bitwise_and> {};

                /// Tag type for the binary | operator.
                struct bitwise_or : basic_tag<bitwise_or> {};

                /// Tag type for the binary ^ operator.
                struct bitwise_xor : basic_tag<bitwise_xor> {};

                /// Tag type for the binary , operator.
                struct comma : basic_tag<comma> {};

                /// Tag type for the binary ->* operator.
                struct mem_ptr : basic_tag<mem_ptr> {};

                /// Tag type for the binary = operator.
                struct assign : basic_tag<assign> {};

                /// Tag type for the binary \<\<= operator.
                struct shift_left_assign : basic_tag<shift_left_assign> {};

                /// Tag type for the binary \>\>= operator.
                struct shift_right_assign : basic_tag<shift_right_assign> {};

                /// Tag type for the binary *= operator.
                struct multiplies_assign : basic_tag<multiplies_assign> {};

                /// Tag type for the binary /= operator.
                struct divides_assign : basic_tag<divides_assign> {};

                /// Tag type for the binary %= operator.
                struct modulus_assign : basic_tag<modulus_assign> {};

                /// Tag type for the binary += operator.
                struct plus_assign : basic_tag<plus_assign> {};

                /// Tag type for the binary -= operator.
                struct minus_assign : basic_tag<minus_assign> {};

                /// Tag type for the binary &= operator.
                struct bitwise_and_assign : basic_tag<bitwise_and_assign> {};

                /// Tag type for the binary |= operator.
                struct bitwise_or_assign : basic_tag<bitwise_or_assign> {};

                /// Tag type for the binary ^= operator.
                struct bitwise_xor_assign : basic_tag<bitwise_xor_assign> {};

                /// Tag type for the binary subscript operator.
                struct subscript : basic_tag<subscript> {};

                /// Tag type for the binary virtual data members.
                struct member : basic_tag<member> {};

                /// Tag type for the ternary ?: conditional operator.
                struct if_else_ : basic_tag<if_else_> {};

                /// Tag type for the n-ary function call operator.
                struct function : basic_tag<function> {};

                /// Tag type for the data parameter in the basic_action environment
                struct data_type
                  : basic_tag<data_type>
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(data_type);
                    using basic_tag<data_type>::operator=;
                };

                /// Tag type for the local parameter in the basic_action env
                struct local_type
                  : basic_tag<local_type>
                {
                    BOOST_PROTO_REGULAR_TRIVIAL_CLASS(local_type);
                    using basic_tag<local_type>::operator=;
                };

                namespace
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    // data
                    constexpr data_type const & data = utility::static_const<data_type>::value;

                    ////////////////////////////////////////////////////////////////////////////////
                    // local
                    constexpr local_type const & local = utility::static_const<local_type>::value;
                }

                BOOST_PROTO_IGNORE_UNUSED(data, local);

                // Abstract expression tags
                struct nullary_expr
                {};
                
                struct unary_expr
                {};
                
                struct binary_expr
                {};
                
                struct nary_expr
                {};

            } // namespace tag
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

        struct matches_
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
          : std::is_base_of<tag_base, T>
        {};

        template<typename T>
        struct is_tag<T &>
          : std::is_base_of<tag_base, T>
        {};

        template<typename T>
        struct is_tag<T &&>
          : std::is_base_of<tag_base, T>
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
