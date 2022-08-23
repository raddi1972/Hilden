#include "Parser.hh"

LexerWraper::LexerWraper(const std::string& fileName) 
:curtok(0) {
    FILE* fptr = fopen(fileName.c_str(), "r");
    yyin = fptr;
    int curtok = yylex();
    while(curtok) {
        std::string val(yytext);
        tokens.push_back(Tok(val, curtok));
        curtok = yylex();
    }
}

void LexerWraper::printTokens() {
    for(auto token : tokens) {
        std::cout << "TYPE: " << token.tok << " VALUE: " << token.val << std::endl;
    }
}

std::unique_ptr<ExpressionAST> Parser::ParseExpression() {
    std::cout << "ParseExpression" << std::endl;
    return ParseSum();
}

std::unique_ptr<ExpressionAST> Parser::ParseSum() {
    std::cout << "Parsing Sum" << std::endl;
    auto term = ParseTerm();
    return ParseSumA(std::move(term));
}

std::unique_ptr<BinaryOpAST> Parser::ParseSumA(std::unique_ptr<ExpressionAST> LHS) {
    std::cout << "ParseSumA" << std::endl;
    Tok nextTok = lex->peek(0);

    if(nextTok.tok == Token::OP_PLUS)
    {
        Tok op = lex->next(); // consumes "+"
        auto RHS = ParseTerm();
        auto parsed = std::make_unique<BinaryOpAST>(std::move(LHS), std::move(RHS), op.val[0]);
        return ParseSumA(std::move(parsed));
    }

    if(nextTok.tok == Token::OP_MINUS)
    {
        Tok op = lex->next(); // consumes "+"
        auto RHS = ParseTerm();
        auto parsed = std::make_unique<BinaryOpAST>(std::move(LHS), std::move(RHS), op.val[0]);
        return ParseSumA(std::move(parsed));
    }
}

std::unique_ptr<ExpressionAST> Parser::ParseTerm() {
    std::cout << "ParseTerm" << std::endl;
    auto factor = ParseFactor(); // Getting the factor
    return ParseTermA(std::move(factor));
}

std::unique_ptr<BinaryOpAST> Parser::ParseTermA(std::unique_ptr<ExpressionAST> LHS) {
    Tok nextTok = lex->peek(0);
    std::cout << "ParseTermA" << std::endl;

    if(nextTok.tok == Token::OP_MULT) 
    {
        Tok op = lex->next(); // consumes "*"
        auto RHS = ParseFactor(); // Parses Factor
        auto parsed = std::make_unique<BinaryOpAST>(std::move(LHS), std::move(RHS), op.val[0]);
        return ParseTermA(std::move(parsed));
    }

    if(nextTok.tok == Token::OP_DIV)
    {
        Tok op = lex->next();
        auto RHS = ParseFactor(); // Parses Factor
        auto parsed = std::make_unique<BinaryOpAST>(std::move(LHS), std::move(RHS), op.val[0]);
        return ParseTermA(std::move(parsed));
    }

    return nullptr;
}

std::unique_ptr<ExpressionAST> Parser::ParseFactor() {
    std::cout << "ParseFactor" << std::endl;
    Tok nextTok = lex->peek(0);
    if(nextTok.tok == Token::OPEN_CURLY) // parses "( expr )"
    {
        
        lex->next(); // Consumes the "("
        auto expr = ParseExpression();
        lex->next(); // Consumes the ")"
        return std::move(expr);
    }
    
    if(nextTok.tok == Token::IDENTIFIER) 
    {
        return ParseIdentifier();
    }

    if(nextTok.tok == Token::LIT_HCHAR ||
        nextTok.tok == Token::LIT_HFLOAT ||
        nextTok.tok == Token::LIT_HINT ||
        nextTok.tok == Token::LIT_HSTRING)
    {
        return ParseLiteral();
    }

    return nullptr;
}

std::unique_ptr<LiteralAST> Parser::ParseLiteral() {
    std::cout << "ParseLiteral" << std::endl;

    Tok toParse = lex->next(); // Peeks the next token to check
    ValueType curType;
    switch (toParse.tok)
    {
    case Token::LIT_HCHAR:
        curType = ValueType::CHAR;
        break;
    case Token::LIT_HINT:
        curType = ValueType::INTEGER;
        break;
    case Token::LIT_HFLOAT:
        curType = ValueType::FLOAT;
        break;
    case Token::LIT_HSTRING:
        curType = ValueType::STRING;
        break;
    default:
        curType = ValueType::VOID;
        break;
    }

    std::unique_ptr<Number> lit = std::make_unique<Number>(toParse.val, curType);
    return std::make_unique<LiteralAST>(std::move(lit));
}

std::unique_ptr<ExpressionAST> Parser::ParseIdentifier() {
    std::cout << "ParseIdentifier" << std::endl;

    auto var_name = lex->next().val;

    if(lex->peek(0).tok != OPEN_PAREN)
        return std::make_unique<VariableAST>(var_name);
    
    lex->next(); // eat '('

    std::vector<std::unique_ptr<ExpressionAST>> args;
    auto cur_expr = ParseExpression();
    while(cur_expr) {
        args.push_back(std::move(cur_expr));
        cur_expr = std::move(ParseExpression());
    }


    if(lex->next().tok != Token::CLOSE_PAREN) {
        return nullptr;
    } // eat '('

    return std::make_unique<FunctionCallAST> (var_name, std::move(args));
}


