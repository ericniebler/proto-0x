////////////////////////////////////////////////////////////////////////////////////////////////////
// fusion.hpp
// Make any Proto expression a valid Fusion sequence
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_FUSION_HPP_INCLUDED
#define BOOST_PROTO_FUSION_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <boost/mpl/sequence_tag_fwd.hpp>
#include <boost/fusion/include/is_view.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>
#include <boost/fusion/include/category_of.hpp>
#include <boost/fusion/include/iterator_base.hpp>
#include <boost/fusion/include/intrinsic.hpp>
#include <boost/fusion/include/single_view.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/as_list.hpp>
#include <boost/fusion/include/is_segmented.hpp>
#include <boost/fusion/sequence/comparison/enable_comparison.hpp>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/children.hpp>
#include <boost/proto/cxx11/expr.hpp>
#include <boost/proto/cxx11/unpack_expr.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Expr, std::size_t Pos>
            struct expr_iterator
              : fusion::iterator_base<expr_iterator<Expr, Pos>>
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(expr_iterator);

                static constexpr std::size_t index = Pos;
                using expr_type = Expr;
                using category = fusion::random_access_traversal_tag;
                using fusion_tag =
                    proto_expr_iterator<
                        typename Expr::proto_tag_type
                      , typename Expr::proto_domain_type
                    >;

                explicit expr_iterator(Expr &e) noexcept
                  : expr_(e)
                {}

                Expr &expr_;
            };

            template<typename Tag>
            struct as_element
            {
                template<typename Sig>
                struct result;

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result<This(Expr const &)>
                {};

                template<typename This, typename Expr>
                struct result<This(Expr &)>
                  : std::conditional<
                        is_same<Tag, typename Expr::proto_tag_type>::value
                      , flat_view<Expr>
                      , fusion::single_view<Expr &>
                    >
                {};

                template<typename Expr>
                typename result<as_element(Expr &)>::type operator ()(Expr &e) const
                {
                    return typename result<as_element(Expr &)>::type(e);
                }

                template<typename Expr>
                typename result<as_element(Expr const &)>::type operator ()(Expr const &e) const
                {
                    return typename result<as_element(Expr const &)>::type(e);
                }
            };

            template<typename Expr>
            struct flat_view
              : fusion::sequence_base<flat_view<Expr>> 
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(flat_view);

                using expr_type = Expr;
                using category = fusion::forward_traversal_tag;
                using fusion_tag =
                    proto_flat_view<
                        typename Expr::proto_tag_type
                      , typename Expr::proto_domain_type
                    >;
                using segments_type =
                    typename fusion::result_of::as_list<
                        typename fusion::result_of::transform<
                            Expr
                          , as_element<typename Expr::proto_tag_type>
                        >::type
                    >::type;

                explicit flat_view(Expr &e)
                  : segs_(fusion::as_list(
                        fusion::transform(e, as_element<typename Expr::proto_tag_type>())
                    ))
                {}

                segments_type segs_;
            };
        }

        namespace result_of
        {
            template<typename Expr>
            struct flatten
              : flatten<Expr const &>
            {};

            template<typename Expr>
            struct flatten<Expr &>
            {
                using type = detail::flat_view<Expr>;
            };
        }

        namespace functional
        {
            /// \brief A PolymorphicFunctionObject type that returns a "flattened"
            /// view of a Proto expression tree.
            ///
            /// A PolymorphicFunctionObject type that returns a "flattened"
            /// view of a Proto expression tree. For a tree with a top-most node
            /// tag of type \c T, the elements of the flattened sequence are
            /// determined by recursing into each child node with the same
            /// tag type and returning those nodes of different type. So for
            /// instance, the Proto expression tree corresponding to the
            /// expression <tt>a | b | c</tt> has a flattened view with elements
            /// [a, b, c], even though the tree is grouped as
            /// <tt>((a | b) | c)</tt>.
            struct flatten
            {
                template<typename Sig>
                struct result;

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result<This(Expr const &)>
                {};

                template<typename This, typename Expr>
                struct result<This(Expr &)>
                {
                    using type = proto::detail::flat_view<Expr>;
                };

                template<typename Expr>
                proto::detail::flat_view<Expr> operator ()(Expr &e) const
                {
                    return proto::detail::flat_view<Expr>(e);
                }

                template<typename Expr>
                proto::detail::flat_view<Expr const> operator ()(Expr const &e) const
                {
                    return proto::detail::flat_view<Expr const>(e);
                }
            };
        }

        /// \brief A function that returns a "flattened"
        /// view of a Proto expression tree.
        ///
        /// For a tree with a top-most node
        /// tag of type \c T, the elements of the flattened sequence are
        /// determined by recursing into each child node with the same
        /// tag type and returning those nodes of different type. So for
        /// instance, the Proto expression tree corresponding to the
        /// expression <tt>a | b | c</tt> has a flattened view with elements
        /// [a, b, c], even though the tree is grouped as
        /// <tt>((a | b) | c)</tt>.
        template<typename Expr>
        proto::detail::flat_view<Expr> flatten(Expr &e)
        {
            return proto::detail::flat_view<Expr>(e);
        }

        /// \overload
        ///
        template<typename Expr>
        proto::detail::flat_view<Expr const> flatten(Expr const &e)
        {
            return proto::detail::flat_view<Expr const>(e);
        }
    }
}

