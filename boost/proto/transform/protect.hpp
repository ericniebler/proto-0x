///////////////////////////////////////////////////////////////////////////////
// protect.hpp
// Keep a transform from being applied when building the return type of
// proto::construct.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_PROTECT_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_PROTECT_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/transform/base.hpp>

namespace boost
{
    namespace proto
    {
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
        ///   : proto::when<_, proto::construct( Bar<_protect<_> >() ) >
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
    }
}

#endif
