#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "operators.h"
#include "stack.h"

const int MAX_STACK_SIZE = 100;

// todo currently pops more off stack than exist

std::unordered_map<char, CompleteOperator> operators = {
    {'+', CompleteOperator{op_ptr(new Plus), 1, LEFTRIGHT, BINARY}},
    {'-', CompleteOperator{op_ptr(new Sub), 1, LEFTRIGHT, BINARY}},
    {'/', CompleteOperator{op_ptr(new Div), 10, LEFTRIGHT, BINARY}},
    {'*', CompleteOperator{op_ptr(new Mul), 10, LEFTRIGHT, BINARY}},
    {'^', CompleteOperator{op_ptr(new Exp), 100, RIGHTLEFT, BINARY}},
    {'~', CompleteOperator{op_ptr(new Neg), 10, RIGHTLEFT, UNARY}},
    {'$', CompleteOperator{op_ptr(new Mul2), 10, RIGHTLEFT, UNARY}},
    {'(', CompleteOperator{op_ptr(new LeftParen), -1, RIGHTLEFT, UNARY}}};

enum LastRead { OPERATOR, OPERAND };

void applyOperation(cop_ptr&, Stack<cop_ptr, MAX_STACK_SIZE>&,
                    Stack<double, MAX_STACK_SIZE>&);

// todo right left associativity has issues

bool checkEval(cop_ptr const& curr, cop_ptr const& prev) {
    return (!(curr->nAry == UNARY)) && (curr->assoc <= prev->assoc &&
                                        curr->precedence < prev->precedence) ||
           (curr->assoc == LEFTRIGHT && curr->assoc == prev->assoc &&
            curr->precedence == prev->precedence);
}

int main() {
    Stack<cop_ptr, MAX_STACK_SIZE> optrs;
    Stack<double, MAX_STACK_SIZE> opnds;
    LastRead lastRead = OPERATOR;  // if the first character is a '-'
                                   // then it needs to be treated as a negative

    while (1) {
        std::string test;
        std::cout << "-->";
        std::cin >> test;
        
        for (auto ch : test) {
            if (isdigit(ch)) {
                opnds.push(ch - '0');
                lastRead = OPERAND;
            } else if (ch == ' ') {
                continue;
            } else if (ch == '(') {
                optrs.push(cop_ptr(&operators[ch]));
                lastRead = OPERAND;
            } else if (ch == ')') {
                auto op = optrs.pop();
                while (op->precedence != -1) {
                    applyOperation(op, optrs, opnds);
                    op = optrs.pop();
                }
            } else {
                cop_ptr op;
                if (ch == '-' && lastRead == OPERATOR) {
                    op = cop_ptr(&operators['~']);
                } else {
                    op = cop_ptr(&operators[ch]);
                }
                while (!optrs.isEmpty() && checkEval(op, optrs.peek())) {
                    auto topOp = optrs.pop();
                    applyOperation(topOp, optrs, opnds);
                }
                optrs.push(op);
                lastRead = OPERATOR;
            }
        }
        while (!optrs.isEmpty()) {  // need more conditions
            auto op = optrs.pop();
            applyOperation(op, optrs, opnds);
        }
        std::cout << "result: " << opnds.pop() << std::endl;
    }
}
void applyOperation(cop_ptr& op, Stack<cop_ptr, MAX_STACK_SIZE>& optrs,
                    Stack<double, MAX_STACK_SIZE>& opnds) {
    if (op->nAry == BINARY) {
        auto top = opnds.pop();
        auto next = opnds.pop();
        opnds.push(op->op->apply(top, next));
    } else if (op->nAry == UNARY) {
        if (op->assoc == RIGHTLEFT) {
            auto top = opnds.pop();
            opnds.push(op->op->apply(top));
        } else if (op->assoc == LEFTRIGHT) {
            auto save = opnds.pop();
            auto top = opnds.pop();
            opnds.push(save);
            opnds.push(op->op->apply(top));
        }
    }
}
