///////////////////////////////////////////////////////////////////////////////
// utility.hpp
// Generally useful stuff.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_UTILITY_HPP_INCLUDED
#define BOOST_PROTO_UTILITY_HPP_INCLUDED

#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // none
            struct none
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // find_first_sfinae_error
            template<typename ...Args>
            struct find_first_sfinae_error;

            template<typename Head, typename ...Tail>
            struct find_first_sfinae_error<Head, Tail...>
              : find_first_sfinae_error<Tail...>
            {};

            template<typename Sig, typename ...Tail>
            struct find_first_sfinae_error<utility::sfinae_error<Sig>, Tail...>
            {
                typedef utility::sfinae_error<Sig> type;
            };
        }

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // empty
            struct empty
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // never
            template<typename ...T>
            struct never
              : std::false_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // any
            struct any
            {
                any() = default;

                template<typename T>
                constexpr any(T const &) noexcept
                {}

                template<typename T>
                operator T &&() const noexcept;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // static_const
            template<typename T>
            struct static_const
            {
                static constexpr T value = T();
            };

            template<typename T>
            constexpr T static_const<T>::value;

            ////////////////////////////////////////////////////////////////////////////////////////
            // ignore
            template<typename ...T>
            inline void ignore(T &&...) noexcept
            {}
        }

        namespace detail
        {
            template<bool Head, typename ...List>
            struct or_
              : std::false_type
            {
                typedef none which;
            };

            template<typename Head, typename ...Tail>
            struct or_<true, Head, Tail...>
              : std::true_type
            {
                typedef Head which; // which predicate succeeded?
            };

            template<typename Head, typename Next, typename ...Tail>
            struct or_<false, Head, Next, Tail...>
              : or_<Next::value, Next, Tail...>
            {};
        }

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // or_ (like mpl::or_, but variadic)
            template<typename ...T>
            struct or_
              : std::false_type
            {
                typedef detail::none which;
            };

            template<typename Head, typename ...T>
            struct or_<Head, T...>
              : detail::or_<Head::value, Head, T...>
            {};
        }

        namespace detail
        {
            template<bool Head, typename ...List>
            struct and_
              : std::true_type
            {
                typedef none which;
            };

            template<typename Head, typename ...Tail>
            struct and_<false, Head, Tail...>
              : std::false_type
            {
                typedef Head which; // which predicate failed?
            };

            template<typename Head, typename Next, typename ...Tail>
            struct and_<true, Head, Next, Tail...>
              : and_<Next::value, Next, Tail...>
            {};
        }

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // and_  (like mpl::and_, but variadic)
            template<typename ...T>
            struct and_
              : std::true_type
            {
                typedef detail::none which;
            };

            template<typename Head, typename ...T>
            struct and_<Head, T...>
              : detail::and_<Head::value, Head, T...>
            {};
        }

        namespace utility
        {
            ///////////////////////////////////////////////////////////////////////////
            // indices
            template<std::size_t ...I>
            struct indices
            {};
        }

        namespace detail
        {
            template<std::size_t From, std::size_t To, typename Ints>
            struct make_indices_;

            template<std::size_t From, std::size_t To, std::size_t... I>
            struct make_indices_<From, To, utility::indices<I...>>
              : make_indices_<From, To-1, utility::indices<To-1, I...>>
            {};

            template<std::size_t N, std::size_t... I>
            struct make_indices_<N, N, utility::indices<I...>>
            {
                typedef utility::indices<I...> type;
            };
        }

        namespace utility
        {
            ///////////////////////////////////////////////////////////////////////////
            // make_indices
            template<std::size_t From, std::size_t To>
            using make_indices = typename detail::make_indices_<From, To, indices<>>::type;

            ////////////////////////////////////////////////////////////////////////////////////////
            // identity
            struct identity
            {
                template<typename T>
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    T(static_cast<T &&>(t))
                )
            };
        }

        namespace detail
        {
            template<typename T>
            inline auto constexpr by_val_(T &t, int)
                BOOST_PROTO_AUTO_RETURN(T(t))

            template<typename T>
            inline auto constexpr by_val_(T const &t, int)
                BOOST_PROTO_AUTO_RETURN(T(t))

            template<typename T>
            inline auto constexpr by_val_(T &&t, int)
                BOOST_PROTO_AUTO_RETURN(T(static_cast<T &&>(t)))

            template<typename T>
            inline auto constexpr by_val_(T &t, long)
                BOOST_PROTO_AUTO_RETURN(t)

            template<typename T>
            inline auto constexpr by_val_(T const &t, long)
                BOOST_PROTO_AUTO_RETURN(t)
        }

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // by_val
            struct by_val
            {
                template<typename T>
                inline constexpr auto operator()(T &&t) const
                BOOST_PROTO_AUTO_RETURN(
                    detail::by_val_(static_cast<T &&>(t), 1)
                )
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // by_ref
            struct by_ref
            {
                template<typename T>
                inline constexpr T const & operator()(T &&t) const noexcept
                {
                    return t;
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // logical_ops
            struct logical_ops
            {
                static inline constexpr bool and_() noexcept
                {
                    return true;
                }

                static inline constexpr bool and_(bool b0) noexcept
                {
                    return b0;
                }

                static inline constexpr bool and_(bool b0, bool b1) noexcept
                {
                    return b0 && b1;
                }

                static inline constexpr bool and_(bool b0, bool b1, bool b2) noexcept
                {
                    return b0 && b1 && b2;
                }

                template<typename ...Bool, typename Impl = logical_ops>
                static inline constexpr bool and_(bool b0, bool b1, bool b2, bool b3, Bool... rest) noexcept
                {
                    return b0 && b1 && b2 && b3 && Impl::and_(rest...);
                }

                static inline constexpr bool or_() noexcept
                {
                    return false;
                }

                static inline constexpr bool or_(bool b0) noexcept
                {
                    return b0;
                }

                static inline constexpr bool or_(bool b0, bool b1) noexcept
                {
                    return b0 || b1;
                }

                static inline constexpr bool or_(bool b0, bool b1, bool b2) noexcept
                {
                    return b0 || b1 || b2;
                }

                template<typename ...Bool, typename Impl = logical_ops>
                static inline constexpr bool or_(bool b0, bool b1, bool b2, bool b3, Bool... rest) noexcept
                {
                    return b0 || b1 || b2 || b3 || Impl::or_(rest...);
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // lazy_condition
            template<bool Cond, typename Fun0, typename Fun1>
            struct lazy_conditional
              : Fun0
            {};

            template<typename Fun0, typename Fun1>
            struct lazy_conditional<false, Fun0, Fun1>
              : Fun1
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // undef
            template<typename T>
            struct undef;

            ////////////////////////////////////////////////////////////////////////////////////////
            // list
            template<typename ...T>
            struct list;
        }

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // pop_back_
            template<typename List, typename ...T>
            struct pop_back_;

            template<template<typename...> class List, typename ...As, typename T0>
            struct pop_back_<List<As...>, T0>
            {
                typedef List<As...> type;
            };

            template<template<typename...> class List, typename ...As, typename T0, typename T1>
            struct pop_back_<List<As...>, T0, T1>
            {
                typedef List<As..., T0> type;
            };

            template<template<typename...> class List, typename ...As, typename T0, typename T1, typename T2>
            struct pop_back_<List<As...>, T0, T1, T2>
            {
                typedef List<As..., T0, T1> type;
            };

            template<template<typename...> class List, typename ...As, typename T0, typename T1, typename T2, typename... Tail>
            struct pop_back_<List<As...>, T0, T1, T2, Tail...>
              : pop_back_<List<As..., T0, T1, T2>, Tail...>
            {};

            template<typename Ret, typename ...As, typename T0>
            struct pop_back_<Ret(As...), T0>
            {
                typedef Ret type(As...);
            };

            template<typename Ret, typename ...As, typename T0, typename T1>
            struct pop_back_<Ret(As...), T0, T1>
            {
                typedef Ret type(As..., T0);
            };

            template<typename Ret, typename ...As, typename T0, typename T1, typename T2>
            struct pop_back_<Ret(As...), T0, T1, T2>
            {
                typedef Ret type(As..., T0, T1);
            };

            template<typename Ret, typename ...As, typename T0, typename T1, typename T2, typename... Tail>
            struct pop_back_<Ret(As...), T0, T1, T2, Tail...>
              : pop_back_<Ret(As..., T0, T1, T2), Tail...>
            {};
        }

        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // pop_back
            template<typename List>
            struct pop_back;
            
            template<template<typename...> class List, typename...As>
            struct pop_back<List<As...>>
              : detail::pop_back_<List<>, As...>
            {};
            
            template<typename Ret, typename...As>
            struct pop_back<Ret(As...)>
              : detail::pop_back_<Ret(), As...>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // concat
            template<typename List0, typename List1>
            struct concat;

            template<
                template<typename...> class T0, typename ...List
              , template<typename...> class T1, typename ...Rest
            >
            struct concat<T0<List...>, T1<Rest...>>
            {
                typedef T0<List..., Rest...> type;
            };

            template<typename Ret0, typename ...List, template<typename...> class T1, typename ...Rest>
            struct concat<Ret0(List...), T1<Rest...>>
            {
                typedef Ret0 type(List..., Rest...);
            };

            template<template<typename...> class T0, typename ...List, typename Ret1, typename ...Rest>
            struct concat<T0<List...>, Ret1(Rest...)>
            {
                typedef T0<List..., Rest...> type;
            };

            template<typename Ret0, typename ...List, typename Ret1, typename ...Rest>
            struct concat<Ret0(List...), Ret1(Rest...)>
            {
                typedef Ret0 type(List..., Rest...);
            };
        }

        namespace detail
        {
            template<std::size_t I, typename T, typename List>
            struct append_;

            template<typename T, typename ...Ts>
            struct append_<0, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts...> type;
            };

            template<typename T, typename ...Ts>
            struct append_<1, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T> type;
            };

            template<typename T, typename ...Ts>
            struct append_<2, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T> type;
            };

            template<typename T, typename ...Ts>
            struct append_<3, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T, T> type;
            };

            template<typename T, typename ...Ts>
            struct append_<4, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T, T, T> type;
            };

            template<std::size_t I, typename T, typename ...Ts>
            struct append_<I, T, utility::list<Ts...>>
              : append_<I-5, T, utility::list<Ts..., T, T, T, T, T>>
            {};
        }

        namespace utility
        {
            template<std::size_t I, typename T, typename List = list<>>
            struct list_of
              : detail::append_<I, T, List>
            {};

            template<std::size_t I, typename T, typename List>
            struct append
              : detail::append_<I, T, List>
            {};

            template<typename First, typename Second>
            using first = First;

            template<typename First, typename Second>
            using second = Second;

            template<typename T>
            struct rvalue_reference_wrapper
            {
                T && value;
                typedef T type;

                constexpr rvalue_reference_wrapper(T && t) noexcept
                  : value(static_cast<T &&>(t))
                {}

                T && get() const
                {
                    return static_cast<T &&>(value);
                }
            };
        }

        namespace detail
        {
            template<typename Ints>
            struct get_nth_;

            template<typename ...Vs>
            struct get_nth_<utility::list<Vs...>>
            {
                template<typename T, typename ...Us>
                static constexpr auto eval(utility::first<utility::any, Vs>..., T && t, Us &&...) noexcept
                    -> utility::rvalue_reference_wrapper<T>
                {
                    return static_cast<T &&>(t);
                }
            };
        }

        namespace utility
        {
            namespace result_of
            {
                template<std::size_t N, typename ...Ts>
                struct get_nth
                  : decltype(detail::get_nth_<typename list_of<N, void>::type>::eval(std::declval<Ts>()...))
                {};
            }

            template<std::size_t N, typename ...Ts>
            inline constexpr typename result_of::get_nth<N, Ts...>::type get_nth(Ts &&... ts)
            BOOST_PROTO_RETURN(
                detail::get_nth_<typename list_of<N, void>::type>::eval(static_cast<Ts &&>(ts)...).get()
            )

            namespace result_of
            {
                template<typename... Ts>
                struct back
                  : result_of::get_nth<sizeof...(Ts) - 1, Ts...>
                {};
            }

            template<typename... Ts>
            inline constexpr auto back(Ts &&... ts)
            BOOST_PROTO_AUTO_RETURN(
                utility::get_nth<sizeof...(Ts) - 1>(static_cast<Ts &&>(ts)...)
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // is_base_of
            template<typename T, typename U>
            struct is_base_of
              : std::is_base_of<T, U>
            {};

            template<typename T, typename U>
            struct is_base_of<T &, U>
              : std::is_base_of<T, U>
            {};

            template<typename T, typename U>
            struct is_base_of<T, U &>
              : std::is_base_of<T, U>
            {};

            template<typename T, typename U>
            struct is_base_of<T &, U &>
              : std::is_base_of<T, U>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // void_type
            struct void_type
            {
                template<typename T>
                friend T && operator,(T && t, void_type const &) noexcept
                {
                    return static_cast<T &&>(t);
                }

                template<typename T>
                friend T && operator,(void_type const &, T && t) noexcept
                {
                    return static_cast<T &&>(t);
                }

                friend void_type const &operator,(void_type const &, void_type const &) noexcept
                {
                    return utility::static_const<void_type>::value;
                }
            };

            namespace
            {
                constexpr void_type const &void_ = utility::static_const<void_type>::value;
            }

            BOOST_PROTO_IGNORE_UNUSED(void_);

            ////////////////////////////////////////////////////////////////////////////////////////
            // sized_type
            template<int N>
            struct sized_type
            {
                typedef char (&type)[N];
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // sfinae_error
            struct sfinae_error_base
            {
                template<typename ...Args>
                friend typename detail::find_first_sfinae_error<Args...>::type
                boost_proto_try_find_errors(int, Args &&...args) noexcept
                {
                    return typename detail::find_first_sfinae_error<Args...>::type();
                }
            };

            template<typename Fun, typename ...Args>
            struct sfinae_error<Fun(Args...)>
              : sfinae_error_base
            {
                virtual void what() const noexcept
                {
                    typedef decltype(std::declval<Fun>()(std::declval<Args>()...)) error_message;
                }
            };

            template<typename ...Args>
            inline int boost_proto_try_find_errors(long, Args &&...) noexcept
            {
                return 0;
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // Many thanks to Paul Fultz for some of the ideas behind try_call
            template<typename Fun>
            class try_call_wrapper
            {
                Fun fun_;

                template<typename ...Args>
                auto call_or_fail_(int, Args &&... args) const
                BOOST_PROTO_AUTO_RETURN(
                    fun_(static_cast<Args &&>(args)...)
                )

                template<typename Sig, typename ...Args>
                sfinae_error<Sig> call_or_fail_(sfinae_error<Sig>, Args &&... args) const noexcept
                {
                    return sfinae_error<Sig>();
                }

            public:
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(try_call_wrapper);

                explicit constexpr try_call_wrapper(Fun &&fun)
                    noexcept(noexcept(Fun(static_cast<Fun &&>(fun))))
                  : fun_(static_cast<Fun &&>(fun))
                {}

                template<typename ...Args>
                auto operator()(Args &&...args) const
                BOOST_PROTO_AUTO_RETURN(
                    this->call_or_fail_(
                        // Must be an unqualified call to possibly find the sfinae_error friend function
                        boost_proto_try_find_errors(1, static_cast<Args &&>(args)...)
                      , static_cast<Args &&>(args)...
                    )
                )

                template<typename ...Args>
                sfinae_error<Fun(Args...)> operator()(Args &&...) const volatile noexcept
                {
                    // Uncomment this line to get the full template instantiation backtrace
                    //const_cast<try_call_wrapper const *>(this)->fun_(static_cast<Args &&>(args)...);
                    return sfinae_error<Fun(Args...)>();
                }
            };

            template<typename Fun>
            inline constexpr auto try_call(Fun &&fun)
            BOOST_PROTO_AUTO_RETURN(
                try_call_wrapper<Fun>(static_cast<Fun &&>(fun))
            )

        #if 1
            #define BOOST_PROTO_TRY_CALL(...) boost::proto::utility::try_call(__VA_ARGS__)
        #else
            #define BOOST_PROTO_TRY_CALL(...) __VA_ARGS__
        #endif
        }
    }
}

#endif
