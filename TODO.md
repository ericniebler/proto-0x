A Grab-Bag of Miscellaneous Thoughts
====================================

* Thoughts about `make(foo<some_action<int>>())`
    + It doesn't invoke `some_action<>`. Should it? With what semantics? Is it right to be looking
      for a nested `::type` after substitution, or should it be checking for action-ness instead
      (or in addtion to)? What would that break?
    + Answer: it is technically not OK to test `some_action<int>` for action-ness if there were
      no substitutions because it may not actually instantiate properly. See `careful` test in
      test/make.cpp

* Rethink the need for a separate `make` action entirely. It makes examples ugly and verbose.
  OTOH, it's teachable and simple: for any action `XXX(YYY)`, the type `XXX` trivially determines
  what the action does.

* An action like `plus(transform(_left), transform(_right))` finds proto's overloaded `operator+`
  and is naturally like the pass-through transform. DANGER: don't return dangling references
  if the domain captures child nodes by reference!
    - How about this:

```cpp
struct unary_plus
{
    template<typename T, BOOST_PROTO_ENABLE_IF(is_expr<decltype(+declval<T>())>::value)>
    auto operator()(T && t) const
    BOOST_PROTO_AUTO_RETURN(
        make_expr(unary_plus(), static_cast<T &&>(t))
    )

    template<typename T, BOOST_PROTO_ENABLE_IF(!is_expr<decltype(+declval<T>())>::value)>
    auto operator()(T && t) const
    BOOST_PROTO_AUTO_RETURN(
        + static_cast<T &&>(t)
    )
};
```

* Think about how to use old and new Proto side-by-side. Can inline namespaces help here?
  Eventually, this will need to be moved into a separate directory, right?

* Implement proper lexical scoping for `let` expressions.
    + Can the call action be modified to automatically create a proper lexical scope?

* Think about basic actions.
    + Is the interface correct?
    + Do we really need actions to support an arbitrary number of arguments, when we only use
      the first three in practice? It needlessly complicates the action protocol.
    + Should the local variable scope be part of the environment, or should it be a
      fourth parameter?
    + Should *all* environment variables be part of a scope? Should we allow
      non-environments in the environments parameter as Proto-current does?

* How about a `static_cast_` function object?

* Should terminal-ness really be encoded in the tag? (Answer: yeah, probably. Otherwise,
 `make_expr` needs another way to know whether it's building a terminal or not.)

* Everyplace where we directly return an rvalue reference is potentially creating a dangling
  reference.

* Implementation of `unpack_expr` could probably use `fusion::invoke_function_object`.

* Should `_(A(_left), B(_right))` be synonymous for `pass(A, B)`? That would be consistent with the
  use of `_` in expression patterns like `_(A,B)`, where `_` means "any tag", but inconsistent with
  the use of `_` as a primitive transform, where `_(A,B)` simply returns `a`.

* I think we have a problem with `as_expr` calling `store_child`. In proto-current,
  `functional::make_expr` doesn't create dangling references when used in transforms, regardless of
  the Domain's `as_child` policy. In proto-0x, `using store_child = by_ref` does cause `make_expr` to
  create dangling references. Should the operators should not have exactly the same behavior as
  `make_expr`? That would effect the use of the tag types as expression creators in actions.
