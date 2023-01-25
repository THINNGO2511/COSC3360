#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <cstdlib>
#include "math.h"
using namespace std;

//PQ implementation
template <class T>
struct PNode {
    T data;
    int prio;
    PNode<T> *next;
    PNode<T> *left;
    PNode<T> *right;
};

template <class T>
class prio_queue {

public:
    PNode<T> *front;
    PNode<T> *rear;

    prio_queue(){
        front = nullptr;
        rear = nullptr;
    }

    bool isEmpty(){
        return front == nullptr;
    }

    void enqueue(T _data, int _prio) {
        PNode<T> *temp = new PNode<T>();
        temp->data = _data;
        temp->prio = _prio;

        temp->next = nullptr;

        if (isEmpty()) {
            front = temp;
            rear = temp;
        }

        else {
            PNode<T> *cu = front;
            PNode<T> *prev = nullptr;
            if (temp->prio < cu->prio || (temp->prio == cu->prio  && temp->data < cu->data)) {
                temp->next = front;
                front = temp;
            }
            else {
                while (cu != nullptr && temp->prio >= cu->prio ) {
                    prev = cu;
                    cu = cu->next;
                }
                prev->next = temp;
                temp->next = cu;
                if (temp->next == nullptr)
                    rear = temp;
            }
        }
    }
    
    //this function allows enqueueing with a node instead of only data and priority
    void enqueueNode(PNode<T> *temp){
        temp->next = nullptr;

        if (isEmpty()) {
            front = temp;
            rear = temp;
        }

        else {
            PNode<T> *cu = front;
            PNode<T> *prev = nullptr;
            if (temp->prio < cu->prio || (temp->prio == cu->prio  && temp->data < cu->data)) {
                temp->next = front;
                front = temp;
            }
            else {
                while (cu != nullptr && temp->prio >= cu->prio ) {
                    prev = cu;
                    cu = cu->next;
                }
                prev->next = temp;
                temp->next = cu;
                if (temp->next == nullptr)
                    rear = temp;
            }
        }
    }

    PNode<T>* dequeue(){
        if (!isEmpty()) {
            PNode<T> *temp = front;
            front = front->next;
            return temp;
        }
    }

    PNode<T>* getFront() {
        if (!isEmpty())
            return front;
        return nullptr;
    }

    void print() {
        PNode<T> *cu = front;
        while (cu != nullptr) {
            cout<< cu->prio<<" "<< cu->data<< endl;
            cu = cu->next;
        }
    }

    int size(){
        PNode<T> *temp = front;
        int count =0;
        while (temp != nullptr){
            count++;
            temp = temp->next;
        }
        return count;
    }

    void process(){
        while(size() > 1){
            PNode<char> *firstNode = getFront();
            dequeue();
            PNode<char> *secondNode = getFront();
            dequeue();

            PNode<T> *newNode = new PNode<T>();
            newNode->prio = firstNode->prio + secondNode->prio;
            newNode->data = '\0';
            newNode->left = firstNode;
            newNode->right = secondNode;

            // enqueue(newNode->data, newNode->prio);// need to make a new func call enqueueNode
            enqueueNode(newNode);
        }

        printTree(getFront(), " ");
    }

    void printTree(PNode<T> *root, string str ){
        if(!root){
            return;
        }

        if(root->data!='\0'){
            cout<<root->data<<": "<<str<<endl;
        }

        printTree(root->left,str+"0");
        printTree(root->right,str+"1");
    }
};

#endif