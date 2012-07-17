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

            template<typename ...List, typename T0>
            struct pop_back_<utility::list<List...>, T0>
            {
                typedef utility::list<List...> type;
            };

            template<typename ...List, typename T0, typename T1>
            struct pop_back_<utility::list<List...>, T0, T1>
            {
                typedef utility::list<List..., T0> type;
            };

            template<typename ...List, typename T0, typename T1, typename T2>
            struct pop_back_<utility::list<List...>, T0, T1, T2>
            {
                typedef utility::list<List..., T0, T1> type;
            };

            template<typename ...List, typename T0, typename T1, typename T2, typename... Tail>
            struct pop_back_<utility::list<List...>, T0, T1, T2, Tail...>
              : pop_back_<utility::list<List..., T0, T1, T2>, Tail...>
            {};
        }

        namespace utility
        {
            template<typename ...T>
            using pop_back = typename detail::pop_back_<list<>, T...>::type;
        }

        namespace detail
        {
            template<std::size_t I, typename T, typename List>
            struct list_of_;

            template<typename T, typename ...Ts>
            struct list_of_<0, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts...> type;
            };

            template<typename T, typename ...Ts>
            struct list_of_<1, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T> type;
            };

            template<typename T, typename ...Ts>
            struct list_of_<2, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T> type;
            };

            template<typename T, typename ...Ts>
            struct list_of_<3, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T, T> type;
            };

            template<typename T, typename ...Ts>
            struct list_of_<4, T, utility::list<Ts...>>
            {
                typedef utility::list<Ts..., T, T, T, T> type;
            };

            template<std::size_t I, typename T, typename ...Ts>
            struct list_of_<I, T, utility::list<Ts...>>
              : list_of_<I-5, T, utility::list<Ts..., T, T, T, T, T>>
            {};
        }

        namespace utility
        {
            template<int I, typename T>
            using list_of = typename detail::list_of_<I, T, list<>>::type;

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
                  : decltype(detail::get_nth_<list_of<N, void>>::eval(std::declval<Ts>()...))
                {};
            }

            template<std::size_t N, typename ...Ts>
            constexpr typename result_of::get_nth<N, Ts...>::type && get_nth(Ts &&... ts) noexcept
            {
                return detail::get_nth_<list_of<N, void>>::eval(static_cast<Ts &&>(ts)...).value;
            }

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
        }
    }
}

#endif
