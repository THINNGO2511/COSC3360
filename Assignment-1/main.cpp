#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

void printTree(PNode<char> *root, string str ){
    if(!root){
        return;
    }

    if(root->data!='\0'){
        cout<<root->data<<": "<<str<<endl;
    }

    printTree(root->left,str+"0");
    printTree(root->right,str+"1");
}

void process(prio_queue<char>& pq){
    while(pq.size() > 1){
        PNode<char> *firstNode = pq.getFront();
        // cout<<pq.getFront()->data<<pq.getFront()->prio<<endl;
        pq.dequeue();
        PNode<char> *secondNode = pq.getFront();
        // cout<<pq.getFront()->data<<pq.getFront()->prio<<endl;
        pq.dequeue();

        // cout<<pq.getFront()->data<<pq.getFront()->prio<<endl;
        PNode<char> *result = pq.CombineNode(firstNode, secondNode);
        pq.enqueue(result->data, result->prio);
    }

    printTree(pq.getFront(), " ");
}

int main(int argc, char** argv){
    char arr[] = {'A', 'B', 'C', 'D'};
	int freq[] = { 3, 1, 3, 2 };

    int size = sizeof(arr) / sizeof(arr[0]);

    prio_queue<char> pq;

    pq.enqueue('A', 3);
    pq.enqueue('B', 1);
    pq.enqueue('C', 3);
    pq.enqueue('D', 2);
    // pq.enqueue('\0', 6);

    // pq.print();
    // cout<<pq.size()<<endl;
    process(pq);

    return 0;
}