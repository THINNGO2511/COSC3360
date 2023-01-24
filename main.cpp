#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

template <class T>
struct PNode {
    T data;
    int prio;
    PNode<T> *next;
};

template <class T>
class prio_queue {
private:
    PNode<T> *front;
    PNode<T> *rear;

public:
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
            if (temp->prio < cu->prio) {
                temp->next = front;
                front = temp;
            }
            else {
                while (cu != nullptr && temp->prio >= cu->prio) {
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

    void dequeue(){
        if (!isEmpty()) {
            PNode<T> *temp = front;
            front = front->next;
            delete temp;
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
};

int main(int argc, char** argv){
    char arr[] = {'A', 'B', 'C', 'D'};
	int freq[] = { 3, 1, 3, 2 };

    int size = sizeof(arr) / sizeof(arr[0]);

    prio_queue<char> pq;

    pq.enqueue('A', 3);
    pq.enqueue('B', 1);
    pq.enqueue('C', 3);
    pq.enqueue('D', 2);

    pq.print();

    return 0;
}