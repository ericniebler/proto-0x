A Grab-Bag of Miscellaneous Thoughts
====================================

* Should `_(A(_left), B(_right))` be synonymous for `pass(A, B)`? That would be consistent with the
  use of `_` in expression patterns like `_(A,B)`, where `_` means "any tag", but inconsistent with
  the use of `_` as a primitive transform, where `_(A,B)` simply returns `a`.

* Each domain should have a post-process function object that takes and returns an expression. This
  is to replicate the generator functionality from Proto v4, where actions can be used to select
  between several wrapper types based on structural matching of the new expression.

* Thoughts about `make(foo<some_action<int>>())`
    + It doesn't invoke `some_action<>`. Should it? With what semantics? Is it right to be looking
      for a nested `::type` after substitution, or should it be checking for action-ness instead
      (or in addtion to)? What would that break?
    + Answer: it is technically not OK to test `some_action<int>` for action-ness if there were
      no substitutions because it may not actually instantiate properly. See `careful` test in
      test/make.cpp

* Proto v4 needs inline namespace also, and a macro-based solution for picking which namespace
  (v4 or v5) gets inlined.

* `_eval<lambda_eval>` should be `eval(lambda_eval)` or even `eval_with(lambda_eval)`

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

* Everyplace where we directly return an rvalue reference is potentially creating a dangling
  reference.

* Implementation of `unpack_expr` could probably use `fusion::invoke_function_object`.

