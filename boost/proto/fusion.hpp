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
#include <boost/fusion/include/transform_view.hpp>
#include <boost/fusion/include/is_segmented.hpp>
#include <boost/fusion/sequence/comparison/enable_comparison.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/children.hpp>
#include <boost/proto/expr.hpp>

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

                typedef Expr expr_type;
                static constexpr std::size_t index = Pos;
                typedef fusion::random_access_traversal_tag category;
                typedef proto_expr_iterator fusion_tag;

                explicit expr_iterator(Expr &e)
                  : expr_(e)
                {}

                Expr &expr_;
            };

            template<typename Expr>
            struct flat_view
            {
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(flat_view);

                typedef Expr expr_type;
                typedef fusion::forward_traversal_tag category;
                typedef proto_flat_view fusion_tag;

                explicit flat_view(Expr &e)
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
                typedef detail::flat_view<Expr> type;
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
                    typedef proto::detail::flat_view<Expr> type;
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

            template<>
            struct is_sequence_impl<proto::proto_flat_view>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<>
            struct is_sequence_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag>
            struct is_view_impl;

            template<>
            struct is_view_impl<proto::proto_flat_view>
            {
                template<typename Sequence>
                struct apply
                  : std::true_type
                {};
            };

            template<>
            struct is_view_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                  : std::false_type
                {};
            };

            template<typename Tag>
            struct value_of_impl;

            template<>
            struct value_of_impl<proto::proto_expr_iterator>
            {
                template<typename Iterator>
                struct apply
                  : proto::exprs::children_element<Iterator::index, typename Iterator::expr_type::proto_children_type>
                {};
            };

            template<typename Tag>
            struct deref_impl;

            template<>
            struct deref_impl<proto::proto_expr_iterator>
            {
                template<
                    typename Iterator
                  , std::size_t Arity = proto::arity_of<typename Iterator::expr_type>::value
                >
                struct apply
                {
                    typedef
                        typename proto::result_of::child<
                            typename Iterator::expr_type &
                          , (std::size_t)Iterator::index
                        >::type
                    type;

                    static type call(Iterator const &iter)
                    {
                        return proto::child<(std::size_t)Iterator::index>(iter.expr_);
                    }
                };

                template<typename Iterator>
                struct apply<Iterator, 0>
                {
                    typedef
                        typename proto::result_of::value<
                            typename Iterator::expr_type &
                        >::type
                    type;

                    static type call(Iterator const &iter)
                    {
                        return proto::value(iter.expr_);
                    }
                };
            };

            template<typename Tag>
            struct advance_impl;

            template<>
            struct advance_impl<proto::proto_expr_iterator>
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

                    typedef
                        proto::detail::expr_iterator<
                            typename Iterator::expr_type
                          , (std::size_t)signed_index
                        >
                    type;

                    static type call(Iterator const &iter)
                    {
                        return type(iter.expr_);
                    }
                };
            };

            template<typename Tag>
            struct distance_impl;

            template<>
            struct distance_impl<proto::proto_expr_iterator>
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

            template<>
            struct next_impl<proto::proto_expr_iterator>
            {
                template<typename Iterator>
                struct apply
                  : advance_impl<proto::proto_expr_iterator>::template apply<
                        Iterator
                      , std::integral_constant<int, 1>
                    >
                {};
            };

            template<typename Tag>
            struct prior_impl;

            template<>
            struct prior_impl<proto::proto_expr_iterator>
            {
                template<typename Iterator>
                struct apply
                  : advance_impl<proto::proto_expr_iterator>::template apply<
                        Iterator
                      , std::integral_constant<int, -1>
                    >
                {};
            };

            template<typename Tag>
            struct category_of_impl;

            template<>
            struct category_of_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                {
                    typedef random_access_traversal_tag type;
                };
            };

            template<typename Tag>
            struct size_impl;

            template<>
            struct size_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                  : Sequence::proto_size
                {};
            };

            template<typename Tag>
            struct begin_impl;

            template<>
            struct begin_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                {
                    typedef
                        proto::detail::expr_iterator<
                            Sequence
                          , 0
                        >
                    type;

                    static type call(Sequence &seq)
                    {
                        return type(seq);
                    }
                };
            };

            template<typename Tag>
            struct end_impl;

            template<>
            struct end_impl<proto::proto_expr>
            {
                template<typename Sequence>
                struct apply
                {
                    typedef
                        proto::detail::expr_iterator<
                            Sequence
                          , Sequence::proto_size::value
                        >
                    type;

                    static type call(Sequence &seq)
                    {
                        return type(seq);
                    }
                };
            };

            template<typename Tag>
            struct value_at_impl;

            template<>
            struct value_at_impl<proto::proto_expr>
            {
                template<typename Sequence, typename Index>
                struct apply
                  : proto::exprs::children_element<(std::size_t)Index::value, typename Sequence::proto_children_type>
                {};
            };

            template<typename Tag>
            struct at_impl;

            template<>
            struct at_impl<proto::proto_expr>
            {
                template<
                    typename Sequence
                  , typename Index
                  , std::size_t Arity = proto::arity_of<Sequence>::value
                >
                struct apply
                {
                    typedef
                        typename proto::result_of::child<
                            Sequence &
                          , (std::size_t)Index::value
                        >::type
                    type;

                    static type call(Sequence &seq)
                    {
                        return proto::child<(std::size_t)Index::value>(seq);
                    }
                };

                template<typename Sequence, typename Index>
                struct apply<Sequence, Index, 0>
                {
                    typedef
                        typename proto::result_of::value<
                            Sequence &
                        >::type
                    type;

                    static type call(Sequence &seq)
                    {
                        return proto::value(seq);
                    }
                };
            };

            template<typename Tag>
            struct is_segmented_impl;

            template<>
            struct is_segmented_impl<proto::proto_flat_view>
            {
                template<typename Iterator>
                struct apply
                  : std::true_type
                {};
            };

            template<typename Tag>
            struct segments_impl;

            template<>
            struct segments_impl<proto::proto_flat_view>
            {
                template<typename Sequence>
                struct apply
                {
                    typedef typename Sequence::expr_type::proto_tag_type proto_tag_type;

                    typedef
                        fusion::transform_view<
                            typename Sequence::expr_type
                          , proto::detail::as_element<proto_tag_type>
                        >
                    type;

                    static type call(Sequence &sequence)
                    {
                        return type(sequence.expr_, proto::detail::as_element<proto_tag_type>());
                    }
                };
            };

            template<>
            struct category_of_impl<proto::proto_flat_view>
            {
                template<typename Sequence>
                struct apply
                {
                    typedef forward_traversal_tag type;
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
        struct sequence_tag<proto::expr<ExprDesc, Domain>>
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template<typename ExprDesc, typename Domain>
        struct sequence_tag<proto::basic_expr<ExprDesc, Domain>>
        {
            typedef fusion::fusion_sequence_tag type;
        };
    }
}

#endif
