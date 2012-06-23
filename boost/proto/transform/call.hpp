////////////////////////////////////////////////////////////////////////////////////////////////////
// call.hpp
// Contains definition of the call<> transform.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_CALL_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_CALL_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        /// \brief Wrap \c PrimitiveTransform so that <tt>when\<\></tt> knows
        /// it is callable. Requires that the parameter is actually a
        /// PrimitiveTransform.
        ///
        /// This form of <tt>call\<\></tt> is useful for annotating an
        /// arbitrary PrimitiveTransform as callable when using it with
        /// <tt>when\<\></tt>. Consider the following transform, which
        /// is parameterized with another transform.
        ///
        /// \code
        /// template<typename Grammar>
        /// struct Foo
        ///   : when<
        ///         unary_plus<Grammar>
        ///       , Grammar(_child)   // May or may not work.
        ///     >
        /// {};
        /// \endcode
        ///
        /// The problem with the above is that <tt>when\<\></tt> may or
        /// may not recognize \c Grammar as callable, depending on how
        /// \c Grammar is implemented. (See <tt>is_callable\<\></tt> for
        /// a discussion of this issue.) You can guard against
        /// the issue by wrapping \c Grammar in <tt>call\<\></tt>, such
        /// as:
        ///
        /// \code
        /// template<typename Grammar>
        /// struct Foo
        ///   : when<
        ///         unary_plus<Grammar>
        ///       , call<Grammar>(_child)   // OK, this works
        ///     >
        /// {};
        /// \endcode
        ///
        /// The above could also have been written as:
        ///
        /// \code
        /// template<typename Grammar>
        /// struct Foo
        ///   : when<
        ///         unary_plus<Grammar>
        ///       , call<Grammar(_child)>   // OK, this works, too
        ///     >
        /// {};
        /// \endcode
        template<typename PrimitiveTransform, int>
        struct call
          : PrimitiveTransform::proto_transform_type
        {};

        template<typename Fun, typename ...Args, int N>
        struct call<Fun(Args...), N>
          : transform<call<Fun(Args...), N>>
        {
            template<typename ...T, typename X = Fun, BOOST_PROTO_ENABLE_IF(is_transform<X>::value)>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                detail::invoke_transform_1_<true, X(Args...)>()(static_cast<T &&>(t)...)
            )

            template<typename ...T, typename X = Fun, BOOST_PROTO_ENABLE_IF(!is_transform<X>::value)>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                X{}(as_transform<Args>()(static_cast<T &&>(t)...)...)
            )
        };

    } // namespace proto
} // namespace boost

#endif
