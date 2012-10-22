A Grab-Bag of Miscellaneous Thoughts
====================================

* Implement  proper lexical scoping for let expressions.
    + Can the call action be modified to automatically create a proper lexical scope?

* Think about basic actions.
    + Is the interface correct?
    + Maybe the environment should be the second parameter and state should be third.
    + Should the local variable scope be part of the environment, or should it be a
      fourth parameter?
    + Should *all* environment variables be part of a scope? Should we allow
      non-environments in the environments parameter as Proto-current does?

* Thoughts about `construct(foo<some_action<int>>())`
    - It doesn't invoke some_action. Should it? With what semantics? Is it right to be looking
      for a nested ::type after substitution, or should it be checking for action-ness instead
      (or in addtion to)? What would that break?

* How about a `static_cast_` function object?

* Should terminal-ness really be encoded in the tag?

* I don't really like the use of `enable_if<!is_tag...>` in action specialization for callables.

* Functional namespace should be carved up: `proto::functional::fusion`? `proto::functional::std`?

* Should `or_/when` be renamed `match/case_`?

* Everyplace where we directly return an rvalue reference is potentially creating a dangling
  reference.

* Crazy thought: what if the tags (e.g. `proto::plus`) were aliases for `proto::op`
  (`proto::op<proto::some_plus_tag>`). It could be used in actions to add stuff, and an action
  like `plus(transform(_left), transform(_right))` would find proto's overloaded `operator+` 
  and naturally be like the pass-through transform.
      + Problem: `plus(transform, transform)` wouldn't be "right" since it would apply transform
        twice to the current expression and build a plus node out of that instead of out of the
        transformed children.