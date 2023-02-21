#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <sstream>
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
    string storageStr, storageData;

    //initializing huffmanTree
    prio_queue(){
        front = nullptr;
        rear = nullptr;
        storageStr = "";
        storageData = "";
    }

    //check if tree is empty
    bool isEmpty(){
        return front == nullptr;
    }
    
    //enqueue information to the tree by creating a new node with provided data
    void enqueue(T _data, int _prio) {
        PNode<T> *temp = new PNode<T>();
        temp->data = _data;
        temp->prio = _prio;

        temp->next = nullptr;

        if (isEmpty()) {// if tree is empty, then this will be the root of the tree
            front = temp;
            rear = temp;
        }

        else {//otherwise...
            PNode<T> *cu = front;
            PNode<T> *prev = nullptr;

            if(temp->prio < cu->prio) {// if temp->prio is less than cu->prio, then add to front
                temp->next = front;
                front = temp;
            }
            else if(temp->prio == cu->prio){// if temp->prio is equal than cu->prio
                if(temp->data < cu->data){// if temp->data is less than cu->data, then add to front
                    temp->next = front;
                    front = temp;
                }
                else if(temp->data > cu->data){//if priority is the same and temp->data is more than cu->data, then traverse then add when it's appropriate
                    while(cu != nullptr && temp->data > cu->data && temp->prio == cu->prio){
                        prev = cu;
                        cu = cu->next;
                    }
                    prev->next = temp;
                    temp->next = cu;
                    if(temp->next == nullptr){ rear = temp; }
                }
            }
            else if(temp->prio > cu->prio){//if temp->prio is greater than cu->prio, then traverse until it's at least equal to cu->prio.
                while(cu != nullptr && temp->prio > cu->prio){
                    prev = cu;
                    cu = cu->next;
                }
                if(cu == nullptr){//if it's the end of tree, then add.
                    prev->next = temp;
                    temp->next = cu;
                }
                else if(temp->data < cu->data){// if temp->data is less than cu->data, then add
                    prev->next = temp;
                    temp->next = cu;
                }
                else if(temp->data > cu->data){//if temp->data is more than cu->data, then continue to traverse then add when it's appropriate
                    while(cu != nullptr && temp->data > cu->data && temp->prio == cu->prio){
                        prev = cu;
                        cu = cu->next;
                    }
                    prev->next = temp;
                    temp->next = cu;
                    if(temp->next == nullptr){ rear = temp; }
                }
            }
        }
    }
    
    //this function allows enqueueing with a node instead of only data and priority
    //it's only used for combining two nodes together
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
                    while (cu != nullptr && temp->prio > cu->prio ) {// took out = of temp>prio
                        prev = cu;
                        cu = cu->next;
                    }
                    prev->next = temp;
                    temp->next = cu;
                    if (temp->next == nullptr) {rear = temp;}
            }
        }
    } 

    //removing node
    PNode<T>* dequeue(){
        if (!isEmpty()) {
            PNode<T> *temp = front;
            front = front->next;
            return temp;
        }
    }

    //returning the root of the tree
    PNode<T>* getFront() {
        if (!isEmpty())
            return front;
        return nullptr;
    }

    //printing out the queue
    void print() {
        PNode<T> *cu = front;
        while (cu != nullptr) {
            cout<< cu->prio<<" "<< cu->data<< endl;
            cu = cu->next;
        }
    }

    //getting priority queue size
    int size(){
        PNode<T> *temp = front;
        int count =0;
        while (temp != nullptr){
            count++;
            temp = temp->next;
        }
        return count;
    }

    // this function pops out first two nodes from the queue and combine them together, then enqueue them again
    //then print them out
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

            enqueueNode(newNode);
        }

        printTree(getFront(), " ");
    } 

    //printing out the entire tree
    void printTree(PNode<T> *root, string str){
        if(!root){
            return;
        }

        if(root->data!='\0'){
            cout<<"Symbol: "<<root->data<<", Frequency: "<<root->prio<<", Code: "<<str<<endl;
            storageStr+=str;
            storageData+=root->data;
        }

        printTree(root->left,str+"0");
        printTree(root->right,str+"1");
    }
};

#endif