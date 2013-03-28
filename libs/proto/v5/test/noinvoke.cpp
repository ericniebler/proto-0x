////////////////////////////////////////////////////////////////////////////////////////////////////
// noinvoke.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <boost/proto/v5/proto.hpp>
#include "./unit_test.hpp"
namespace proto=boost::proto;
using proto::_;

struct Test
  : proto::def<
        proto::make(
            proto::noinvoke<
                // This remove_pointer invocation is bloked by noinvoke
                std::remove_pointer<
                    // This add_pointer invocation is *not* blocked by noinvoke
                    std::add_pointer<_>
                >
            >()
        )
    >
{};

struct Test2
  : proto::def<
        // This add_pointer gets invoked because a substitution takes place
        // within it.
        proto::make(
            std::add_pointer<
                proto::noinvoke<
                    // This remove_pointer invocation is bloked by noinvoke
                    std::remove_pointer<
                        // This add_pointer invocation is *not* blocked by noinvoke
                        std::add_pointer<_>
                    >
                >
            >()
        )
    >
{};

template<typename T, typename U>
struct select2nd
{
    using type = U;
};

struct Test3
  : proto::def<
        // This add_pointer gets invoked because a substitution takes place
        // within it.
        proto::make(
            select2nd<
                void
              , proto::noinvoke<
                    // This remove_pointer invocation is bloked by noinvoke
                    select2nd<
                        void
                        // This add_pointer invocation is *not* blocked by noinvoke
                      , std::add_pointer<_>
                    >
                >
            >()
        )
    >
{};

void test_noinvoke()
{
    using Int = proto::literal<int>;
    Int i {42};

    static_assert(
        std::is_same<
            std::result_of<Test(Int)>::type
          , std::remove_pointer<Int *>
        >::value
      , "test 1"
    );

    std::remove_pointer<Int *> t = Test()(i);
    BOOST_PROTO_IGNORE_UNUSED(t);

    static_assert(
        std::is_same<
            std::result_of<Test2(Int)>::type
          , std::remove_pointer<Int *> *
        >::value
      , "test 2"
    );

    std::remove_pointer<Int *> * t2 = Test2()(i);
    BOOST_PROTO_IGNORE_UNUSED(t2);

    static_assert(
        std::is_same<
            std::result_of<Test3(Int)>::type
          , select2nd<void, Int *>
        >::value
      , "test 3"
    );

    select2nd<void, Int *> t3 = Test3()(i);
    BOOST_PROTO_IGNORE_UNUSED(t3);
}

using namespace boost::unit_test;
////////////////////////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::noinvoke");

    test->add(BOOST_TEST_CASE(&test_noinvoke));

    return test;
}
