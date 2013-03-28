////////////////////////////////////////////////////////////////////////////////////////////////////
// def.cpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/v5/proto.hpp>

namespace proto = boost::proto;
using proto::_;

struct Grammar
  : proto::def<
        proto::match(
            proto::terminal(int)
          , proto::plus(Grammar, Grammar)
        )
    >
{};

static_assert(proto::is_grammar<Grammar>::value, "");
static_assert(!proto::is_action<Grammar>::value, "");

static_assert(proto::is_grammar<proto::match(proto::terminal(int))>::value, "");
static_assert(!proto::is_action<proto::match(proto::terminal(int))>::value, "");

struct GrammarAndAction
  : proto::def<
        proto::match(
            proto::case_(proto::terminal(int), proto::pass)
          , proto::case_(proto::plus(Grammar, Grammar), proto::pass)
        )
    >
{};

static_assert(proto::is_grammar<GrammarAndAction>::value, "");
static_assert(proto::is_action<GrammarAndAction>::value, "");

static_assert(proto::is_grammar<proto::match(proto::case_(proto::terminal(int), proto::pass))>::value, "");
static_assert(proto::is_action<proto::match(proto::case_(proto::terminal(int), proto::pass))>::value, "");

struct Action
  : proto::def<
        proto::fold(_, proto::make(int()), proto::_state)
    >
{};

static_assert(!proto::is_grammar<Action>::value, "");
static_assert(proto::is_action<Action>::value, "");

static_assert(!proto::is_grammar<proto::fold(_, proto::make(int()), proto::_state)>::value, "");
static_assert(proto::is_action<proto::fold(_, proto::make(int()), proto::_state)>::value, "");

static_assert(proto::is_grammar<_>::value, "");
static_assert(proto::is_action<_>::value, "");

struct foo
{};

static_assert(!proto::is_grammar<foo>::value, "");
static_assert(!proto::is_action<foo>::value, "");
