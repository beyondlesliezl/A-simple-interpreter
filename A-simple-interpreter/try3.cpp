#include<iostream>
#include<cstring>
#include<string>
using namespace std;

enum TokenType {
    INTEGER,
    PLUS,
    MINUS,
    EOF_TOKEN
};

class Token {
    public:
        TokenType type;
        string value;

        Token(TokenType type, string value) : type(type), value(value) {};

        string to_str() const {
            return "Token(" + to_string(this->type) + ", " + value + ")";
        }
};

class Interpreter{
    public:
        string text;
        int pos;
        Token current_token;
        char current_char;

        Interpreter(string text) : text(text), pos(0), current_token(INTEGER, "") {};

        void error() {
            throw runtime_error("Invalid syntax");
        }

        void advance() {
            this->pos += 1;
            if (this->pos > this->text.length() - 1) 
                this->current_char = '\0';
            else
                this->current_char = this->text[this->pos];
        }

        void skip_whitespace() {
            while (this->current_char != '\0' && isspace(this->current_char))
                this->advance();
        }

        int integer() {
            string result = "";
            while (this->current_char != '\0' && isdigit(this->current_char)) {
                result += this->current_char;
                this->advance();
            }
            return stoi(result);
        }

        Token get_next_token() {
            while(this->current_char != '\0') {
                if (isspace(this->current_char)) {
                    this->skip_whitespace();
                    continue;
                }

                if (isdigit(this->current_char)) {
                    return Token(INTEGER, to_string(integer()));
                }

                if (this->current_char == '+') {
                    this->advance();
                    return Token(PLUS, "+");
                }

                if (this->current_char == '-') {
                    this->advance();
                    return Token(MINUS, "-");
                }
                this->error();
            }

            return Token(EOF_TOKEN, "");
        }

        void eat(TokenType token_type) {
            if (this->current_token.type == token_type)
                this->current_token = this->get_next_token();
            else
                this->error();
        }

        int term() {
            Token token = this->current_token;
            this->eat(INTEGER);
            return stoi(token.value);
        }

        int expr() {
            this->current_token = this->get_next_token();

            int result = this->term();
            while (this->current_token.type == PLUS || this->current_token.type == MINUS) {
                Token token = this->current_token;
                if (token.type == PLUS) {
                    this->eat(PLUS);
                    result = result + this->term();
                }
                else if (token.type == MINUS) {
                    this->eat(MINUS);
                    result = result - this->term();
                }
            }
            return result;
        }
};

int main() {
    // Example usage:
    Interpreter interpreter("3 + 5 - 2");
    int result = interpreter.expr();

    std::cout << "Result: " << result << std::endl;

    return 0;
}