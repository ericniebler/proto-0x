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

#ifndef BOOST_PROTO_DETAIL_COMMON_DOMAIN_HPP_INCLUDED
#define BOOST_PROTO_DETAIL_COMMON_DOMAIN_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include <boost/proto/cxx11/proto_fwd.hpp>
#include <boost/proto/cxx11/utility.hpp>

#ifndef BOOST_PROTO_ASSERT_VALID_DOMAIN
# define BOOST_PROTO_ASSERT_VALID_DOMAIN(DOM)                                                       \
    static_assert(!std::is_same<DOM, ::boost::proto::cxx11::detail::not_a_domain>::value, "no common domain!")
#endif

namespace boost
{
    namespace proto
    {
        inline namespace cxx11
        {
            namespace detail
            {
                template<typename Domain>
                struct domain_
                  : domain_<typename Domain::proto_super_domain_type>
                {
                    using type = Domain;
                    using domain_<typename Domain::proto_super_domain_type>::deduce;
                    static Domain deduce(domain_<Domain>*);
                };

                template<>
                struct domain_<not_a_domain>
                {
                    using type = not_a_domain;
                    static not_a_domain deduce(void*);
                };

                template<>
                struct domain_<default_domain>
                  : domain_<not_a_domain>
                {};

                utility::sized_type<1> default_test(void*, void*);
                utility::sized_type<2> default_test(domain_<default_domain>*, void*);
                utility::sized_type<3> default_test(void*, domain_<default_domain>*);
                utility::sized_type<4> default_test(domain_<default_domain>*, domain_<default_domain>*);

                ////////////////////////////////////////////////////////////////////////////////////
                // common_domain2
                template<
                    typename D0
                  , typename D1
                  , std::size_t DefaultCase = sizeof(proto::cxx11::detail::default_test((domain_<D0>*)0, (domain_<D1>*)0))
                >
                struct common_domain2
                {
                    using type = decltype(domain_<D0>::deduce((domain_<D1>*)0));
                    BOOST_PROTO_ASSERT_VALID_DOMAIN(type);
                };

                template<typename D0, typename D1>
                struct common_domain2<D0, D1, 2>
                {
                    using type = D1;
                };

                template<typename D0, typename D1>
                struct common_domain2<D0, D1, 3>
                {
                    using type = D0;
                };

                template<typename D0>
                struct common_domain2<D0, default_domain, 4>
                {
                    using type = D0;
                };

                template<typename D1>
                struct common_domain2<default_domain, D1, 4>
                {
                    using type = D1;
                };

                template<>
                struct common_domain2<default_domain, default_domain, 4>
                {
                    using type = default_domain;
                };

                ////////////////////////////////////////////////////////////////////////////////////
                // common_domain
                template<typename ...DS>
                struct common_domain;

                template<typename D0>
                struct common_domain<D0>
                {
                    using type = D0;
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
}

#endif // BOOST_PROTO_DETAIL_COMMON_DOMAIN_HPP_INCLUDED
