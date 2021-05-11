#ifndef NODE_H
#define NODE_H

#include <iostream>

class Node {
  public:
    Node();
    Node(int, Node*);
    virtual ~Node();

    Node* getNext();
    int getDirection();
    void setNext(Node*);
    void setDirection(int);

  protected:

  private:
    Node *next;
    int direction;
};

#endif // NODE_H