namespace boost
{
    namespace fusion
    {
        namespace extension
        {
            template<typename Tag>
            struct is_sequence_impl;

            template<typename Tag, typename Domain>
            struct is_sequence_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag, typename Domain>
            struct is_sequence_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag>
            struct is_view_impl;

            template<typename Tag, typename Domain>
            struct is_view_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag, typename Domain>
            struct is_view_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                  : std::false_type
                {};
            };

            template<typename Tag>
            struct value_of_impl;

            template<typename Tag, typename Domain>
            struct value_of_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<typename Iterator>
                struct apply
                  : proto::exprs::children_element<
                        Iterator::index
                      , typename Iterator::expr_type::proto_children_type
                    >
                {};
            };

            template<typename Tag>
            struct deref_impl;

            template<typename Tag, typename Domain>
            struct deref_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<
                    typename Iterator
                  , std::size_t Arity = proto::arity_of<typename Iterator::expr_type>::value
                >
                struct apply
                {
                    using type =
                        typename proto::result_of::child<
                            typename Iterator::expr_type &
                          , (std::size_t)Iterator::index
                        >::type;

                    static type call(Iterator const &iter)
                    {
                        return proto::child<(std::size_t)Iterator::index>(iter.expr_);
                    }
                };

                template<typename Iterator>
                struct apply<Iterator, 0>
                {
                    using type =
                        typename proto::result_of::value<
                            typename Iterator::expr_type &
                        >::type;

