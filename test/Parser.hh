
#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>
#include <memory>
#include "AST.hh"
#include "tokens.hh"

extern int yylex();
extern int yylineno; // Helps in the error messages (to find the line number)
extern char* yytext; // gives the text which is identified by the lexer
extern FILE* yyin;

struct Tok {
    Token tok;
    std::string val;

    Tok(const std::string& val, int curtok)
        :tok((Token)curtok), val(val) {}
};

class LexerWraper {
private:
    std::vector<Tok> tokens;
    int curtok;
public:
    LexerWraper(const std::string& fileName);

    void printTokens();

    Tok peek(int k) {
        if(curtok + k < tokens.size()) {
            return tokens[curtok + k];
        }
    }

    Tok next() {
        if(curtok + 1 < tokens.size()) {
            curtok++;
            return tokens[curtok + 1];
        } else {
            return Tok("", Token::END_OF);
        }
    }
};

class Parser {

private:
    std::unique_ptr<LexerWraper> lex;

public:
    Parser(const std::string& filename) {
        lex = std::make_unique<LexerWraper>(filename);
    }

    void printTokens() {
        lex->printTokens();
    }

    std::unique_ptr<ExpressionAST> ParseExpression();
    std::unique_ptr<ExpressionAST> ParseIdentifier();
    std::unique_ptr<LiteralAST> ParseLiteral();
    std::unique_ptr<ExpressionAST> ParseBinaryOpAST();

    std::unique_ptr<ExpressionAST> ParseFactor();
    std::unique_ptr<ExpressionAST> ParseTerm();
    std::unique_ptr<BinaryOpAST> ParseTermA(std::unique_ptr<ExpressionAST> LHS);
};


#endif
