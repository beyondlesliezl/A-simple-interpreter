# Token types
#
# EOF (end-of-file) token is used to indicate that
# there is no more input left for lexical analysis
INTEGER, MUL, DIV, EOF, PLUS, MINUS= 'INTEGER', 'MUL', 'DIV', 'EOF', 'PLUS', 'MINUS'


class Token(object):
    def __init__(self, type, value):
        # token type: INTEGER, MUL, DIV, or EOF
        self.type = type
        # token value: non-negative integer value, '*', '/', or None
        self.value = value

    def __str__(self):
        """String representation of the class instance.

        Examples:
            Token(INTEGER, 3)
            Token(MUL, '*')
        """
        return 'Token({type}, {value})'.format(
            type=self.type,
            value=repr(self.value)
        )

    def __repr__(self):
        return self.__str__()


class Lexer(object):
    def __init__(self, text):
        # client string input, e.g. "3 * 5", "12 / 3 * 4", etc
        self.text = text
        # self.pos is an index into self.text
        self.pos = 0
        self.current_char = self.text[self.pos]

    def error(self):
        raise Exception('Invalid character')

    def advance(self):
        """Advance the `pos` pointer and set the `current_char` variable."""
        self.pos += 1
        if self.pos > len(self.text) - 1:
            self.current_char = None  # Indicates end of input
        else:
            self.current_char = self.text[self.pos]

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def integer(self):
        """Return a (multidigit) integer consumed from the input."""
        result = ''
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.advance()
        return int(result)

    def get_next_token(self):
        """Lexical analyzer (also known as scanner or tokenizer)

        This method is responsible for breaking a sentence
        apart into tokens. One token at a time.
        """
        while self.current_char is not None:

            if self.current_char.isspace():
                self.skip_whitespace()
                continue

            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())

            if self.current_char == '*':
                self.advance()
                return Token(MUL, '*')

            if self.current_char == '/':
                self.advance()
                return Token(DIV, '/')

            if self.current_char == '+':
                self.advance()
                return Token(PLUS, '+')
            
            if self.current_char == '-':
                self.advance()
                return Token(MINUS, '-')

            self.error()

        return Token(EOF, None)


class Interpreter(object):
    def __init__(self, lexer):
        self.lexer = lexer
        # set current token to the first token taken from the input
        self.current_token = self.lexer.get_next_token()

    def error(self):
        raise Exception('Invalid syntax')

    def eat(self, token_type):
        # compare the current token type with the passed token
        # type and if they match then "eat" the current token
        # and assign the next token to the self.current_token,
        # otherwise raise an exception.
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    def factor(self):
        """Return an INTEGER token value.

        factor : INTEGER
        """
        token = self.current_token
        self.eat(INTEGER)
        return token.value

    def addop(self):
        token = self.current_token 
        if token.type == PLUS:
            self.eat(PLUS)
            return Token(PLUS, '+')
        elif token.type == MINUS:
            self.eat(MINUS)
            return Token(MINUS, '-')

    def mulop(self):
        token = self.current_token
        if token.type == MUL:
            self.eat(MUL)
            return Token(MUL, '*')
        elif token.type == DIV:
            self.eat(DIV)
            return Token(DIV, '-')
        
    def term(self):
        #* 这就是为什么要消除左递归了，如果是左递归，那么就会无限循环，无法书写代码
        result = self.factor()
        while self.current_token.type in (MUL, DIV):
            token = self.mulop()
            if token.type == DIV:
                result //= self.factor()
            else:
                result *= self.factor()
        return result


    def expr(self):
        """Arithmetic expression parser / interpreter.

        expr   : exp addop term | term
        addop  : (PLUS | MINUS)
        term   : term mulop factor | factor 
        mulop  : (MUL  | DIV)
        factor : (exp) | INTEGER
        -----------------------------------------------
        expr   : term (addop term)*
        addop  : (PLUS | MINUS)
        term   : factor (mulop factor)*
        mulop  : (MUL | DIV)
        factor : (exp) | INTEGER
        """
        result = self.term()

        while self.current_token.type in (PLUS, MINUS):
            token = self.addop()
            if token.type == PLUS:
                result = result + self.term()
            elif token.type == MINUS:
                result = result - self.term()

        return result


def main():
    while True:
        try:
            text = input('calc> ')
        except EOFError:
            break
        if not text:
            continue
        lexer = Lexer(text)
        interpreter = Interpreter(lexer)
        result = interpreter.expr()
        print(result)


if __name__ == '__main__':
    main()