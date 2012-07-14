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
#include <boost/proto/args.hpp>
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
            // build_result_
            template<typename R, typename List, typename... Rest>
            struct build_result_;

            template<typename R, typename ...As, typename... Rest>
            struct build_result_<R, utility::list<As...>, Rest...>
            {
                typedef R type(As..., Rest...);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_2_
            template<std::size_t I, typename PrimitiveTransform>
            struct expand_pattern_2_
            {
                typedef PrimitiveTransform type;
            };

            template<std::size_t I, typename T>
            struct expand_pattern_2_<I, pack(T)>
            {
                typedef proto::_child<I> type(T);
            };

            template<typename T>
            struct expand_pattern_2_<(std::size_t)-1, pack(T)>
            {
                typedef proto::_value type(T);
            };

            template<std::size_t I, typename R, typename ...As>
            struct expand_pattern_2_<I, R(As...)>
            {
                typedef R type(typename expand_pattern_2_<I, As>::type...);
            };

            template<std::size_t I, typename R, typename ...As>
            struct expand_pattern_2_<I, R(*)(As...)>
              : expand_pattern_2_<I, R(As...)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // expand_pattern_1_
            template<typename Indices, typename PrimitiveTransform>
            struct expand_pattern_1_;

            template<std::size_t ...I, typename R, typename ...As>
            struct expand_pattern_1_<utility::indices<I...>, R(As......)>
              : build_result_<
                    R
                  , utility::pop_back<As...>
                  , typename expand_pattern_2_<
                        I
                      , typename utility::result_of::back<As...>::type
                    >::type...
                >
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // collect_pack_transforms_
            template<typename UnpackingPattern, typename Transforms = void()>
            struct collect_pack_transforms_
            {
                typedef Transforms type;
            };

            template<typename Ret, typename Head, typename ...Tail, typename Transforms>
            struct collect_pack_transforms_<Ret(Head, Tail...), Transforms>
              : collect_pack_transforms_<
                    Ret(Tail...)
                  , typename collect_pack_transforms_<Head, Transforms>::type
                >
            {};

            template<typename Ret, typename Head, typename ...Tail, typename Transforms>
            struct collect_pack_transforms_<Ret(*)(Head, Tail...), Transforms>
              : collect_pack_transforms_<Ret(Head, Tail...), Transforms>
            {};

            template<typename Transform, typename ...Transforms>
            struct collect_pack_transforms_<pack(Transform), void(Transforms...)>
            {
                typedef void type(as_transform<Transform>, Transforms...);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_2_
            template<typename Arity>
            using compute_indices_2_ =
                typename std::conditional<
                    Arity::value == 0
                  , utility::indices<(std::size_t)-1>
                  , utility::make_indices<0, Arity::value>
                >::type;

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_1_
            template<typename Transforms, typename ...Args>
            struct compute_indices_1_
            {
                static_assert(
                    utility::never<Transforms>::value
                  , "No pack expression found in unpacking pattern. See boost::proto::pack."
                );
            };

            template<typename Head, typename...Tail, typename ...Args>
            struct compute_indices_1_<void(Head, Tail...), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef typename compute_indices_1_<void(Tail...), Args...>::arity tail_arity;
                static_assert(
                    arity::value == tail_arity::value
                  , "Two pack expressions in unpacking pattern have different arities"
                );
                typedef compute_indices_2_<arity> type;
            };

            template<typename Head, typename ...Args>
            struct compute_indices_1_<void(Head), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef compute_indices_2_<arity> type;
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
        struct as_transform<Ret(*)(Args......), I>
          : as_transform<Ret(Args......), I>
        {};

        // Handle regular trasforms, be they primitive, callable or object
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

        // Handle transforms with pack expansions
        template<typename Ret, typename ...Tfxs, int I>
        struct as_transform<Ret(Tfxs......), I>
          : transform<as_transform<Ret(Tfxs......), I>>
        {
            typedef
                typename detail::collect_pack_transforms_<
                    typename utility::result_of::back<Tfxs...>::type
                >::type
            transforms_type;

            template<typename ...Args>
            auto operator()(Args &&... args) const
            BOOST_PROTO_AUTO_RETURN(
                as_transform<
                    typename detail::expand_pattern_1_<
                        typename detail::compute_indices_1_<transforms_type, Args...>::type
                      , Ret(Tfxs......)
                    >::type
                >()(static_cast<Args &&>(args)...)
            )
        };
    }
}

#endif
