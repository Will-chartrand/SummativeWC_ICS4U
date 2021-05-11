#include "Node.h"

Node::Node() {
    direction = 0;
    next = nullptr;
}

Node::Node(int _direction, Node *_next) {
    direction = _direction;
    next = _next;
}

Node::~Node() {
    //dtor
}

Node* Node::getNext() {
    return next;
}

int Node::getDirection() {
    return direction;
}

void Node::setNext(Node *newNext) {
    next = newNext;
    return;
}

void Node::setDirection(int newDirection) {
    direction = newDirection;
    return;
}
