////////////////////////////////////////////////////////////////////////////////////////////////////
// utility.hpp
// Generally useful stuff.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Acknowledgements:
//  Richard Smith for the ideas behind get_nth and back
//  Dave Abrahams for some additional compile-time perf tweaks to get_nth and back

#ifndef BOOST_PROTO_UTILITY_HPP_INCLUDED
#define BOOST_PROTO_UTILITY_HPP_INCLUDED

#include <cstddef>
#include <functional>
#include <type_traits>
#include <boost/utility/addressof.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace boost
{
    namespace proto
    {
        namespace utility
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // never
            template<typename ...T>
            struct never
              : std::false_type
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // static_const
            template<typename T>
            struct static_const
            {
                static constexpr T value{};
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
            ////////////////////////////////////////////////////////////////////////////////////////
            // none
            struct none
            {};

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

            ///////////////////////////////////////////////////////////////////////////
            // list
            template<typename...>
            struct list;
        }

        namespace detail
        {
            // Wrapper to prevent type decay
            template<typename T>
            struct no_decay
            {
                typedef T type;
            };

            template<typename Ignored>
            struct any
            {
                any() = default;

                template<typename T>
                constexpr any(T const &) noexcept
                {}

                template<typename T>
                operator T &&() const noexcept;
            };

            template<typename Ignored = decltype(nullptr)>
            struct any_pod
            {
                any_pod(...);
            };

            // Glue two sets of indices together
            template<typename I1, typename I2>
            struct append_indices;

            template<std::size_t...N1, std::size_t...N2>
            struct append_indices<utility::indices<N1...>, utility::indices<N2...>>
            {
                typedef utility::indices<N1..., (sizeof...(N1) + N2)...> type;
            };

            // generate indices [0,N) in O(log(N)) time
            template<std::size_t N>
            struct make_indices
              : append_indices<
                    typename make_indices<N / 2>::type
                  , typename make_indices<N - N / 2>::type
                >
            {};

            template<>
            struct make_indices<0>
            {
                typedef utility::indices<> type;
            };

            template<>
            struct make_indices<1>
            {
                typedef utility::indices<0> type;
            };
        }

        namespace utility
        {
            ///////////////////////////////////////////////////////////////////////////
            // make_indices
            template<std::size_t N>
            using make_indices = typename detail::make_indices<N>::type;

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

            // Generate lists<_,_,_,..._> with N arguments in O(log N)
            template<std::size_t N, typename T, typename List = list<>>
            struct list_of
              : concat<
                    typename list_of<N / 2, T, List>::type
                  , typename list_of<N - N / 2, T, List>::type
                >
            {};

            template<typename T, typename List>
            struct list_of<0, T, List>
            {
                typedef List type;
            };

            template<typename T, template<typename...> class List>
            struct list_of<1, T, List<>>
            {
                typedef List<T> type;
            };

            template<typename T, typename R>
            struct list_of<1, T, R()>
            {
                typedef R type(T);
            };

            template<std::size_t I, typename T, typename List>
            struct append
              : concat<
                    List
                  , typename list_of<I, T>::type
                >
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

                auto get() const
                    BOOST_PROTO_AUTO_RETURN(T(static_cast<T &&>(value)))
            };
        }

        namespace detail
        {
            template<typename Ignored>
            struct get_nth_type;

            template<typename ...Ignored>
            struct get_nth_type<utility::list<Ignored...>>
            {
                template<typename T, typename ...Us>
                static constexpr T eval(any_pod<Ignored>..., T *, Us *...) noexcept;
            };

            template<typename Ignored>
            struct get_nth_value;

            template<typename ...Ignored>
            struct get_nth_value<utility::list<Ignored...>>
            {
                template<typename T, typename ...Us>
                static constexpr auto eval(detail::any<Ignored>..., T && t, Us &&...) noexcept
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
                using get_nth =
                   decltype(
                        detail::get_nth_type<typename list_of<N, decltype(nullptr)>::type>::eval(
                            (detail::no_decay<Ts>*)nullptr ...));
            }

            namespace functional
            {
                template<std::size_t N>
                struct get_nth
                {
                    template<typename ...Ts>
                    inline constexpr auto operator()(Ts &&... ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        detail::get_nth_value<typename list_of<N, decltype(nullptr)>::type>::eval(
                            static_cast<Ts &&>(ts)...).get()
                    )
                };
            }

            namespace result_of
            {
                template<typename... Ts>
                struct back
                  : result_of::get_nth<sizeof...(Ts) - 1, Ts...>
                {};
            }

            namespace functional
            {
                struct back
                {
                    template<typename... Ts>
                    inline constexpr auto operator()(Ts &&... ts) const
                    BOOST_PROTO_AUTO_RETURN(
                        utility::functional::get_nth<sizeof...(Ts) - 1>()(static_cast<Ts &&>(ts)...)
                    )
                };
            }

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
            // substitution_failure
            struct substitution_failure_base
            {
            private:
                template<typename Sig, typename ...Args>
                static auto find_first_failure(substitution_failure<Sig> const &, Args const &...)
                    -> substitution_failure<Sig>;

                template<typename Head, typename ...Args, typename Impl = substitution_failure_base>
                static auto find_first_failure(Head const &, Args const &...args)
                    -> decltype(Impl::find_first_failure(args...));

            public:
                template<typename ...Args, typename Impl = substitution_failure_base>
                friend inline auto
                boost_proto_try_find_substitution_failure(int, Args const &...args) noexcept
                    -> decltype(substitution_failure_base::find_first_failure(args...))
                {
                    return decltype(substitution_failure_base::find_first_failure(args...))();
                }
            };

            template<typename Fun, typename ...Args>
            struct substitution_failure<Fun(Args...)> final
              : substitution_failure_base
            {
                #if 0
                // The following is not needed in clang because the "virtual" what method has
                // the desired effect. Other compilers, however, might benefit from the following,
                // in which case, the error will be reported iff a substitution_failure gets
                // assigned to an auto variable.
                substitution_failure() = default;
                substitution_failure(substitution_failure &&) = default;
                substitution_failure(substitution_failure const &that)
                {
                    that.what();
                }
                substitution_failure &operator=(substitution_failure &&) = delete;
                substitution_failure &operator=(substitution_failure const &) = delete;
                #endif

                virtual void what() const noexcept
                {
                    typedef
                        decltype(std::declval<Fun>()(std::declval<Args>()...))
                    error_message;
                }
            };

            template<typename ...Args>
            inline int boost_proto_try_find_substitution_failure(long, Args const &...) noexcept
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
                auto call_or_fail_(substitution_failure<Sig> const &, Args &&...) const noexcept
                    -> substitution_failure<Sig>
                {
                    return substitution_failure<Sig>();
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
                        // Must be an unqualified call to possibly find the substitution_failure friend function
                        boost_proto_try_find_substitution_failure(1, args...)
                      , static_cast<Args &&>(args)...
                    )
                )

                template<typename ...Args>
                auto operator()(Args &&...) const volatile noexcept
                    -> substitution_failure<Fun(Args...)>
                {
                    // Uncomment this line to get the full template instantiation backtrace
                    //const_cast<try_call_wrapper const *>(this)->fun_(static_cast<Args &&>(args)...);
                    return substitution_failure<Fun(Args...)>();
                }
            };

            template<typename Fun>
            inline constexpr auto try_call(Fun &&fun)
            BOOST_PROTO_AUTO_RETURN(
                try_call_wrapper<Fun>(static_cast<Fun &&>(fun))
            )

        #if 1
            #define BOOST_PROTO_TRY_CALL boost::proto::utility::try_call
        #else
            #define BOOST_PROTO_TRY_CALL
        #endif
        }

        namespace detail
        {
            ////////////////////////////////////////////////////////////////////////////////////////
            // proto_get_pointer
            template<typename T>
            auto proto_get_pointer(T & t, long)
            BOOST_PROTO_AUTO_RETURN(
                boost::addressof(t) // TODO replace with std::addressof
            )

            template<typename T>
            auto proto_get_pointer(T && t, int)
            BOOST_PROTO_AUTO_RETURN(
                get_pointer(static_cast<T &&>(t))
            )

            ////////////////////////////////////////////////////////////////////////////////////////
            // mem_fun_binder_
            template<typename T, typename PMF>
            class mem_fun_binder_
            {
                T obj_;
                PMF pmf_;

            public:
                BOOST_PROTO_REGULAR_TRIVIAL_CLASS(mem_fun_binder_);

                template<typename U>
                mem_fun_binder_(U && u, PMF pmf) noexcept(noexcept(T(static_cast<U &&>(u))))
                  : obj_(static_cast<U &&>(u))
                  , pmf_(pmf)
                {}

                template<typename ...A>
                auto operator()(A &&... a) const
                BOOST_PROTO_AUTO_RETURN(
                    (detail::proto_get_pointer(obj_, 1) ->* pmf_)(static_cast<A &&>(a)...)
                )
            };
        }
    }
}

#endif
