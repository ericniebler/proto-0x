///////////////////////////////////////////////////////////////////////////////
// base.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_BASE_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

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

            ////////////////////////////////////////////////////////////////////////////////////////
            // select_
            template<typename T, typename U>
            inline T && select_(T &&t, U &&) noexcept
            {
                return static_cast<T &&>(t);
            }

            template<typename U>
            inline U && select_(utility::void_type const &, U &&u) noexcept
            {
                return static_cast<U &&>(u);
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_2_
            template<std::size_t N, typename Sig>
            struct call_2_;

            template<std::size_t N, typename Transform, typename ...Args>
            struct call_2_<N, Transform(Args...)>
              : call_2_<N - 1, Transform(Args..., _void)>
            {};

            template<typename Transform, typename ...Args>
            struct call_2_<0, Transform(Args...)>
            {
                template<typename ...T, BOOST_PROTO_ENABLE_IF(sizeof...(T) == sizeof...(Args))>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    typename Transform::proto_transform_type()(
                        detail::select_(
                            as_transform<Args>()(static_cast<T &&>(t)...)
                          , static_cast<T &&>(t)
                        )...
                    )
                )

                template<typename ...T, BOOST_PROTO_ENABLE_IF(sizeof...(T) < sizeof...(Args))>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    typename Transform::proto_transform_type()(
                        as_transform<Args>()(static_cast<T &&>(t)...)...
                    )
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_1_
            template<bool IsTransform, typename Sig>
            struct call_1_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle transforms.
            template<typename Transform, typename ...Args>
            struct call_1_<true, Transform(Args...)>
            {
                template<typename ...T>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    call_2_<
                        (sizeof...(T) > sizeof...(Args) ? sizeof...(T) - sizeof...(Args) : 0)
                      , Transform(Args...)
                    >()(static_cast<T &&>(t)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle callables and object constructions.
            template<typename CallOrObj, typename ...Args>
            struct call_1_<false, CallOrObj(Args...)>
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // Handle callables
                template<typename ...T, typename C = CallOrObj>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    C{}(as_transform<Args>()(static_cast<T &&>(t)...)...)
                )

                ////////////////////////////////////////////////////////////////////////////////////
                // Handle objects
                template<typename ...T, typename C = CallOrObj>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    C{as_transform<Args>()(static_cast<T &&>(t)...)...}
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // as_transform_
            template<typename T>
            typename T::proto_transform_type as_transform_(int);

            template<typename T>
            _protect<T> as_transform_(...);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform_base
        struct transform_base
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform
        // CRTP base class for all transforms
        template<typename Transform>
        struct transform
          : transform_base
        {
            typedef Transform proto_transform_type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_transform
        template<typename T>
        struct is_transform
          : std::is_base_of<transform_base, T>
        {};

        template<typename T>
        struct is_transform<T &>
          : std::is_base_of<transform_base, T>
        {};

        /// \brief A PrimitiveTransform which prevents another PrimitiveTransform
        /// from being applied in an \c ObjectTransform.
        ///
        /// When building higher order transforms with <tt>make\<\></tt> or
        /// <tt>lazy\<\></tt>, you sometimes would like to build types that
        /// are parameterized with Proto transforms. In such lambda-style
        /// transforms, Proto will unhelpfully find all nested transforms
        /// and apply them, even if you don't want them to be applied. Consider
        /// the following transform, which will replace the \c _ in
        /// <tt>Bar<_>()</tt> with <tt>proto::terminal\<int\>::type</tt>:
        ///
        /// \code
        /// template<typename T>
        /// struct Bar
        /// {};
        ///
        /// struct Foo
        ///   : proto::when<_, Bar<_>() >
        /// {};
        ///
        /// proto::terminal<int>::type i = {0};
        ///
        /// int main()
        /// {
        ///     Foo()(i);
        ///     std::cout << typeid(Foo()(i)).name() << std::endl;
        /// }
        /// \endcode
        ///
        /// If you actually wanted to default-construct an object of type
        /// <tt>Bar\<_\></tt>, you would have to protect the \c _ to prevent
        /// it from being applied. You can use <tt>proto::_protect\<\></tt>
        /// as follows:
        ///
        /// \code
        /// // OK: replace anything with Bar<_>()
        /// struct Foo
        ///   : proto::when<_, Bar<_protect<_> >() >
        /// {};
        /// \endcode
        template<typename T>
        struct _protect
          : transform<_protect<T>>
        {
            template<typename... Args>
            T operator()(Args &&...) const
            {
                static_assert(utility::never<T>::value, "don't call _protect<T>::operator() at runtime!");
                return T{};
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////
        // _void
        struct _void
          : transform<_void>
        {
            template<typename ...T>
            utility::void_type const &operator()(T &&...) const noexcept
            {
                return utility::void_;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename T, int>
        struct as_transform
          : decltype(detail::as_transform_<T>(1))
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(*)(Args...), I>
          : as_transform<Ret(Args...), I>
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(Args...), I>
          : transform<as_transform<Ret(Args...), I>>
        {
            template<typename ...T, typename X = typename detail::make_1_<Ret, T...>::type>
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                detail::call_1_<is_transform<X>::value, X(Args...)>()(static_cast<T &&>(t)...)
            )
        };
    }
}

#endif
