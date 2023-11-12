# part4

## how to convert the grammar to source code

1. Each rule, R, defined in the grammar, becomes a method with the same name, and references to that rule become a method call: R(). The body of the method follows the flow of the body of the rule using the very same guidelines.

2. Alternatives ``(a1 | a2 | aN)`` become an ``if-elif-else`` statement

3. An optional grouping``(…)*`` becomes a while statement that can loop over zero or more times

4. Each token reference ``T`` becomes a call to the method ``eat: eat(T)``. The way the eat method works is that it consumes the token T if it matches the current lookahead token, then it gets a new token from the lexer and assigns that token to the ``current_token`` internal variable.

