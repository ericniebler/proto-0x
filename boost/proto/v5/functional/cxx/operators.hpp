////////////////////////////////////////////////////////////////////////////////////////////////////
// operators.hpp
// Function objects coresponding to the C++ operators
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_FUNCTIONAL_CXX_OPERATORS_HPP_INCLUDED
#define BOOST_PROTO_CXX11_FUNCTIONAL_CXX_OPERATORS_HPP_INCLUDED

#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/utility.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace functional
            {
                namespace cxx
                {
                    /// Function object for the unary + operator.
                    struct unary_plus
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            + static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary - operator.
                    struct negate
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            - static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary * operator.
                    struct dereference
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            * static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary ~ operator.
                    struct complement
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            ~ static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary & operator.
                    struct address_of
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            & static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary ! operator.
                    struct logical_not
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            ! static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary prefix ++ operator.
                    struct pre_inc
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            ++ static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary prefix -- operator.
                    struct pre_dec
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            -- static_cast<T &&>(t)
                        )
                    };

                    /// Function object for the unary postfix ++ operator.
                    struct post_inc
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) ++
                        )
                    };

                    /// Function object for the unary postfix -- operator.
                    struct post_dec
                    {
                        template<typename T>
                        auto operator()(T && t) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) --
                        )
                    };

                    /// Function object for the binary \<\< operator.
                    struct shift_left
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) << static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \>\> operator.
                    struct shift_right
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) >> static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary * operator.
                    struct multiplies
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) * static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary / operator.
                    struct divides
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) / static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary % operator.
                    struct modulus
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) % static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary + operator.
                    struct plus
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) + static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary - operator.
                    struct minus
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) - static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \< operator.
                    struct less
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) < static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \> operator.
                    struct greater
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) > static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \<= operator.
                    struct less_equal
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) <= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \>= operator.
                    struct greater_equal
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) >= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary == operator.
                    struct equal_to
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) == static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary != operator.
                    struct not_equal_to
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) != static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary || operator.
                    struct logical_or
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) || static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary && operator.
                    struct logical_and
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) && static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary & operator.
                    struct bitwise_and
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) & static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary | operator.
                    struct bitwise_or
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) | static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary ^ operator.
                    struct bitwise_xor
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) ^ static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary , operator.
                    struct comma
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) , static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary ->* operator.
                    struct mem_ptr
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            utility::mem_ptr_adl(static_cast<T &&>(t), static_cast<U &&>(u))
                        )
                    };

                    /// Function object for the binary = operator.
                    struct assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) = static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \<\<= operator.
                    struct shift_left_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) <<= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary \>\>= operator.
                    struct shift_right_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) >>= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary *= operator.
                    struct multiplies_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) *= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary /= operator.
                    struct divides_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) /= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary %= operator.
                    struct modulus_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) %= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary += operator.
                    struct plus_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) += static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary -= operator.
                    struct minus_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) -= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary &= operator.
                    struct bitwise_and_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) &= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary |= operator.
                    struct bitwise_or_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) |= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary ^= operator.
                    struct bitwise_xor_assign
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) -= static_cast<U &&>(u)
                        )
                    };

                    /// Function object for the binary subscript operator.
                    struct subscript
                    {
                        template<typename T, typename U>
                        auto operator()(T && t, U && u) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) [ static_cast<U &&>(u) ]
                        )
                    };

                    /// Function object for the ternary ?: conditional operator.
                    struct if_else
                    {
                        template<typename T, typename U, typename V>
                        auto operator()(T && t, U && u, V && v) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<T &&>(t) ? static_cast<U &&>(u) : static_cast<V &&>(v)
                        )
                    };

                    /// Function object for the n-ary function call operator.
                    struct function
                    {
                        template<typename Fun, typename ...T>
                        auto operator()(Fun && fun, T &&... t) const
                        BOOST_PROTO_AUTO_RETURN(
                            static_cast<Fun &&>(fun)(static_cast<T &&>(t)...)
                        )

                        template<typename Type, typename Class, typename Obj>
                        auto operator()(Type Class::*pm, Obj && obj) const
                        BOOST_PROTO_AUTO_RETURN(
                            (utility::get_pointer_adl(static_cast<Obj &&>(obj), 1) ->* pm)
                        )

                        template<typename Type, typename Class, typename ...Args, typename Obj, typename ...T>
                        auto operator()(Type (Class::*pmf)(Args...), Obj && obj, T &&... t) const
                        BOOST_PROTO_AUTO_RETURN(
                            (utility::get_pointer_adl(static_cast<Obj &&>(obj), 1) ->* pmf)(static_cast<T &&>(t)...)
                        )
                    };
                }
            }
        }
    }
}

#endif
