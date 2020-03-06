#ifndef OPERATORS_H
#define OPERATORS_H

#include <cmath>
#include <memory>
// val1 will always be the top element, i.e. the operand to the right
// val2 will always be the next element, i.e. the operand to the left

enum Associatvity { LEFTRIGHT,
                    RIGHTLEFT };
enum Nary { BINARY,
            UNARY };

class Operator {
   public:
    // binary
    virtual double apply(double val1, double val2) {}
    // unary
    virtual double apply(double val1) {}
};

class Plus : public Operator {
    virtual double apply(double val1, double val2) { return val1 + val2; }
};

class Mul : public Operator {
    virtual double apply(double val1, double val2) { return val1 * val2; }
};

class Div : public Operator {
    virtual double apply(double val1, double val2) { return val2 / val1; }
};

class Sub : public Operator {
    virtual double apply(double val1, double val2) { return val2 - val1; }
};

class Exp : public Operator {
    virtual double apply(double val1, double val2) { return pow(val2, val1); }
};

class Neg : public Operator {
    virtual double apply(double val1) { return val1 * -1; }
};

class LeftParen : public Operator {};
using op_ptr = std::shared_ptr<Operator>;

struct CompleteOperator {
    op_ptr op;
    int precedence;
    Associatvity assoc;
    Nary nAry;
};

using cop_ptr = CompleteOperator*;
#endif