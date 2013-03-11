////////////////////////////////////////////////////////////////////////////////////////////////////
// deep_copy.hpp
// Replace all nodes stored by reference with nodes stored by value.
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DEEP_COPY_HPP_INCLUDED
#define BOOST_PROTO_DEEP_COPY_HPP_INCLUDED

#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/matches.hpp>
#include <boost/proto/cxx11/make_expr.hpp>
#include <boost/proto/cxx11/action/basic_action.hpp>
#include <boost/proto/cxx11/action/switch.hpp>
#include <boost/proto/cxx11/action/pass.hpp>

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace detail
            {
                struct _deep_copy_cases
                {
                    template<typename Tag, bool IsTerminal = Tag::proto_is_terminal_type::value>
                    struct case_
                      : as_action_<proto::cxx11::pass(proto::cxx11::_deep_copy...)>
                    {};

                    template<typename Tag>
                    struct case_<Tag, true>
                      : proto::cxx11::basic_action<case_<Tag, true>>
                    {
                        template<typename E, typename... Rest>
                        auto operator()(E && e, Rest &&...) const
                        BOOST_PROTO_AUTO_RETURN(
                            typename decltype(e.proto_domain())::make_expr()(
                                proto::cxx11::tag_of(static_cast<E &&>(e))
                              , utility::by_val()(proto::cxx11::value(static_cast<E &&>(e)))
                            )
                        )
                    };
                };
            }

            ////////////////////////////////////////////////////////////////////////////////////////
            // _deep_copy
            // A BasicAction that replaces all nodes stored by reference with
            // nodes stored by value.
            struct _deep_copy
              : detail::as_action_<switch_(detail::_deep_copy_cases)>
            {};

            ////////////////////////////////////////////////////////////////////////////////////////
            // deep_copy
            // Replaces all nodes stored by reference with nodes stored by value.
            template<typename E>
            auto deep_copy(E && e)
            BOOST_PROTO_AUTO_RETURN(
                proto::cxx11::_deep_copy()(static_cast<E &&>(e))
            )

            namespace functional
            {
                ////////////////////////////////////////////////////////////////////////////////////
                // deep_copy
                // A UnaryPolymorphicFunction that replaces all nodes stored by reference
                // with nodes stored by value.
                struct deep_copy
                {
                    template<typename E>
                    auto operator()(E && e) const
                    BOOST_PROTO_AUTO_RETURN(
                        proto::cxx11::_deep_copy()(static_cast<E &&>(e))
                    )
                };
            }
        }
    }
}

#endif
