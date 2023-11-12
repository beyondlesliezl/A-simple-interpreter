# some important things 

## part4

### how to convert the grammar to source code

1. Each rule, ``R``, defined in the grammar, becomes a method with the same name, and references to that rule become a method call: ``R()``. The body of the method follows the flow of the body of the rule using the very same guidelines.
2. Alternatives ``(a1 | a2 | aN)`` become an ``if-elif-else`` statement
3. An optional grouping ``(…)*`` becomes a while statement that can loop over zero or more times
4. Each token reference ``T`` becomes a call to the method ``eat: eat(T)``. The way the eat method works is that it consumes the token T if it matches the current lookahead token, then it gets a new token from the lexer and assigns that token to the ``current_token`` internal variable.

## part5

### [how to construct a grammar from the precedence table](https://ruslanspivak.com/lsbasi-part5/)

1. For each level of precedence define a non-terminal. The body of a production for the non-terminal should contain arithmetic operators from that level and non-terminals for the next higher level of precedence.

2. Create an additional non-terminal factor for basic units of expression, in our case, integers. The general rule is that if you have N levels of precedence, you will need N + 1 non-terminals in total: one non-terminal for each level plus one non-terminal for basic units of expression.

3. 先匹配，优先级越低，即在parse 树中， 越高的node， 优先级越低

#### example

``7 + 3 - 1``   `` 8 / 4 * 2 ``

1. According to Rule 1 we will define two non-terminals: a non-terminal called expr for level 2 and a non-terminal called term for level 1. 

2. And by following Rule 2 we will define a factor non-terminal for basic units of arithmetic expressions, integers.

The start symbol of our new grammar will be expr and the expr production will contain a body representing the use of operators from level 2, which in our case are operators ``+`` and ``-`` , and will contain term non-terminals for the next higher level of precedence, level 1:

``expr : term((PLUS | MINUS) term)*``

The term production will have a body representing the use of operators from level 1, which are operators * and / , and it will contain the non-terminal factor for the basic units of expression, integers:

``term : factor((MUL | DIV) factor)*``

And the production for the non-terminal factor will be:

``factor : INTEGER``