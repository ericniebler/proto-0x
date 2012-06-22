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
            T && at(utility::any, T && t) const noexcept
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
              , BOOST_PROTO_ENABLE_IF(!BOOST_PROTO_IS_CONVERTIBLE(V, environment))>
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

        ////////////////////////////////////////////////////////////////////////////////////////////
        // callable
        struct callable
        {};

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // is_callable_
            std::true_type is_callable_(callable const &);
            std::false_type is_callable_(utility::any const &);

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_transform_
            std::true_type is_transform_(transform_base const &);
            std::false_type is_transform_(utility::any const &);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_callable
        template<typename T>
        struct is_callable
          : decltype(detail::is_callable_(std::declval<T>()))
        {};

        ////////////////////////////////////////////////////////////////////////////////////////////
        // transform_base
        struct transform_base
          : callable
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
        /// it from being applied. You can use <tt>proto::protect\<\></tt>
        /// as follows:
        ///
        /// \code
        /// // OK: replace anything with Bar<_>()
        /// struct Foo
        ///   : proto::when<_, Bar<protect<_> >() >
        /// {};
        /// \endcode
        template<typename T>
        struct protect
          : transform<protect<T>>
        {
            template<typename... Args>
            T operator()(Args &&...) const noexcept(noexcept(T{}))
            {
                return T{};
            }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // is_transform
        template<typename T>
        struct is_transform
          : decltype(detail::is_transform_(std::declval<T>()))
        {};

        template<typename R, typename...A>
        struct is_transform<R(A...)>
          : std::true_type
        {};

        template<typename R, typename...A>
        struct is_transform<R(*)(A...)>
          : std::true_type
        {};

        namespace detail
        {
            struct void_
              : transform<void_>
            {
                template<typename ...T>
                void_ operator()(T &&...) const noexcept
                {
                    return void_{};
                }
            };

            template<typename T, typename U>
            T && either_or(T &&t, U &&) noexcept
            {
                return static_cast<T &&>(t);
            }

            template<typename U>
            U && either_or(void_, U &&u) noexcept
            {
                return static_cast<U &&>(u);
            }

            template<std::size_t N, std::size_t M, typename ...T>
            struct pad_until
              : pad_until<N, M+1, T..., void_>
            {};

            template<std::size_t N, typename ...T>
            struct pad_until<N, N, T...>
            {
                typedef utility::list<T...> type;
            };

            template<typename O, typename Args>
            struct invoke_transform;

            template<typename O, typename ...Args>
            struct invoke_transform<O, utility::list<Args...>>
            {
                template<typename ...T>
                auto operator()(T &&... t) const
                BOOST_PROTO_AUTO_RETURN(
                    typename O::proto_transform_type()(
                        detail::either_or(
                            as_transform<Args>()(static_cast<T &&>(t)...)
                          , static_cast<T &&>(t)
                        )...
                    )
                )
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // as_transform
        template<typename T, bool B>
        struct as_transform
          : protect<T>
        {};

        template<typename T>
        struct as_transform<T, true>
          : T::proto_transform_type
        {};

        template<typename Ret, typename ...Args>
        struct as_transform<Ret(Args...), true>
          : transform<as_transform<Ret(Args...)>>
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle transforms.
            template<
                typename ...T
              , typename Transform = typename detail::make_1_<Ret, T...>::type
              , BOOST_PROTO_ENABLE_IF(is_transform<Transform>::value)
            >
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                detail::invoke_transform<
                    Transform
                  , typename detail::pad_until<sizeof...(T), sizeof...(Args), Args...>::type
                >()(static_cast<T &&>(t)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle callables that are not transforms
            template<
                typename ...T
              , typename Callable = typename detail::make_1_<Ret, T...>::type
              , BOOST_PROTO_ENABLE_IF(!is_transform<Callable>::value)
              , BOOST_PROTO_ENABLE_IF_VALID_EXPR(
                    Callable{}(as_transform<Args>()(std::declval<T>()...)...)
                )
            >
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                Callable{}(as_transform<Args>()(static_cast<T &&>(t)...)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // Handle objects
            template<
                typename ...T
              , typename Object = typename detail::make_1_<Ret, T...>::type
              , BOOST_PROTO_ENABLE_IF(!is_transform<Object>::value)
              , BOOST_PROTO_ENABLE_IF_VALID_EXPR(
                    Object{as_transform<Args>()(std::declval<T>()...)...}
                )
            >
            auto operator()(T &&... t) const
            BOOST_PROTO_AUTO_RETURN(
                Object{as_transform<Args>()(static_cast<T &&>(t)...)...}
            )
        };

        template<typename Ret, typename ...Args>
        struct as_transform<Ret(*)(Args...), true>
          : as_transform<Ret(Args...)>
        {};
    }
}

#endif
