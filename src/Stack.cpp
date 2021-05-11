#include "Stack.h"

Stack::Stack() {
    //ctor
}

Stack::Stack(Node *_frunt, Node *_bacc) {
    frunt = _frunt;
    bacc = _bacc;
}

Stack::~Stack() {
    //dtor
}

int Stack::pop() {
    if (frunt != nullptr) {
        Node *frontNode = frunt;
        if (frunt == bacc) {
            bacc = nullptr;
        }
        frunt = frunt->getNext();
        frontNode->setNext(nullptr);
        int returnValue = frontNode->getDirection();
        delete frontNode;

        return returnValue;
    }
    return -2;
}

int Stack::peek() {
    if (frunt != nullptr) {
        return frunt->getDirection();
    }
    return -1;
}

void Stack::push(int _value) {
    Node *newNode = new Node(_value, nullptr);
    if (frunt == nullptr) {
        frunt = newNode;
        bacc = newNode;
    } else {
        newNode->setNext(frunt);
        frunt = newNode;
    }
    return;
}

Node* Stack::getFront() {
    return frunt;
}

Node* Stack::getBack() {
    return bacc;
}

void Stack::setFront(Node *newFront) {
    frunt = newFront;
    return;
}

void Stack::setBack(Node *newBack) {
    bacc = newBack;
    return;
}

void Stack::display() {
    Node *ptr_iterator = frunt;
    while(ptr_iterator != nullptr) {
        std::cout << ptr_iterator->getDirection()<< std::endl;
        ptr_iterator = ptr_iterator->getNext();
    }
    return;
}

void Stack::deleteStack() {
    if(frunt != nullptr) {
        Node *current = frunt;
        while(frunt->getNext() != nullptr) {
            current = frunt->getNext();
            delete frunt;
            frunt = current;
        }
    }
    return;
}
