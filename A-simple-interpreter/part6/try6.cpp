#include<iostream>
#include<cstring>
#include<string>
using namespace std;

enum TokenType {
    INTEGER, MUL, DIV, EOF_TOKEN, PLUS, MINUS, LPAREN, RPAREN
};

class Token {
    public:
        TokenType type;
        string value;

        Token(TokenType t, string v) : type(t), value(v) {};

        string to_str() const {
            return "Token(" + to_string(this->type) + ", " + this->value + ")";
        }
};

class Lexer {
    public:
    string text;
    int pos;
    Token current_token;
    char current_char;

    Lexer(string text) : text(text), pos(0), current_token(INTEGER, "")
    {
    }

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void advance() {
        this->pos += 1;
        if (pos < this->text.length() - 1) {
            this->current_char = this->text[this->pos];
        } else {
            this->current_char = '\0';
        }
    }

    void skip_whitespace() {
        while(this->current_char != '\0' && isspace(this->current_char)) {
                advance();
        }
    }

    int integer() {
        string result = "";
        while(this->current_char != '\0' && isdigit(this->current_char)) {
            result += this->current_char;
            advance();
        }
        return stoi(result);
    }

    Token get_next_token() {
        while(this->current_char != '\0') {
            if(isspace(current_char)) {
                advance();
                continue;
            }

            if (isdigit(current_char)) {
                return Token(INTEGER, to_string(integer()));
            }

            if (current_char == '+') {
                advance();
                return Token(PLUS, "+");
            }

            if (current_char == '-') {
                advance();
                return Token(MINUS, "-");
            }

            if (current_char == '*') {
                advance();
                return Token(MUL, "*");
            }

            if (current_char == '/') {
                advance();
                return Token(MUL, "/");
            }
            
            if (current_char == '(') {
                advance();
                return Token(LPAREN, "(");
            }

            if (current_char == ')') {
                advance();
                return Token(RPAREN, ")");
            }

            error();
        }

        return Token(EOF_TOKEN, "");
    }
};

class Interpreter {
    public:
    Lexer lexer;
    Token CurrentToken;

    Interpreter(Lexer l): lexer(l), CurrentToken(this->lexer.get_next_token()) {};

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void eat(TokenType tp) {
        if (CurrentToken.type == tp) {
            CurrentToken = this->lexer.get_next_token();
        }
        else {
            error();
        }
    }

    int factor() {
        if (CurrentToken.type == INTEGER) {
            eat(INTEGER);
            return stoi(CurrentToken.value);
        } else {
            eat(LPAREN);
            int result = this->expr();
            eat(RPAREN);
            return result;
        }
    }

    int term() {
        int result = factor();
        while(CurrentToken.type == MUL || CurrentToken.type == DIV) {
            Token token = this->CurrentToken;
            if (token.type == MUL) {
                eat(MUL);
                result *= factor();
            } else {
                eat(DIV);
                result /= factor();
            }
        }
        return result;
    }

    int expr() {
        int result = term();
        while(CurrentToken.type == PLUS || CurrentToken.type == MINUS) {
            Token token = CurrentToken;
            if (token.type == PLUS) {
                result += term();
            } else {
                result -= term();
            }
        }
        return result;
    }
};

