////////////////////////////////////////////////////////////////////////////////////////////////////
// debug.hpp
// Utilities for debugging Proto expression trees
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DEBUG_HPP_INCLUDED
#define BOOST_PROTO_DEBUG_HPP_INCLUDED

#include <cstddef>
#include <iostream>
#include <functional>
#include <boost/preprocessor/stringize.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/expr.hpp>
#include <boost/proto/utility.hpp>
#include <boost/detail/sp_typeinfo.hpp>

namespace boost
{
    namespace proto
    {
        namespace tags
        {
        #define BOOST_PROTO_DEFINE_TAG_INSERTION(Tag)                                               \
            /** \brief INTERNAL ONLY */                                                             \
            inline std::ostream &operator <<(std::ostream &sout, Tag const &)                       \
            {                                                                                       \
                return sout << BOOST_PP_STRINGIZE(Tag);                                             \
            }                                                                                       \
            /**/

            BOOST_PROTO_DEFINE_TAG_INSERTION(terminal)
            BOOST_PROTO_DEFINE_TAG_INSERTION(unary_plus)
            BOOST_PROTO_DEFINE_TAG_INSERTION(negate)
            BOOST_PROTO_DEFINE_TAG_INSERTION(dereference)
            BOOST_PROTO_DEFINE_TAG_INSERTION(complement)
            BOOST_PROTO_DEFINE_TAG_INSERTION(address_of)
            BOOST_PROTO_DEFINE_TAG_INSERTION(logical_not)
            BOOST_PROTO_DEFINE_TAG_INSERTION(pre_inc)
            BOOST_PROTO_DEFINE_TAG_INSERTION(pre_dec)
            BOOST_PROTO_DEFINE_TAG_INSERTION(post_inc)
            BOOST_PROTO_DEFINE_TAG_INSERTION(post_dec)
            BOOST_PROTO_DEFINE_TAG_INSERTION(shift_left)
            BOOST_PROTO_DEFINE_TAG_INSERTION(shift_right)
            BOOST_PROTO_DEFINE_TAG_INSERTION(multiplies)
            BOOST_PROTO_DEFINE_TAG_INSERTION(divides)
            BOOST_PROTO_DEFINE_TAG_INSERTION(modulus)
            BOOST_PROTO_DEFINE_TAG_INSERTION(plus)
            BOOST_PROTO_DEFINE_TAG_INSERTION(minus)
            BOOST_PROTO_DEFINE_TAG_INSERTION(less)
            BOOST_PROTO_DEFINE_TAG_INSERTION(greater)
            BOOST_PROTO_DEFINE_TAG_INSERTION(less_equal)
            BOOST_PROTO_DEFINE_TAG_INSERTION(greater_equal)
            BOOST_PROTO_DEFINE_TAG_INSERTION(equal_to)
            BOOST_PROTO_DEFINE_TAG_INSERTION(not_equal_to)
            BOOST_PROTO_DEFINE_TAG_INSERTION(logical_or)
            BOOST_PROTO_DEFINE_TAG_INSERTION(logical_and)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_and)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_or)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_xor)
            BOOST_PROTO_DEFINE_TAG_INSERTION(comma)
            BOOST_PROTO_DEFINE_TAG_INSERTION(mem_ptr)
            BOOST_PROTO_DEFINE_TAG_INSERTION(assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(shift_left_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(shift_right_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(multiplies_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(divides_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(modulus_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(plus_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(minus_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_and_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_or_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_xor_assign)
            BOOST_PROTO_DEFINE_TAG_INSERTION(subscript)
            BOOST_PROTO_DEFINE_TAG_INSERTION(member)
            BOOST_PROTO_DEFINE_TAG_INSERTION(if_else_)
            BOOST_PROTO_DEFINE_TAG_INSERTION(function)

            #undef BOOST_PROTO_DEFINE_TAG_INSERTION
        }

        namespace hidden_detail_
        {
            struct ostream_wrapper
            {
                std::ostream &sout_;

                ostream_wrapper(std::ostream &sout)
                  : sout_(sout)
                {}
            };

            struct named_any
            {
                char const *name_;

                template<typename T>
                named_any(T const &)
                  : name_(BOOST_SP_TYPEID(T).name())
                {}
            };

            inline std::ostream &operator<<(ostream_wrapper sout_wrap, named_any t)
            {
                return sout_wrap.sout_ << t.name_;
            }
        }

        namespace detail
        {
            struct display_expr_
            {
                explicit display_expr_(std::ostream &sout, int depth = 0)
                  : depth_(depth)
                  , first_(true)
                  , sout_(sout)
                {}

                template<typename E, BOOST_PROTO_ENABLE_IF(is_terminal<E>::value)>
                void operator()(E const &e) const
                {
                    using namespace hidden_detail_;
                    using tag = typename tag_of<E>::type;
                    this->sout_.width(this->depth_);
                    this->sout_ << (this->first_ ? "" : ", ");
                    this->sout_ << tag() << "(" << proto::value(e) << ")\n";
                    this->first_ = false;
                }

                template<typename E, BOOST_PROTO_ENABLE_IF(!is_terminal<E>::value)>
                void operator()(E const &e) const
                {
                    using namespace hidden_detail_;
                    using tag = typename tag_of<E>::type;
                    this->sout_.width(this->depth_);
                    this->sout_ << (this->first_ ? "" : ", ");
                    this->sout_ << tag() << "(\n";
                    exprs::for_each(e.proto_args(), display_expr_(this->sout_, this->depth_ + 4));
                    this->sout_.width(this->depth_);
                    this->sout_ << "" << ")\n";
                    this->first_ = false;
                }

            private:
                int depth_;
                mutable bool first_;
                std::ostream &sout_;
            };
        }

        namespace functional
        {
            /// \brief Pretty-print a Proto expression tree.
            ///
            /// A PolymorphicFunctionObject which accepts a Proto expression
            /// tree and pretty-prints it to an \c ostream for debugging
            /// purposes.
            struct display_expr
            {
                /// \param sout  The \c ostream to which the expression tree
                ///              will be written.
                /// \param depth The starting indentation depth for this node.
                ///              Children nodes will be displayed at a starting
                ///              depth of <tt>depth+4</tt>.
                explicit display_expr(::std::ostream &sout = ::std::cout, int depth = 0)
                  : depth_(depth)
                  , sout_(sout)
                {}

                /// \brief Pretty-print the current node in a Proto expression
                /// tree.
                template<typename E>
                void operator()(E const &e) const
                {
                    detail::display_expr_(this->sout_, this->depth_)(e);
                }

            private:
                int depth_;
                ::std::reference_wrapper< ::std::ostream> sout_;
            };
        }

        /// \brief Pretty-print a Proto expression tree.
        ///
        /// \note Equivalent to <tt>functional::display_expr(0, sout)(static_cast<E &&>(e))</tt>
        /// \param expr The Proto expression tree to pretty-print
        /// \param sout The \c ostream to which the output should be
        ///             written. If not specified, defaults to
        ///             <tt>std::cout</tt>.
        template<typename E>
        void display_expr(E const &e, std::ostream &sout)
        {
            functional::display_expr(sout, 0)(e);
        }

        /// \overload
        ///
        template<typename E>
        void display_expr(E const &e)
        {
            functional::display_expr()(e);
        }

        /// \brief Assert at compile time that a particular expression
        ///        matches the specified grammar.
        ///
        /// \note Equivalent to <tt>BOOST_MPL_ASSERT((proto::matches\<E, Grammar\>))</tt>
        /// \param expr The Proto expression to check againts <tt>Grammar</tt>
        template<typename Grammar, typename E>
        void assert_matches(E const & /*expr*/)
        {
            static_assert(
                proto::matches<E, Grammar>::value
              , "The expression does not match the specified grammar"
            );
        }

        /// \brief Assert at compile time that a particular expression
        ///        does not match the specified grammar.
        ///
        /// \note Equivalent to <tt>BOOST_MPL_ASSERT_NOT((proto::matches\<E, Grammar\>))</tt>
        /// \param expr The Proto expression to check againts <tt>Grammar</tt>
        template<typename Grammar, typename E>
        void assert_matches_not(E const & /*expr*/)
        {
            static_assert(
                !proto::matches<E, Grammar>::value
              , "The expression matches the specified grammar"
            );
        }

        /// \brief Assert at compile time that a particular expression
        ///        matches the specified grammar.
        ///
        /// \note Equivalent to <tt>proto::assert_matches\<Grammar\>(E)</tt>
        /// \param E The Proto expression to check againts <tt>Grammar</tt>
        /// \param Grammar The grammar used to validate E.
        #define BOOST_PROTO_ASSERT_MATCHES(E, Grammar)                                              \
            (true ? (void)0 : boost::proto::assert_matches<Grammar>(E))

        /// \brief Assert at compile time that a particular expression
        ///        does not match the specified grammar.
        ///
        /// \note Equivalent to <tt>proto::assert_matches_not\<Grammar\>(E)</tt>
        /// \param E The Proto expression to check againts <tt>Grammar</tt>
        /// \param Grammar The grammar used to validate E.
        #define BOOST_PROTO_ASSERT_MATCHES_NOT(E, Grammar)                                          \
            (true ? (void)0 : boost::proto::assert_matches_not<Grammar>(E))
    }
}

#endif
