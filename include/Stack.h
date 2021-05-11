#ifndef STACK_H
#define STACK_H

#include "Node.h"

class Stack {
  public:
    Stack();
    Stack(Node*, Node*);
    virtual ~Stack();

    Node *getFront();
    Node *getBack();
    void setFront(Node*);
    void setBack(Node*);
    int pop();
    int peek();
    void push(int);
    void display();
    void deleteStack();

  protected:

  private:
    Node *frunt;
    Node *bacc;
    int stackSize;
};

#endif // STACK_H