                    static type call(Iterator const &iter)
                    {
                        return proto::value(iter.expr_);
                    }
                };
            };

            template<typename Tag>
            struct advance_impl;

            template<typename Tag, typename Domain>
            struct advance_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<typename Iterator, typename N>
                struct apply
                {
                    static constexpr std::ptrdiff_t signed_index =
                        (std::ptrdiff_t)Iterator::index + (std::ptrdiff_t)N::value;

                    // Range-check the result
                    static_assert(
                        0 <= signed_index
                      , "Cannot advance before the beginning of the sequence"
                    );

                    // index == size allowed for 1-past the last element.
                    static_assert(
                        Iterator::expr_type::proto_size::value >= (std::size_t)signed_index
                      , "Cannot advance past the end of the sequence"
                    );

                    using type =
                        proto::detail::expr_iterator<
                            typename Iterator::expr_type
                          , (std::size_t)signed_index
                        >;

                    static type call(Iterator const &iter)
                    {
                        return type(iter.expr_);
                    }
                };
            };

            template<typename Tag>
            struct distance_impl;

            template<typename Tag, typename Domain>
            struct distance_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<typename IteratorFrom, typename IteratorTo>
                struct apply
                  : std::integral_constant<
                        int
                      , (int)((std::ptrdiff_t)IteratorTo::index - (std::ptrdiff_t)IteratorFrom::index)
                    >
                {};
            };

            template<typename Tag>
            struct next_impl;

            template<typename Tag, typename Domain>
            struct next_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<typename Iterator>
                struct apply
                  : advance_impl<proto::proto_expr_iterator<Tag, Domain>>::template apply<
                        Iterator
                      , std::integral_constant<int, 1>
                    >
                {};
            };

            template<typename Tag>
            struct prior_impl;

            template<typename Tag, typename Domain>
            struct prior_impl<proto::proto_expr_iterator<Tag, Domain>>
            {
                template<typename Iterator>
                struct apply
                  : advance_impl<proto::proto_expr_iterator<Tag, Domain>>::template apply<
                        Iterator
                      , std::integral_constant<int, -1>
                    >
                {};
            };

            template<typename Tag>
            struct category_of_impl;

            template<typename Tag, typename Domain>
            struct category_of_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type = random_access_traversal_tag;
                };
            };

            template<typename Tag>
            struct size_impl;

            template<typename Tag, typename Domain>
            struct size_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                  : Sequence::proto_size
                {};
            };

            template<typename Tag>
            struct begin_impl;

            template<typename Tag, typename Domain>
            struct begin_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type =
                        proto::detail::expr_iterator<
                            Sequence
                          , 0
                        >;

                    static type call(Sequence &seq)
                    {
                        return type(seq);
                    }
                };
            };

            template<typename Tag>
            struct end_impl;

            template<typename Tag, typename Domain>
            struct end_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type =
                        proto::detail::expr_iterator<
                            Sequence
                          , Sequence::proto_size::value
                        >;

                    static type call(Sequence &seq)
                    {
                        return type(seq);
                    }
                };
            };

            template<typename Tag>
            struct value_at_impl;

            template<typename Tag, typename Domain>
            struct value_at_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence, typename Index>
                struct apply
                  : proto::exprs::children_element<(std::size_t)Index::value, typename Sequence::proto_children_type>
                {};
            };

            template<typename Tag>
            struct at_impl;

            template<typename Tag, typename Domain>
            struct at_impl<proto::proto_expr<Tag, Domain>>
            {
                template<
                    typename Sequence
                  , typename Index
                  , std::size_t Arity = proto::arity_of<Sequence>::value
                >
                struct apply
                {
                    using type =
                        typename proto::result_of::child<
                            Sequence &
                          , (std::size_t)Index::value
                        >::type;

                    static type call(Sequence &seq)
                    {
                        return proto::child<(std::size_t)Index::value>(seq);
                    }
                };

                template<typename Sequence, typename Index>
                struct apply<Sequence, Index, 0>
                {
                    using type =
                        typename proto::result_of::value<
                            Sequence &
                        >::type;

                    static type call(Sequence &seq)
                    {
                        return proto::value(seq);
                    }
                };
            };

            template<typename Tag>
            struct convert_impl;

            template<typename Tag, typename Domain>
            struct convert_impl<proto::proto_expr<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type =
                        typename proto::domains::result_of::unpack_expr<
                            Domain
                          , Tag
                          , Sequence &
                        >::type;

                    static type call(Sequence& seq)
                    {
                        return proto::domains::unpack_expr<Domain>(Tag(), seq);
                    }
                };
            };

            template<typename Tag, typename Domain>
            struct convert_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type =
                        typename proto::domains::result_of::unpack_expr<
                            Domain
                          , Tag
                          , Sequence &
                        >::type;

                    static type call(Sequence & seq)
                    {
                        return proto::domains::unpack_expr<Domain>(Tag(), seq);
                    }
                };
            };

            template<typename Tag>
            struct is_segmented_impl;

            template<typename Tag, typename Domain>
            struct is_segmented_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Iterator>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag>
            struct segments_impl;

            template<typename Tag, typename Domain>
            struct segments_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type = typename Sequence::segments_type const &;

                    static type call(Sequence &sequence)
                    {
                        return sequence.segs_;
                    }
                };
            };

            template<typename Tag, typename Domain>
            struct category_of_impl<proto::proto_flat_view<Tag, Domain>>
            {
                template<typename Sequence>
                struct apply
                {
                    using type = forward_traversal_tag;
                };
            };

        }

        namespace traits
        {
            template<typename Seq1, typename Seq2>
            struct enable_equality<
                Seq1
              , Seq2
              , typename std::enable_if<
                    proto::is_expr<Seq1>::value ||
                    proto::is_expr<Seq2>::value
                >::type
            >
                : std::false_type
            {};

            template<typename Seq1, typename Seq2>
            struct enable_comparison<
                Seq1
              , Seq2
              , typename std::enable_if<
                    proto::is_expr<Seq1>::value ||
                    proto::is_expr<Seq2>::value
                >::type
            >
              : std::false_type
            {};
        }
    }
}

namespace boost
{
    namespace mpl
    {
        template<typename ExprDesc, typename Domain>
        struct sequence_tag<proto::exprs::expr<ExprDesc, Domain>>
        {
            using type = fusion::fusion_sequence_tag;
        };

        template<typename ExprDesc, typename Domain>
        struct sequence_tag<proto::exprs::basic_expr<ExprDesc, Domain>>
        {
            using type = fusion::fusion_sequence_tag;
        };
    }
}

#endif
