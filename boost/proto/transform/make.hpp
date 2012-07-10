////////////////////////////////////////////////////////////////////////////////////////////////////
// make.hpp
// Contains definition of the make<> transform.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_MAKE_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_MAKE_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/mpl/print.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename T>
            typename T::type nested_type_(int);

            template<typename T>
            T nested_type_(long);

            template<typename T, bool Applied>
            struct nested_type
            {
                typedef decltype(detail::nested_type_<T>(1)) type;
                typedef std::true_type applied;
            };

            template<typename T>
            struct nested_type<T, false>
            {
                typedef T type;
                typedef std::false_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_3_
            template<typename R, typename ...Args>
            struct make_3_
              : make_2_<R, Args...>
            {
                static_assert(!std::is_pointer<R>::value, "ptr to function?");
            };

            template<typename R, typename ...A, typename ...Args>
            struct make_3_<R(A...), Args...>
            {
                typedef decltype(utility::by_val()(as_transform<R(A...)>()(std::declval<Args>()...))) type;
                typedef std::true_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_2_
            template<typename R, typename ...Args>
            struct make_2_
            {
                // Relies on the fact that as_transform<R> is _protect<R> when R is not a transform:
                typedef decltype(utility::by_val()(as_transform<R>()(std::declval<Args>()...))) type;
                typedef is_transform<R> applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_2_<R<A...>, Args...>
              : nested_type<
                    R<typename make_3_<A, Args...>::type...>
                  , utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                >
            {};

            template<typename R, typename ...Args>
            struct make_2_<_noinvoke<R>, Args...>
            {
                typedef R type;
                typedef std::false_type applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_2_<_noinvoke<R<A...>>, Args...>
            {
                typedef R<typename make_3_<A, Args...>::type...> type;
                typedef utility::or_<typename make_3_<A, Args...>::applied...> applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_1_
            template<typename R, typename ...Args>
            struct make_1_
            {
                typedef R type;
                typedef std::false_type applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_1_<R<A...>, Args...>
              : nested_type<
                    R<typename make_3_<A, Args...>::type...>
                  , utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                >
            {};

            template<typename R, typename ...Args>
            struct make_1_<_noinvoke<R>, Args...>
            {
                typedef R type;
                typedef std::false_type applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_1_<_noinvoke<R<A...>>, Args...>
            {
                typedef R<typename make_3_<A, Args...>::type...> type;
                typedef
                    std::integral_constant<
                        bool
                      , utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                    >
                applied;
            };
        }

        /// \brief A PrimitiveTransform which computes a type by evaluating any
        /// nested transforms and then constructs an object of that type.
        ///
        /// The <tt>make\<\></tt> transform checks to see if \c Object is a template.
        /// If it is, the template type is disassembled to find nested transforms.
        /// Proto considers the following types to represent transforms:
        ///
        /// \li Function types
        /// \li Function pointer types
        /// \li Types for which <tt>proto::is_callable\< type \>::value</tt> is \c true
        ///
        /// <tt>boost::result_of\<make\<T\<X0,X1,...\> \>(Expr, State, Data)\>::type</tt>
        /// is evaluated as follows. For each \c X in <tt>X0,X1,...</tt>, do:
        ///
        /// \li If \c X is a template like <tt>U\<Y0,Y1,...\></tt>, then let <tt>X'</tt>
        ///     be <tt>boost::result_of\<make\<U\<Y0,Y1,...\> \>(Expr, State, Data)\>::type</tt>
        ///     (which evaluates this procedure recursively). Note whether any
        ///     substitutions took place during this operation.
        /// \li Otherwise, if \c X is a transform, then let <tt>X'</tt> be
        ///     <tt>boost::result_of\<when\<_, X\>(Expr, State, Data)\>::type</tt>.
        ///     Note that a substitution took place.
        /// \li Otherwise, let <tt>X'</tt> be \c X, and note that no substitution
        ///     took place.
        /// \li If any substitutions took place in any of the above steps and
        ///     <tt>T\<X0',X1',...\></tt> has a nested <tt>::type</tt> typedef,
        ///     the result type is <tt>T\<X0',X1',...\>::type</tt>.
        /// \li Otherwise, the result type is <tt>T\<X0',X1',...\></tt>.
        ///
        /// Note that <tt>when\<\></tt> is implemented in terms of <tt>call\<\></tt>
        /// and <tt>make\<\></tt>, so the above procedure is evaluated recursively.
        template<typename Object>
        struct make
          : transform<make<Object>>
        {
            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                typename detail::make_3_<Object, Args...>::type{}
            )
        };

        /// \brief A PrimitiveTransform which computes a type by evaluating any
        /// nested transforms and then constructs an object of that type with the
        /// current expression, state and data, transformed according
        /// to \c A0 through \c AN.
        template<typename Object, typename... A>
        struct make<Object(A...)>
          : transform<make<Object(A...)>>
        {
            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                typename detail::make_3_<Object, Args...>::type{
                    as_transform<A>()(static_cast<Args &&>(args)...)...
                }
            )
        };

    } // namespace proto
} // namespace boost

#endif
