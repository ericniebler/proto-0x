///////////////////////////////////////////////////////////////////////////////
// protect.hpp
// Keep a basic_action from being applied when building the return type of
// proto::construct.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_ACTION_PROTECT_HPP_INCLUDED
#define BOOST_PROTO_ACTION_PROTECT_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/action/base.hpp>
#include <boost/proto/action/action.hpp>

namespace boost
{
    namespace proto
    {
        /// \brief A BasicAction which prevents another BasicAction
        /// from being applied in an \c ObjectAction.
        ///
        /// When building higher order actions with <tt>make\<\></tt> or
        /// <tt>lazy\<\></tt>, you sometimes would like to build types that
        /// are parameterized with Proto actions. In such lambda-style
        /// actions, Proto will unhelpfully find all nested actions
        /// and apply them, even if you don't want them to be applied. Consider
        /// the following basic_action, which will replace the \c _ in
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
        /// If you were actually trying to pass the \c _ basic_action to \c Bar
        /// you can use \c proto::_protect:
        ///
        /// \code
        /// // OK: replace anything with Bar<_protect<_> >()
        /// struct Foo
        ///   : proto::when<_, proto::construct( Bar<_protect<_> >() ) >
        /// {};
        /// \endcode
        ///
        /// <tt>_protect\<X\></tt> behaves just like <tt>action\<X\></tt>
        /// when used as a basic_action.
        template<typename T, int I>
        struct _protect
          : T
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(Args...), I>
          : action<R(Args...)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(*)(Args...), I>
          : action<R(Args...)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(Args......), I>
          : action<R(Args......)>
        {};

        template<typename R, typename...Args, int I>
        struct _protect<R(*)(Args......), I>
          : action<R(Args......)>
        {};
    }
}

#endif
