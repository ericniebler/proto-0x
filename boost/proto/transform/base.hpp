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
#include <boost/mpl/identity.hpp>
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

            template<typename T, bool Applied, bool IsTransform, typename ...Args>
            struct invoke_
            {
                typedef decltype(utility::by_val()(T()(std::declval<Args>()...))) type;
                typedef std::true_type applied;
            };

            template<typename T, typename ...Args>
            struct invoke_<T, true, false, Args...>
            {
                typedef decltype(detail::nested_type_<T>(1)) type;
                typedef std::true_type applied;
            };

            template<typename T, typename ...Args>
            struct invoke_<T, false, false, Args...>
            {
                typedef T type;
                typedef std::false_type applied;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // make_4_
            template<bool IsTransform, typename R, typename ...Args>
            struct make_4_
            {
                typedef decltype(utility::by_val()(R()(std::declval<Args>()...))) type;
                typedef std::true_type applied;
            };

            template<typename R, typename ...Args>
            struct make_4_<false, R, Args...>
            {
                typedef R type;
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
              : make_4_<is_transform<R>::value, R, Args...>
            {};

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_2_<R<A...>, Args...>
              : invoke_<
                    R<typename make_3_<A, Args...>::type...>
                  , utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                  , is_transform<R<typename make_3_<A, Args...>::type...>>::value
                  , Args...
                >
            {};

            template<typename R, int I, typename ...Args>
            struct make_2_<_protect<R, I>, Args...>
            {
                typedef _protect<R, I> type;
                typedef std::false_type applied;
            };

            template<typename R, typename ...Args>
            struct make_2_<noinvoke<R>, Args...>
            {
                typedef R type;
                typedef std::false_type applied;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_2_<noinvoke<R<A...>>, Args...>
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
              : invoke_<
                    R<typename make_3_<A, Args...>::type...>
                  , utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                  , is_transform<R<typename make_3_<A, Args...>::type...>>::value
                        && utility::logical_ops::or_(make_3_<A, Args...>::applied::value...)
                  , Args...
                >
            {};

            template<typename R, int I, typename ...Args>
            struct make_1_<_protect<R, I>, Args...>
            {
                typedef _protect<R, I> type;
            };

            template<typename R, typename ...Args>
            struct make_1_<noinvoke<R>, Args...>
            {
                typedef R type;
            };

            template<template<typename...> class R, typename ...A, typename ...Args>
            struct make_1_<noinvoke<R<A...>>, Args...>
            {
                typedef R<typename make_3_<A, Args...>::type...> type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // call_2_
            template<bool NoPad, typename Transform, typename ...Results>
            struct call_2_
            {
                template<typename ...Ts>
                auto operator()(Results &&... results, utility::first<utility::any, Results>..., Ts &&... ts) const
                BOOST_PROTO_AUTO_RETURN(
                    Transform()(static_cast<Results &&>(results)..., static_cast<Ts &&>(ts)...)
                )
            };

            template<typename Transform, typename ...Results>
            struct call_2_<true, Transform, Results...>
            {
                template<typename ...Ts>
                auto operator()(Results &&... results, Ts &&...) const
                BOOST_PROTO_AUTO_RETURN(
                    Transform()(static_cast<Results &&>(results)...)
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
                        (sizeof...(T) <= sizeof...(Args))
                      , Transform
                      , decltype(as_transform<Args>()(static_cast<T &&>(t)...))...
                    >()(as_transform<Args>()(static_cast<T &&>(t)...)..., static_cast<T &&>(t)...)
                )
            };

            /// INTERNAL ONLY : Compile-time optimization for a common case:
            template<typename Transform>
            struct call_1_<true, Transform()>
              : Transform
            {};

            /// INTERNAL ONLY : Compile-time optimization for a common case:
            template<typename Transform>
            struct call_1_<true, Transform(_)>
              : Transform
            {};

            /// INTERNAL ONLY : Compile-time optimization for a common case:
            template<typename Transform>
            struct call_1_<true, Transform(proto::_expr)>
              : Transform
            {};

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

            // Note: Do *NOT* recurse into vararg functions. Those are nested pack transforms,
            // which should not be considered together with this one.
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
                typedef void type(Transforms..., as_transform<Transform>);
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_2_
            template<std::size_t Arity0, std::size_t Arity1 = Arity0>
            struct compute_indices_2_
              : std::conditional<
                    Arity0 == 0
                  , utility::indices<(std::size_t)-1>
                  , utility::make_indices<0, Arity0>
                >
            {
                static_assert(
                    Arity0 == Arity1
                  , "Two pack expressions in unpacking pattern have different arities"
                );
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // compute_indices_1_
            template<typename Transforms, typename ...Args>
            struct compute_indices_1_
            {
                static_assert(
                    utility::never<Transforms>::value
                  , "No pack expression found in unpacking pattern. Use proto::pack(<transform>) "
                    "to designate a transform that returns the expression you want to unpack; "
                    "e.g., proto::pack(_) unpacks the current expression."
                );
            };

            template<typename Head, typename...Tail, typename ...Args>
            struct compute_indices_1_<void(Head, Tail...), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef compute_indices_1_<void(Tail...), Args...> tail;
                typedef typename compute_indices_2_<arity::value, tail::arity::value>::type type;
            };

            template<typename Head, typename ...Args>
            struct compute_indices_1_<void(Head), Args...>
            {
                typedef arity_of<decltype(Head()(std::declval<Args>()...))> arity;
                typedef typename compute_indices_2_<arity::value>::type type;
            };
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
        /// <tt>Bar<_>()</tt> with <tt>proto::terminal\<int\></tt>:
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
        /// proto::terminal<int> i {0};
        ///
        /// int main()
        /// {
        ///     Foo()(i);
        ///     std::cout << typeid(Foo()(i)).name() << std::endl;
        /// }
        /// \endcode
        ///
        /// If you were actually trying to pass the \c _ transform to \c Bar
        /// you can use \c proto::_protect:
        ///
        /// \code
        /// // OK: replace anything with Bar<_protect<_> >()
        /// struct Foo
        ///   : proto::when<_, Bar<_protect<_> >() >
        /// {};
        /// \endcode
        ///
        /// <tt>_protect\<X\></tt> behaves just like <tt>as_transform\<X\></tt>
        /// when used as a transform.
        template<typename T, int I>
        struct _protect
          : T
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(Args...), I>
          : as_transform<R(Args...)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(*)(Args...), I>
          : as_transform<R(Args...)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(Args......), I>
          : as_transform<R(Args......)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(*)(Args......), I>
          : as_transform<R(Args......)>
        {};

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
          : T::proto_transform_type
        {};

        template<typename T, int I>
        struct as_transform<as_transform<T>, I>
          : as_transform<T>
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(*)(Args...), I>
          : as_transform<Ret(Args...), I>
        {};

        template<typename Ret, typename ...Args, int I>
        struct as_transform<Ret(*)(Args......), I>
          : as_transform<Ret(Args......), I>
        {};

        // Handle callable or object transforms
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
