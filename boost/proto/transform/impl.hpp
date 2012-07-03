///////////////////////////////////////////////////////////////////////////////
// impl.hpp
// Helpers for building Proto transforms.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_IMPL_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_IMPL_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/utility.hpp>

namespace boost
{
    namespace proto
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        // evironment_base
        struct environment_base
        {
            void operator[](struct not_a_valid_tag) const noexcept;

            template<typename T>
            T at(utility::any, T && t) const noexcept(noexcept(T(static_cast<T &&>(t))))
            {
                return static_cast<T &&>(t);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // evironment
        // A transform environment is a slot-based storage mechanism, accessible by tag.
        template<typename Tag, typename Value, typename Base /*= environment_base*/>
        struct environment
          : Base
        {
            BOOST_PROTO_REGULAR_TRIVIAL_CLASS(environment);

            Value value_;

            template<typename V, typename B = Base
              , BOOST_PROTO_ENABLE_IF(!(utility::is_base_of<environment, V>::value))>
            explicit environment(V && v, B && b = B())
              : Base(static_cast<B &&>(b))
              , value_(static_cast<V &&>(v))
            {}

            // For key-based lookups not intended to fail
            using Base::operator[];
            auto operator[](Tag) const
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<environment const &>(*this).value_)
            )

            // For key-based lookups that can fail, use the default if key not found.
            using Base::at;
            template<typename T>
            auto at(Tag, T &&) const
            BOOST_PROTO_AUTO_RETURN(
                (static_cast<environment const &>(*this).value_)
            )

            template<typename T, typename V>
            friend environment<T, V, environment> operator,(environment tail, environment<T, V> head)
            {
                return environment<T, V, environment>{
                    static_cast<environment<T, V> &&>(head).value_
                  , static_cast<environment &&>(tail)
                };
            }
        };

        namespace detail
        {
            struct make_env_impl_
            {
                template<typename T>
                static T make_env(T && t)
                {
                    static_assert(
                        utility::is_base_of<environment_base, T>::value
                      , "make_env used in non-environment context"
                    );
                    return t;
                }

                template<typename T, typename U, typename ...V, typename Impl = make_env_impl_>
                static auto make_env(T && t, U && u, V &&... v)
                BOOST_PROTO_AUTO_RETURN(
                    Impl::make_env((static_cast<T &&>(t), static_cast<U &&>(u)), static_cast<V &&>(v)...)
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // make_env
        template<typename ...T>
        inline auto make_env(T &&... t)
        BOOST_PROTO_AUTO_RETURN(
            detail::make_env_impl_::make_env(static_cast<T &&>(t)...)
        )

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

            // Sometimes it's helpful to know why a given transform can't be applied to a
            // set of arguments. The following code can help track it down.
            //typedef utility::any (*non_deducable_args)(...);
            //operator non_deducable_args() const
            //{
            //    static_assert(
            //        utility::never<Transform>::value
            //      , "The specified transform was passed arguments that it doesn't know "
            //        "how to process correctly"
            //    );
            //    return 0;
            //}
        };

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

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // select_value
            template<typename T, typename U>
            inline T && select_value(T &&t, U &&) noexcept
            {
                return static_cast<T &&>(t);
            }

            template<typename U>
            inline U && select_value(utility::void_type const &, U &&u) noexcept
            {
                return static_cast<U &&>(u);
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // invoke_transform_2_
            template<std::size_t N, typename Sig>
            struct invoke_transform_2_;

            template<std::size_t N, typename Transform, typename ...Args>
            struct invoke_transform_2_<N, Transform(Args...)>
              : invoke_transform_2_<N - 1, Transform(Args..., _void)>
            {};

            template<typename Transform, typename ...Args>
            struct invoke_transform_2_<0, Transform(Args...)>
            {
                template<typename ...T, BOOST_PROTO_ENABLE_IF(sizeof...(T) == sizeof...(Args))>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    typename Transform::proto_transform_type()(
                        detail::select_value(
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
            // invoke_transform_1_
            template<bool IsTransform, typename Sig>
            struct invoke_transform_1_;

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle transforms.
            template<typename Transform, typename ...Args>
            struct invoke_transform_1_<true, Transform(Args...)>
            {
                template<typename ...T>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    invoke_transform_2_<
                        (sizeof...(T) > sizeof...(Args) ? sizeof...(T) - sizeof...(Args) : 0)
                      , Transform(Args...)
                    >()(static_cast<T &&>(t)...)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle callables and object constructions.
            template<typename CallOrObj, typename ...Args>
            struct invoke_transform_1_<false, CallOrObj(Args...)>
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
                detail::invoke_transform_1_<is_transform<X>::value, X(Args...)>()(
                    static_cast<T &&>(t)...
                )
            )
        };
    }
}

#endif
