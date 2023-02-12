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

    prio_queue(){
        front = nullptr;
        rear = nullptr;
        storageStr = "";
        storageData = "";
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

            if(temp->prio < cu->prio) {// if temp->prio is less than cu->prio, then add to front
                temp->next = front;
                front = temp;
            }
            else if(temp->prio == cu->prio){// if temp->prio is equal than cu->prio
                if(temp->data < cu->data){// if temp->data is less than cu->data, then add to front
                    temp->next = front;
                    front = temp;
                }
                else if(temp->data > cu->data){//if temp->data is more than cu->data, then traverse then add when it's appropriate
                    while(cu != nullptr && temp->data > cu->data && temp->prio == cu->prio){
                        prev = cu;
                        cu = cu->next;
                    }
                    prev->next = temp;
                    temp->next = cu;
                    if(temp->next == nullptr){ rear = temp; }
                }
            }
            else if(temp->prio > cu->prio){
                while(cu != nullptr && temp->prio > cu->prio){
                    prev = cu;
                    cu = cu->next;
                }

                if(cu == nullptr){
                    prev->next = temp;
                    temp->next = cu;
                }
                else if(temp->data < cu->data){// if temp->data is less than cu->data, then add
                    prev->next = temp;
                    temp->next = cu;
                }
                else if(temp->data > cu->data){//if temp->data is more than cu->data, then traverse then add when it's appropriate
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

            enqueueNode(newNode);
        }

        printTree(getFront(), " ");
    }

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

    void transferData(vector<string> &vect1, vector<char> &vect2){
        string s1, s2;
        stringstream ss1(storageStr);
        stringstream ss2(storageData);

        while(getline(ss1, s1, ' ')){
            vect1.push_back(s1);
        }

        for(int i = 0; i < storageData.length(); i++){
            vect2.push_back(storageData[i]);
        }
    }
};

#endif