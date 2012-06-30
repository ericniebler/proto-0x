////////////////////////////////////////////////////////////////////////////////////////////////////
// recursive_fold.hpp
// Contains definition of the recursive_fold<> and reverse_recursive_fold<> transforms.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_RECURSIVE_FOLD_HPP_INCLUDED
#define BOOST_PROTO_TRANSFORM_RECURSIVE_FOLD_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/matches.hpp>
#include <boost/proto/transform/fold.hpp>
#include <boost/proto/transform/impl.hpp>

namespace boost { namespace proto
{
    namespace detail
    {
        template<typename Tag>
        struct has_tag
          : transform<has_tag<Tag>>
        {
            template<typename E, typename ...Rest>
            std::integral_constant<bool, std::is_same<typename E::proto_tag_type, Tag>::value>
            operator()(E const &, Rest &&...) const;
        };

        template<typename Tag, typename Fun>
        struct recursive_fold_
          : if_<has_tag<Tag>, fold<_, _state, recursive_fold_<Tag, Fun>>, Fun>
        {};

        template<typename Tag, typename Fun>
        struct reverse_recursive_fold_
          : if_<has_tag<Tag>, reverse_fold<_, _state, reverse_recursive_fold_<Tag, Fun>>, Fun>
        {};
    }

    /// \brief A PrimitiveTransform that recursively applies the
    /// <tt>fold\<\></tt> transform to sub-trees that all share a common
    /// tag type.
    ///
    /// <tt>recursive_fold\<\></tt> is useful for flattening trees into lists;
    /// for example, you might use <tt>recursive_fold\<\></tt> to flatten an
    /// expression tree like <tt>a | b | c</tt> into a Fusion list like
    /// <tt>cons(c, cons(b, cons(a)))</tt>.
    ///
    /// <tt>recursive_fold\<\></tt> is easily understood in terms of a
    /// <tt>recurse_if_\<\></tt> helper, defined as follows:
    ///
    /// \code
    /// template<typename Tag, typename Fun>
    /// struct recurse_if_
    ///   : if_<
    ///         // If the current node has type type "Tag" ...
    ///         is_same<tag_of<_>, Tag>()
    ///         // ... recurse, otherwise ...
    ///       , fold<_, _state, recurse_if_<Tag, Fun>>
    ///         // ... apply the Fun transform.
    ///       , Fun
    ///     >
    /// {};
    /// \endcode
    ///
    /// With <tt>recurse_if_\<\></tt> as defined above,
    /// <tt>recursive_fold\<Sequence, State0, Fun\>()(e, s, d)</tt> is
    /// equivalent to
    /// <tt>fold<Sequence, State0, recurse_if_<Expr::proto_tag, Fun>>()(e, s, d).</tt>
    /// It has the effect of folding a tree front-to-back, recursing into
    /// child nodes that share a tag type with the parent node.
    template<typename Sequence, typename State0, typename Fun>
    struct recursive_fold
      : transform<recursive_fold<Sequence, State0, Fun>>
    {
        template<typename E, typename ...Rest>
        auto operator()(E && e, Rest &&... rest) const
        BOOST_PROTO_AUTO_RETURN(
            fold<
                Sequence
              , State0
              , detail::recursive_fold_<typename tag_of<E>::type, Fun>
            >()(
                static_cast<E &&>(e)
              , static_cast<Rest &&>(rest)...
            )
        )
    };

    /// \brief A PrimitiveTransform that recursively applies the
    /// <tt>reverse_fold\<\></tt> transform to sub-trees that all share
    /// a common tag type.
    ///
    /// <tt>reverse_recursive_fold\<\></tt> is useful for flattening trees into
    /// lists; for example, you might use <tt>reverse_recursive_fold\<\></tt> to
    /// flatten an expression tree like <tt>a | b | c</tt> into a Fusion list
    /// like <tt>cons(a, cons(b, cons(c)))</tt>.
    ///
    /// <tt>reverse_recursive_fold\<\></tt> is easily understood in terms of a
    /// <tt>recurse_if_\<\></tt> helper, defined as follows:
    ///
    /// \code
    /// template<typename Tag, typename Fun>
    /// struct recurse_if_
    ///   : if_<
    ///         // If the current node has type type "Tag" ...
    ///         is_same<tag_of<_>, Tag>()
    ///         // ... recurse, otherwise ...
    ///       , reverse_fold<_, _state, recurse_if_<Tag, Fun>>
    ///         // ... apply the Fun transform.
    ///       , Fun
    ///     >
    /// {};
    /// \endcode
    ///
    /// With <tt>recurse_if_\<\></tt> as defined above,
    /// <tt>reverse_recursive_fold\<Sequence, State0, Fun\>()(e, s, d)</tt> is
    /// equivalent to
    /// <tt>reverse_fold<Sequence, State0, recurse_if_<Expr::proto_tag, Fun>>()(e, s, d).</tt>
    /// It has the effect of folding a tree back-to-front, recursing into
    /// child nodes that share a tag type with the parent node.
    template<typename Sequence, typename State0, typename Fun>
    struct reverse_recursive_fold
      : transform<reverse_recursive_fold<Sequence, State0, Fun>>
    {
        template<typename E, typename ...Rest>
        auto operator()(E && e, Rest &&... rest) const
        BOOST_PROTO_AUTO_RETURN(
            reverse_fold<
                Sequence
              , State0
              , detail::reverse_recursive_fold_<typename tag_of<E>::type, Fun>
            >()(
                static_cast<E &&>(e)
              , static_cast<Rest &&>(rest)...
            )
        )
    };
}}

#endif
