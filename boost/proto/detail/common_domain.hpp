////////////////////////////////////////////////////////////////////////////////////////////////////
// common_domain.hpp
// Contains definition of common_domain\<\> class template
// for finding the domain that is common among the specified
// domains
//
//  Copyright 2012 Daniel Wallin, Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Many thanks to Daniel Wallin who first implemented this code. Thanks
// also to Jeremiah Willcock, John Bytheway and Krishna Achuthan who
// offered alternate solutions to this tricky programming problem.

#ifndef BOOST_PROTO_COMMON_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_COMMON_DOMAIN_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include <boost/proto/proto_fwd.hpp>

#ifndef BOOST_PROTO_ASSERT_VALID_DOMAIN
# define BOOST_PROTO_ASSERT_VALID_DOMAIN(DOM)                                                       \
    static_assert(!std::is_same<DOM, ::boost::proto::detail::not_a_domain>::value, "no common domain!")
#endif

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<typename Domain>
            struct domain_
              : domain_<typename Domain::proto_super_domain_type>
            {
                typedef Domain type;
                using domain_<typename Domain::proto_super_domain_type>::deduce;
                static Domain deduce(domain_<Domain>*);
            };

            template<>
            struct domain_<not_a_domain>
            {
                typedef not_a_domain type;
                static not_a_domain deduce(void*);
            };

            template<>
            struct domain_<default_domain>
              : domain_<not_a_domain>
            {};

            sized_type<1>::type default_test(void*, void*);
            sized_type<2>::type default_test(domain_<default_domain>*, void*);
            sized_type<3>::type default_test(void*, domain_<default_domain>*);
            sized_type<4>::type default_test(domain_<default_domain>*, domain_<default_domain>*);

            ////////////////////////////////////////////////////////////////////////////////////////
            // common_domain2
            template<
                typename D0
              , typename D1
              , std::size_t DefaultCase = sizeof(proto::detail::default_test((domain_<D0>*)0, (domain_<D1>*)0))
            >
            struct common_domain2
            {
                typedef decltype(domain_<D0>::deduce((domain_<D1>*)0)) type;
                BOOST_PROTO_ASSERT_VALID_DOMAIN(type);
            };

            template<typename D0, typename D1>
            struct common_domain2<D0, D1, 2>
            {
                typedef D1 type;
            };

            template<typename D0, typename D1>
            struct common_domain2<D0, D1, 3>
            {
                typedef D0 type;
            };

            template<typename D0>
            struct common_domain2<D0, default_domain, 4>
            {
                typedef D0 type;
            };

            template<typename D1>
            struct common_domain2<default_domain, D1, 4>
            {
                typedef D1 type;
            };

            template<>
            struct common_domain2<default_domain, default_domain, 4>
            {
                typedef default_domain type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////
            // common_domain
            template<typename ...DS>
            struct common_domain;

            template<typename D0>
            struct common_domain<D0>
            {
                typedef D0 type;
            };

            template<typename D0, typename D1>
            struct common_domain<D0, D1>
              : common_domain2<D0, D1>
            {};

            template<typename D0, typename D1, typename ...DS>
            struct common_domain<D0, D1, DS...>
              : common_domain<typename common_domain2<D0, D1>::type, DS...>
            {};
        }
    }
}

#endif // BOOST_PROTO_COMMON_DOMAIN_HPP_INCLUDED
