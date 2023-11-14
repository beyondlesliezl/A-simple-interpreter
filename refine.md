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

## part6

### repetition is the mother of all learning.

## part7

### parse tree

* The parse tree records a sequence of rules the parser applies to recognize the input.

* The root of the parse tree is labeled with the grammar start symbol.

* Each interior node represents a non-terminal, that is it represents a grammar rule application, like expr, term, or factor in our case.

* Each leaf node represents a token.

### the main differences between ASTs and Parse trees

* ASTs uses operators/operations as root and interior nodes and it uses operands as their children.

*ASTs do not use interior nodes to represent a grammar rule, unlike the parse tree does.

* ASTs don’t represent every detail from the real syntax (that’s why they’re called abstract) - no rule nodes and no parentheses, for example.

* ASTs are dense compared to a parse tree for the same language construct.

### abstract syntax tree

An abstract syntax tree (AST) is a tree that represents the abstract syntactic structure of a language construct where *each interior node and the root node represents an operator*, and the children of the node represent the operands of that operator.

### how do you encode operator precedence in an AST

To represent that “X happens before Y” you just need to put X lower in the tree than Y.

距离根节点越远，优先级越低

### How do you navigate the tree to properly evaluate the expression represented by that tree

 You do that by using a *postorder traversal* - a special case of depth-first traversal - which starts at the root node and recursively visits the children of each node from left to right. The postorder traversal visits nodes as far away from the root as fast as it can.

 The reason we’re going to use a postorder traversal for our interpreter is that first, we need to evaluate interior nodes lower in the tree because they represent operators with higher precedence and second, we need to evaluate operands of an operator before applying the operator to those operands. 

### recursive-descent parser

a recursive-descent parser is a top-down parser that uses a set of recursive procedures to process the input. Top-down reflects the fact that the parser begins by constructing the top node of the parse tree and then gradually constructs lower nodes.

### visualize ASTs

``python3 genastdot.py "7 + 3 * (10 / (12 / (3 + 1) - 1))" > \
  ast.dot && dot -Tpng -o ast.png ast.dot``

## part8

### What is a unary operator

A unary operator is an operator that operates on one operand only.

Here are the rules for unary plus and unary minus operators:

* The unary minus (-) operator produces the negation of its numeric operand
* The unary plus (+) operator yields its numeric operand without change

* The unary operators have higher precedence than the binary operators +, -, *, and /

## part13

### What information about variables do we need to collect?

1. Name (we need to know the name of a declared variable because later we will be looking up variables by their names)

2. Category (we need to know what kind of an identifier it is: variable, type, procedure, and so on)

3. Type (we’ll need this information for type checking)

Symbols will hold that information (name, category, and type) about our variables. What’s a symbol? A symbol is an identifier of some program entity like a variable, subroutine, or built-in type.

What about the category? We will encode category into the class name.

### Where and how should we store the collected information?

The answer is, as you probably already know, in the symbol table.

What is a symbol table? A symbol table is an abstract data type for tracking various symbols in source code. Think of it as a dictionary where the key is the symbol’s name and the value is an instance of the symbol class (or one of its subclasses). 

### How do we implement the “go over all variable declarations” step?

We will create a new AST visitor that will do some actions on visiting VarDecl AST nodes.