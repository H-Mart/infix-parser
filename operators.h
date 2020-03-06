#ifndef OPERATORS_H
#define OPERATORS_H

#include <cmath>
#include <memory>
// val1 will always be the top element, i.e. the operand to the right
// val2 will always be the next element, i.e. the operand to the left

enum Associatvity { LEFTRIGHT, RIGHTLEFT };
enum Nary { BINARY, UNARY };

class Operator {
   public:
    virtual int apply(int val1, int val2){}
    virtual double apply(double val1, double val2){}
    virtual double apply(int val1) {}
    virtual double apply(double val1) {}
};

class Plus : public Operator {
    virtual int apply(int val1, int val2) { return val1 + val2; }
    virtual double apply(double val1, double val2) { return val1 + val2; }
};

class Mul : public Operator {
    virtual int apply(int val1, int val2) { return val1 * val2; }
    virtual double apply(double val1, double val2) { return val1 * val2; }
};

class Div : public Operator {
    virtual int apply(int val1, int val2) { return val2 / double(val1); }
    virtual double apply(double val1, double val2) { return val2 / val1; }
};

class Sub : public Operator {
    virtual int apply(int val1, int val2) { return val2 - val1; }
    virtual double apply(double val1, double val2) { return val2 - val1; }
};

class Exp : public Operator {
    virtual int apply(int val1, int val2) { return pow(val1, val2); }
    virtual double apply(double val1, double val2) { return pow(val1, val2); }
    virtual double apply(int val1, double val2) { return pow(val1, val2); }
    virtual double apply(double val1, int val2) { return pow(val1, val2); }
};

class Neg : public Operator {
    virtual double apply(int val1) { return val1 * -1; }
    virtual double apply(double val1) { return val1 * -1; }
};

class LeftParen : public Operator {};
// using op_ptr = std::shared_ptr<Operator>;
using op_ptr = Operator*;

struct CompleteOperator {
    op_ptr op;
    int precedence;
    Associatvity assoc;
    Nary nAry;
    // explicit CompleteOperator();
    // CompleteOperator(const CompleteOperator&) = delete;
    // CompleteOperator& operator=(const CompleteOperator&) = delete;
    // ~CompleteOperator() = default;
    // CompleteOperator(Operator* op, int prec, Associatvity assoc, Nary nary)
    //     : op(op_ptr(op)), precedence(prec), assoc(assoc), nAry(nary) {}
};

// using cop_ptr = std::shared_ptr<CompleteOperator>;
using cop_ptr = CompleteOperator*;
#endif