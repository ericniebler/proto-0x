////////////////////////////////////////////////////////////////////////////////////////////////////
// recursive_fold.hpp
// Contains definition of the _recursive_fold<> and _reverse_recursive_fold<> actions.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_ACTION_RECURSIVE_FOLD_HPP_INCLUDED
#define BOOST_PROTO_V5_ACTION_RECURSIVE_FOLD_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/matches.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/if.hpp>
#include <boost/proto/v5/action/fold.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename Tag>
                struct _has_tag
                  : basic_action<_has_tag<Tag>>
                {
                    template<typename E, typename ...Rest>
                    constexpr std::integral_constant<
                        bool
                      , std::is_same<typename E::proto_tag_type, Tag>::value
                    >
                    operator()(E const &, Rest &&...) const
                    {
                        return std::integral_constant<
                            bool
                          , std::is_same<typename E::proto_tag_type, Tag>::value
                        >();
                    }
                };

                template<typename Tag, typename Fun>
                struct _recursive_fold_
                  : as_action_<
                        if_(
                            _has_tag<Tag>
                          , fold(_, _state, _recursive_fold_<Tag, Fun>)
                          , Fun
                        )
                    >
                {};

                template<typename Tag, typename Fun>
                struct _reverse_recursive_fold_
                  : as_action_<
                        if_(
                            _has_tag<Tag>
                          , reverse_fold(_, _state, _reverse_recursive_fold_<Tag, Fun>)
                          , Fun
                        )
                    >
                {};

                /// \brief A BasicAction that recursively applies the
                /// <tt>_fold\<\></tt> basic_action to sub-trees that all share a common
                /// tag type.
                ///
                /// <tt>_recursive_fold\<\></tt> is useful for flattening trees into lists;
                /// for example, you might use <tt>_recursive_fold\<\></tt> to flatten an
                /// expression tree like <tt>a | b | c</tt> into a Fusion list like
                /// <tt>cons(c, cons(b, cons(a)))</tt>.
                ///
                /// <tt>_recursive_fold\<\></tt> is easily understood in terms of a
                /// <tt>recurse_if_\<\></tt> helper, defined as follows:
                ///
                /// \code
                /// template<typename Tag, typename Fun>
                /// struct recurse_if_
                ///   : if_<
                ///         // If the current node has type type "Tag" ...
                ///         is_same<tag_of<_>, Tag>()
                ///         // ... recurse, otherwise ...
                ///       , _fold<_, _state, recurse_if_<Tag, Fun>>
                ///         // ... apply the Fun basic_action.
                ///       , Fun
                ///     >
                /// {};
                /// \endcode
                ///
                /// With <tt>recurse_if_\<\></tt> as defined above,
                /// <tt>_recursive_fold\<Sequence, State0, Fun\>()(e, s, d)</tt> is
                /// equivalent to
                /// <tt>_fold<Sequence, State0, recurse_if_<Expr::proto_tag, Fun>>()(e, s, d).</tt>
                /// It has the effect of folding a tree front-to-back, recursing into
                /// child nodes that share a tag type with the parent node.
                template<typename Sequence, typename State0, typename Fun>
                struct _recursive_fold
                  : basic_action<_recursive_fold<Sequence, State0, Fun>>
                {
                    template<typename E, typename ...Rest>
                    constexpr auto operator()(E && e, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        call_action_<
                            fold(
                                Sequence
                              , State0
                              , detail::_recursive_fold_<typename result_of::tag_of<E>::type, Fun>
                            )
                        >()(
                            static_cast<E &&>(e)
                          , static_cast<Rest &&>(rest)...
                        )
                    )
                };

                /// \brief A BasicAction that recursively applies the
                /// <tt>_reverse_fold\<\></tt> basic_action to sub-trees that all share
                /// a common tag type.
                ///
                /// <tt>_reverse_recursive_fold\<\></tt> is useful for flattening trees into
                /// lists; for example, you might use <tt>_reverse_recursive_fold\<\></tt> to
                /// flatten an expression tree like <tt>a | b | c</tt> into a Fusion list
                /// like <tt>cons(a, cons(b, cons(c)))</tt>.
                ///
                /// <tt>_reverse_recursive_fold\<\></tt> is easily understood in terms of a
                /// <tt>recurse_if_\<\></tt> helper, defined as follows:
                ///
                /// \code
                /// template<typename Tag, typename Fun>
                /// struct recurse_if_
                ///   : if_<
                ///         // If the current node has type type "Tag" ...
                ///         is_same<tag_of<_>, Tag>()
                ///         // ... recurse, otherwise ...
                ///       , _reverse_fold<_, _state, recurse_if_<Tag, Fun>>
                ///         // ... apply the Fun basic_action.
                ///       , Fun
                ///     >
                /// {};
                /// \endcode
                ///
                /// With <tt>recurse_if_\<\></tt> as defined above,
                /// <tt>_reverse_recursive_fold\<Sequence, State0, Fun\>()(e, s, d)</tt> is
                /// equivalent to
                /// <tt>_reverse_fold<Sequence, State0, recurse_if_<Expr::proto_tag, Fun>>()(e, s, d).</tt>
                /// It has the effect of folding a tree back-to-front, recursing into
                /// child nodes that share a tag type with the parent node.
                template<typename Sequence, typename State0, typename Fun>
                struct _reverse_recursive_fold
                  : basic_action<_reverse_recursive_fold<Sequence, State0, Fun>>
                {
                    template<typename E, typename ...Rest>
                    constexpr auto operator()(E && e, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        call_action_<
                            reverse_fold(
                                Sequence
                              , State0
                              , detail::_reverse_recursive_fold_<typename result_of::tag_of<E>::type, Fun>
                            )
                        >()(
                            static_cast<E &&>(e)
                          , static_cast<Rest &&>(rest)...
                        )
                    )
                };
            }

            struct recursive_fold
            {};

            struct reverse_recursive_fold
            {};

            namespace extension
            {
                template<typename Seq, typename State0, typename Fun>
                struct action_impl<recursive_fold(Seq, State0, Fun)>
                  : detail::_recursive_fold<Seq, State0, Fun>
                {};

                template<typename Seq, typename State0, typename Fun>
                struct action_impl<reverse_recursive_fold(Seq, State0, Fun)>
                  : detail::_reverse_recursive_fold<Seq, State0, Fun>
                {};
            }
        }
    }
}

#endif
