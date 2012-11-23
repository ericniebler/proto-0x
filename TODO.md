A Grab-Bag of Miscellaneous Thoughts
====================================

* Implement proper lexical scoping for `let` expressions.
    + Can the call action be modified to automatically create a proper lexical scope?

* Think about basic actions.
    + Is the interface correct?
    + Maybe the environment should be the second parameter and state should be third.
    + Should the local variable scope be part of the environment, or should it be a
      fourth parameter?
    + Should *all* environment variables be part of a scope? Should we allow
      non-environments in the environments parameter as Proto-current does?

* Thoughts about `construct(foo<some_action<int>>())`
    + It doesn't invoke some_action. Should it? With what semantics? Is it right to be looking
      for a nested ::type after substitution, or should it be checking for action-ness instead
      (or in addtion to)? What would that break?

* How about a `static_cast_` function object?

* Should terminal-ness really be encoded in the tag?

* Functional namespace should be carved up: `proto::functional::fusion`? `proto::functional::std`?

* Should `or_/when` be renamed `match/case_`? Answer: probably. Maybe even:
    + `struct MyAlgo : match<case_(g0, a0), case_(g1, a0), /*...*/> {};`

* Should `and_`, `or_`, and `not_` expect Boolean Actions, like `if_`? And logically negating a match
  could be `match_not()`

* Everyplace where we directly return an rvalue reference is potentially creating a dangling
  reference.

* An action like `plus(transform(_left), transform(_right))` finds proto's overloaded `operator+` 
  and is naturally like the pass-through transform. DANGER: don't return dangling references
  if the domain captures child nodes by reference!
    - How about this: 
    
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
