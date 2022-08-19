#include <string>
#include <vector>
#include <memory>
#include "Number.hh"

class ExpressionAST {
public:
    virtual ~ExpressionAST() {}
};  

class VariableAST : public ExpressionAST {
private:
    std::string name;

public:
    VariableAST(const std::string& name): name(name) {}
};

class FunctionCallAST : public ExpressionAST {
private:
    std::string name;
    std::vector<std::unique_ptr<ExpressionAST>> args;

public:
    FunctionCallAST(const std::string& name, std::vector<std::unique_ptr<ExpressionAST>> args)
    : name(name), args(std::move(args)) {}
};

class LiteralAST : public ExpressionAST {
private:
    ValueType type;
    std::unique_ptr<Number> value;

public:
    LiteralAST(std::unique_ptr<Number> value)
    : value(std::move(value)), type(value->get_type()) {}
};

class BinaryOpAST : public ExpressionAST {
private:
    std::unique_ptr<ExpressionAST> LHS, RHS;
    char op;

public:
    BinaryOpAST(std::unique_ptr<ExpressionAST> LHS, std::unique_ptr<ExpressionAST> RHS, char op)
    : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}

    BinaryOpAST(std::unique_ptr<ExpressionAST> RHS, char op)
    : RHS(std::move(RHS)), op(op) {}

    void addLeft(std::unique_ptr<ExpressionAST> LHS) {
        LHS = std::move(LHS);
    }
};
