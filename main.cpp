#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "operators.h"
#include "stack.h"

const int MAX_STACK_SIZE = 1000;

//todo currently pops more off stack than exist

std::unordered_map<char, CompleteOperator> operators = {
    {'+', CompleteOperator{op_ptr(new Plus), 1, LEFTRIGHT, BINARY}},
    {'-', CompleteOperator{op_ptr(new Sub), 1, LEFTRIGHT, BINARY}},
    {'/', CompleteOperator{op_ptr(new Div), 10, LEFTRIGHT, BINARY}},
    {'*', CompleteOperator{op_ptr(new Mul), 10, LEFTRIGHT, BINARY}},
    {'^', CompleteOperator{op_ptr(new Exp), 100, RIGHTLEFT, BINARY}},
    {'~', CompleteOperator{op_ptr(new Neg), 10, RIGHTLEFT, UNARY}},
    {'(', CompleteOperator{op_ptr(new LeftParen), -1, RIGHTLEFT, UNARY}}};

enum LastRead { OPERATOR, OPERAND };

void applyOperation(cop_ptr&, Stack<cop_ptr, MAX_STACK_SIZE>&,
                    Stack<double, MAX_STACK_SIZE>&);

int main() {
    Stack<cop_ptr, MAX_STACK_SIZE> optrs;
    Stack<double, MAX_STACK_SIZE> opnds;
    LastRead lastRead;

    auto test = std::string("1+2*3+1");

    for (auto ch : test) {
        if (isdigit(ch)) {
            opnds.push(ch - '0');
            lastRead = OPERAND;
        } else if (ch == ' ') {
            continue;
        } else if (ch == '(') {
            continue;
        } else if (ch == ')') {
            auto op = optrs.pop();
            while (op->precedence != -1) {
                applyOperation(op, optrs, opnds);
            }
        } else {
            cop_ptr op;
            if (ch == '-' && lastRead == OPERATOR) {
                op = cop_ptr(&operators['~']);
            } else {
                op = cop_ptr(&operators[ch]);
            }
            if (!optrs.isEmpty()) {
                auto stackTop = optrs.pop();
                optrs.push(stackTop);
            }
            while (!optrs.isEmpty() &&
                   optrs.peek()->precedence >= op->precedence) {// &&
                   //(op->assoc != optrs.peek()->assoc != RIGHTLEFT)) {
                applyOperation(op, optrs, opnds);
            }
            optrs.push(op);
        }
    }
    while (!optrs.isEmpty()) {
        auto op = optrs.pop();
        applyOperation(op, optrs, opnds);
    }
    std::cout << "result: " << opnds.pop() << std::endl;
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
