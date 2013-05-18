////////////////////////////////////////////////////////////////////////////////////////////////////
// debug.hpp
// Utilities for debugging Proto expression trees
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V5_DEBUG_HPP_INCLUDED
#define BOOST_PROTO_V5_DEBUG_HPP_INCLUDED

#include <cstddef>
#include <memory>
#include <string>
#include <iomanip>
#include <iostream>
#include <functional>
#include <boost/preprocessor/stringize.hpp>
#include <boost/type.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/def.hpp>
#include <boost/proto/v5/expr.hpp>
#include <boost/proto/v5/tags.hpp>
#include <boost/proto/v5/fusion.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/detail/access.hpp>
#include <boost/proto/v5/action/basic_action.hpp>
#include <boost/proto/v5/action/env.hpp>
#include <boost/proto/v5/action/if.hpp>
#include <boost/proto/v5/action/integral_constant.hpp>
#include <boost/proto/v5/action/make.hpp>
#include <boost/proto/v5/action/call.hpp>
#include <boost/detail/sp_typeinfo.hpp>

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#define BOOST_PROTO_USE_DEMANGLING
#include <cxxabi.h>
#endif

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace tags
            {
            #define BOOST_PROTO_DEFINE_TAG_INSERTION(Tag)                                           \
                /** \brief INTERNAL ONLY */                                                         \
                inline std::ostream &operator <<(std::ostream &sout, Tag const &)                   \
                {                                                                                   \
                    return sout << BOOST_PP_STRINGIZE(Tag);                                         \
                }                                                                                   \
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

            namespace detail
            {
                inline std::string tidy(std::string name)
                {
                    const std::string from("boost::proto::v5::");
                    const std::string to("proto::");
                    for(std::size_t p = name.find(from);
                        p != std::string::npos;
                        p = name.find(from, p + to.size()))
                    {
                        name.replace(p, from.size(), to);
                    }
                    return name;
                }

                template<typename T>
                std::string name_of()
                {
                    return detail::tidy(name_of_<T>()());
                }

                struct concat
                {
                    explicit concat(char const *delim = "")
                      : delim_(delim)
                    {}

                    std::string operator()() const
                    {
                        return std::string();
                    }

                    std::string operator()(std::string head) const
                    {
                        return head;
                    }

                    template<typename ...Strings>
                    std::string operator()(std::string head, Strings... tail) const
                    {
                        return head + delim_ + (*this)(tail...);
                    }

                    char const *delim_;
                };

                template<template<typename...> class T>
                struct template_name_
                {
                    static constexpr char const* value = 0;
                };

                template<typename T>
                std::string name_of_impl_()
                {
                #ifdef BOOST_PROTO_USE_DEMANGLING
                    struct free_t { void operator()(void *p) { free(p); } };
                    int status = 0;
                    std::unique_ptr<char, free_t> realname(
                        abi::__cxa_demangle(BOOST_SP_TYPEID(T).name(), 0, 0, &status));
                    if(0 == status)
                        return realname.get();
                #endif
                    return BOOST_SP_TYPEID(T).name();
                }

                template<typename T>
                struct name_of_
                {
                    std::string operator()() const
                    {
                        return detail::name_of_impl_<T>();
                    }
                };

                template<typename Ret, typename ...Args>
                struct name_of_<Ret(Args...)>
                {
                    std::string operator()() const
                    {
                        return name_of<Ret>() + "(" + concat(",")(name_of<Args>()...) + ")";
                    }
                };

                template<typename Ret, typename ...Args>
                struct name_of_<Ret(*)(Args...)>
                  : name_of_<Ret(Args...)>
                {};

                template<typename Ret, typename ...Args>
                struct name_of_<Ret(Args......)>
                {
                    std::string operator()() const
                    {
                        return name_of<Ret>() + "(" + concat(",")(name_of<Args>()...) + "...)";
                    }
                };

                template<typename Ret, typename ...Args>
                struct name_of_<Ret(*)(Args......)>
                  : name_of_<Ret(Args......)>
                {};

                template<template<typename...> class T, typename ...Args>
                struct name_of_<T<Args...>>
                {
                    std::string operator()() const
                    {
                        if(template_name_<T>::value)
                            return std::string(template_name_<T>::value) + "<" +
                                concat(",")(name_of<Args>()...) + ">";
                        else
                            return detail::tidy(detail::name_of_impl_<T<Args...>>());
                    }
                };
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
                    std::string name_;

                    template<typename T>
                    named_any(T const &)
                      : name_(detail::name_of<T>())
                    {}
                };

                inline std::ostream &operator<<(ostream_wrapper sout_wrap, named_any t)
                {
                    return sout_wrap.sout_ << t.name_;
                }

                template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<T>::value)>
                std::ostream &maybe_output_expr(std::ostream &sout, T const &t, int depth)
                {
                    return sout << std::setw(depth) << "" << t;
                }
            }

            namespace detail
            {
                template<typename T>
                struct cvref_name
                {
                    static char const* const value;
                };

                template<typename T>
                char const *const cvref_name<T>::value = "";

                template<typename T>
                struct cvref_name<T &>
                {
                    static char const* const value;
                };

                template<typename T>
                char const *const cvref_name<T &>::value = " &";

                template<typename T>
                struct cvref_name<T const &>
                {
                    static char const* const value;
                };

                template<typename T>
                char const *const cvref_name<T const &>::value = " const &";

                template<typename T>
                struct cvref_name<T &&>
                {
                    static char const* const value;
                };

                template<typename T>
                char const *const cvref_name<T &&>::value = " &&";

                // for use with mpl::for_each to fill in an array of cvref names
                struct cvref_names
                {
                    explicit cvref_names(char const **&names)
                      : names_(names)
                    {}

                    template<typename T>
                    void operator()(boost::type<T>) const
                    {
                        *this->names_++ = cvref_name<T>::value;
                    }

                private:
                    char const **&names_;
                };

                template<typename Expr>
                struct expr_wrap
                  : std::remove_reference<Expr>::type
                {
                    using tag = fusion::fusion_sequence_tag;
                };

                struct display_expr_
                {
                    display_expr_(std::ostream &sout, int depth, char const **names, ::std::size_t &index)
                      : depth_(depth)
                      , names_(names)
                      , index_(index)
                      , sout_(sout)
                    {}

                    template<typename E, BOOST_PROTO_ENABLE_IF(is_terminal<E>::value)>
                    void operator()(E &&e) const
                    {
                        using namespace hidden_detail_;
                        using tag_t = typename result_of::tag_of<E>::type;
                        using value_t = typename mpl::at_c<expr_wrap<E>, 0>::type;
                        this->sout_ << std::setw(this->depth_)
                                    << (0 == this->index_ ? "" : ", ")
                                    << tag_t()
                                    << "( "
                                    << "("
                                    << detail::name_of<utility::uncvref<value_t>>()
                                    << detail::cvref_name<value_t>::value
                                    << ") "
                                    << proto::v5::value(static_cast<E &&>(e))
                                    << " "
                                    << ")"
                                    << this->names_[this->index_]
                                    << "\n";
                        ++this->index_;
                    }

                    template<typename E, BOOST_PROTO_ENABLE_IF(!is_terminal<E>::value)>
                    void operator()(E &&e) const
                    {
                        using namespace hidden_detail_;
                        using tag_t = typename result_of::tag_of<E>::type;
                        ::std::size_t index = 0;
                        char const *names[mpl::size<expr_wrap<E>>::value] = {};
                        char const **names_ptr = names;
                        mpl::for_each<expr_wrap<E>, mpl::quote1<boost::type>>(detail::cvref_names(names_ptr));
                        this->sout_ << std::setw(this->depth_)
                                    << (0 == this->index_ ? "" : ", ")
                                    << tag_t()
                                    << "(\n";
                        exprs::for_each(
                            exprs::access::proto_args(static_cast<E &&>(e))
                          , display_expr_(this->sout_, this->depth_ + 4, names, index)
                        );
                        this->sout_ << std::setw(this->depth_)
                                    << ""
                                    << ")"
                                    << this->names_[this->index_]
                                    << "\n";
                        ++this->index_;
                    }

                private:
                    int depth_;
                    char const **names_;
                    ::std::size_t &index_;
                    std::ostream &sout_;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _name_of_
                template<typename T>
                struct _name_of_
                  : basic_action<_name_of_<T>>
                {
                    template<typename...As>
                    std::string operator()(As &&...) const
                    {
                        return detail::name_of<T>();
                    }
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // depth_tag: For use in the trace action to handle indenting levels
                struct depth_tag
                  : env_var_tag<depth_tag>
                {
                    using env_var_tag<depth_tag>::operator=;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _get_depth_
                // Fetch the current depth from the environment
                struct _get_depth_
                  : def<get_env(depth_tag())>
                {};

                struct _push_depth_if
                  : def<if_(
                        has_env(depth_tag())
                      , set_env(depth_tag(), plus(_get_depth_, _int<4>))
                      , push_env(depth_tag(), _int<0>)
                    )>
                {};

                template<typename Action, typename Name = _name_of_<Action>>
                struct _trace_
                  : basic_action<_trace_<Action, Name>>
                {
                    template<typename Expr, typename Env, typename ...Rest
                      , typename DisplayExpr = functional::display_expr>
                    auto operator()(Expr && e, Env &&env, Rest &&... rest) const
                     -> decltype(call_action_<Action>()(
                            static_cast<Expr &&>(e)
                          , std::declval<decltype(_push_depth_if()(
                                static_cast<Expr &&>(e)
                              , static_cast<Env &&>(env)
                            )) &>()
                          , static_cast<Rest &&>(rest)...
                        ))
                    {
                        auto new_env =
                            _push_depth_if()(
                                static_cast<Expr &&>(e)
                              , static_cast<Env &&>(env)
                            );
                        using result_t =
                            decltype(call_action_<Action>()(
                                static_cast<Expr &&>(e)
                              , new_env
                              , static_cast<Rest &&>(rest)...
                            ));
                        int depth = _get_depth_()(e, new_env);
                        std::cout << std::setw(depth) << ""
                                  << "TRACE CALL: "
                                  << call_action_<Name>()(
                                          static_cast<Expr &&>(e)
                                        , new_env
                                        , static_cast<Rest &&>(rest)...
                                      )
                                  << "\n";
                        DisplayExpr{depth}(e);
                        result_t res = call_action_<Action>()(
                            static_cast<Expr &&>(e)
                          , new_env
                          , static_cast<Rest &&>(rest)...
                        );
                        std::cout << std::setw(depth) << ""
                                  << "TRACE RETURN:\n";
                        maybe_output_expr(std::cout, res, depth);
                        std::cout << std::setw(depth) << "" << "\n";
                        return static_cast<result_t>(res);
                    }

                    template<typename Expr>
                    auto operator()(Expr && e) const
                    BOOST_PROTO_AUTO_RETURN(
                        this->operator()(static_cast<Expr &&>(e), empty_env())
                    )
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
                    explicit display_expr(int depth)
                      : depth_(depth)
                      , sout_(::std::cout)
                    {}

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
                    void operator()(E &&e) const
                    {
                        ::std::size_t index = 0;
                        char const *names[1] = {""};
                        detail::display_expr_ disp(this->sout_, this->depth_, names, index);
                        disp(static_cast<E &&>(e));
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
            /// \param depth The number of spaces to indent the expression.
            template<typename E>
            void display_expr(E &&e, std::ostream &sout, int depth = 0)
            {
                functional::display_expr(sout, depth)(static_cast<E &&>(e));
            }

            /// \overload
            ///
            template<typename E>
            void display_expr(E &&e, int depth = 0)
            {
                functional::display_expr(::std::cout, depth)(static_cast<E &&>(e));
            }

            /// \brief Iostream inserter object for Proto expressions.
            ///
            template<typename Expr>
            struct pretty_expr_wrap
            {
                explicit pretty_expr_wrap(Expr &&expr, int depth = 0)
                  : expr_(static_cast<Expr &&>(expr))
                  , depth_(depth)
                {}

                friend std::ostream &operator <<(std::ostream &sout, pretty_expr_wrap const &e)
                {
                    proto::display_expr(e.expr_, sout, e.depth_);
                    return sout;
                }
            private:
                Expr expr_;
                int depth_;
            };

            /// \brief Iostream inserter for Proto expressions.
            ///
            template<typename Expr>
            pretty_expr_wrap<Expr> pretty_expr(Expr &&expr, int depth = 0)
            {
                return pretty_expr_wrap<Expr>(static_cast<Expr &&>(expr), depth);
            }

            /// \brief Assert at compile time that a particular expression
            ///        matches the specified grammar.
            ///
            /// \note Equivalent to <tt>BOOST_MPL_ASSERT((proto::matches\<E, Grammar\>))</tt>
            /// \param expr The Proto expression to check againts <tt>Grammar</tt>
            template<typename Grammar, typename Expr>
            void assert_matches(Expr const &)
            {
                static_assert(
                    proto::v5::matches<Expr, Grammar>()
                  , "The expression does not match the specified grammar"
                );
            }

            /// \brief Assert at compile time that a particular expression
            ///        does not match the specified grammar.
            ///
            /// \note Equivalent to <tt>BOOST_MPL_ASSERT_NOT((proto::matches\<E, Grammar\>))</tt>
            /// \param expr The Proto expression to check againts <tt>Grammar</tt>
            template<typename Grammar, typename Expr>
            void assert_matches_not(Expr const &)
            {
                static_assert(
                    !proto::v5::matches<Expr, Grammar>()
                  , "The expression matches the specified grammar"
                );
            }

            struct trace
            {};

            namespace extension
            {
                template<typename Action>
                struct action_impl<trace(Action)>
                  : detail::_trace_<Action>
                {};

                template<typename Action, typename Name>
                struct action_impl<trace(Action, Name)>
                  : detail::_trace_<Action, Name>
                {};
            }

            namespace exprs
            {
                template<typename Expr, BOOST_PROTO_ENABLE_IF(is_expr<Expr>::value)>
                std::ostream &maybe_output_expr(std::ostream &sout, Expr const &e, int depth)
                {
                    return sout << proto::pretty_expr(e, depth);
                }
            }

            /// \brief Assert at compile time that a particular expression
            ///        matches the specified grammar.
            ///
            /// \note Equivalent to <tt>proto::assert_matches\<Grammar\>(E)</tt>
            /// \param E The Proto expression to check againts <tt>Grammar</tt>
            /// \param Grammar The grammar used to validate E.
            #define BOOST_PROTO_ASSERT_MATCHES(E, Grammar)                                          \
                (true ? (void)0 : boost::proto::v5::assert_matches<Grammar>(E))

            /// \brief Assert at compile time that a particular expression
            ///        does not match the specified grammar.
            ///
            /// \note Equivalent to <tt>proto::assert_matches_not\<Grammar\>(E)</tt>
            /// \param E The Proto expression to check againts <tt>Grammar</tt>
            /// \param Grammar The grammar used to validate E.
            #define BOOST_PROTO_ASSERT_MATCHES_NOT(E, Grammar)                                      \
                (true ? (void)0 : boost::proto::v5::assert_matches_not<Grammar>(E))
        }
    }
}

#endif
