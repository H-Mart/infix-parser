#ifndef STACK_H
#define STACK_H

#include <iostream>

template <typename T, unsigned int SIZE>
class Stack {
   public:
    using Item = T;
    int maxSize = SIZE;
    Stack() : top(0) {}
    void push(Item value) { data[top++] = value; }
    Item pop() { return data[--top]; }
    bool isEmpty() { return !top; }
    Item peek() { return data[top-1]; }
    friend std::ostream& operator<<(std::ostream& o, Stack& s) {
        while (!s.isEmpty()) {
            o << s.pop() << ' ';
        }
        return o;
    }

   private:
    Item data[SIZE];
    int top;
};

#endif