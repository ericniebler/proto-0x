////////////////////////////////////////////////////////////////////////////////////////////////////
// pass.hpp
// Rebuild an expression where each child has been transformed according
// to the corresponding action.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_CXX11_ACTION_pass_HPP_INCLUDED
#define BOOST_PROTO_CXX11_ACTION_pass_HPP_INCLUDED

#include <utility>
#include <type_traits>
#include <boost/proto/v5/proto_fwd.hpp>
#include <boost/proto/v5/children.hpp>
#include <boost/proto/v5/make_expr.hpp>
#include <boost/proto/v5/utility.hpp>
#include <boost/proto/v5/action/basic_action.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace v5
        {
            namespace detail
            {
                template<typename Actions>
                struct _pass_;

                template<typename Action, typename Enable = void>
                struct as_pass_action_
                  : as_action_<Action>
                {};

                template<typename Tag, typename ...Grammars>
                struct as_pass_action_<Tag(*)(Grammars...), typename std::enable_if<proto::v5::is_tag<Tag>::value>::type>
                  : _pass_<Tag(Grammars...)>
                {};

                template<typename Tag, typename ...Grammars>
                struct as_pass_action_<Tag(*)(Grammars......), typename std::enable_if<proto::v5::is_tag<Tag>::value>::type>
                  : _pass_<Tag(Grammars......)>
                {};

                template<typename Indices, typename Pattern>
                struct pass_0_;

                template<std::size_t ...I, typename Tag, typename ...Actions>
                struct pass_0_<utility::indices<I...>, Tag(Actions...)>
                {
                    static_assert(
                        sizeof...(I) == sizeof...(Actions)
                      , "wrong number of arguments to pass-through basic_action"
                    );

                    template<typename E, typename ...Rest>
                    auto operator()(E && e, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        typename result_of::domain_of<E>::type::make_expr()(
                            proto::v5::tag_of(static_cast<E &&>(e))
                          , utility::by_val()(
                                proto::v5::as_expr<typename result_of::domain_of<E>::type>(
                                    as_pass_action_<Actions>()(
                                        proto::v5::child<I>(static_cast<E &&>(e))
                                      , static_cast<Rest &&>(rest)...
                                    )
                                )
                            )...
                        )
                    )
                };

                template<std::size_t ...I, typename Tag, typename ...Actions>
                struct pass_0_<utility::indices<I...>, Tag(Actions......)>
                {
                    static_assert(
                        sizeof...(I) + 1 >= sizeof...(Actions)
                      , "wrong number of arguments to pass-through basic_action"
                    );

                    template<typename ...Args>
                    auto operator()(Args &&... args) const
                    BOOST_PROTO_AUTO_RETURN(
                        pass_0_<
                            utility::indices<I...>
                          , typename utility::concat<
                                typename utility::pop_back<Tag(Actions...)>::type
                              , typename utility::list_of<
                                    sizeof...(I) + 1 - sizeof...(Actions)
                                  , typename utility::result_of::back<Actions...>::type
                                >::type
                            >::type
                        >()(static_cast<Args &&>(args)...)
                    )
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // _pass_
                template<typename Actions>
                struct _pass_
                  : basic_action<_pass_<Actions>>
                {
                    template<typename E, typename ...Rest, BOOST_PROTO_ENABLE_IF(is_terminal<E>::value)>
                    auto operator()(E && e, Rest &&...) const
                    BOOST_PROTO_AUTO_RETURN(
                        utility::by_val()(static_cast<E &&>(e))
                    )

                    template<typename E, typename ...Rest, BOOST_PROTO_ENABLE_IF(!is_terminal<E>::value)>
                    auto operator()(E && e, Rest &&... rest) const
                    BOOST_PROTO_AUTO_RETURN(
                        detail::pass_0_<
                            utility::make_indices<result_of::arity_of<E>::value>
                          , Actions
                        >()(static_cast<E &&>(e), static_cast<Rest &&>(rest)...)
                    )
                };
            }

            struct pass
            {};

            namespace extension
            {
                template<typename ...Actions>
                struct action_impl<pass(Actions...)>
                  : detail::_pass_<pass(Actions...)>
                {};

                template<typename ...Actions>
                struct action_impl<pass(Actions......)>
                  : detail::_pass_<pass(Actions......)>
                {};

                template<typename Tag, typename ...ActiveGrammars>
                struct action_impl<case_(Tag(ActiveGrammars...), pass)>
                  : detail::_pass_<pass(ActiveGrammars...)>
                {};

                template<typename Tag, typename ...ActiveGrammars>
                struct action_impl<case_(Tag(ActiveGrammars......), pass)>
                  : detail::_pass_<pass(ActiveGrammars......)>
                {};
            }
        }
    }
}

#endif
