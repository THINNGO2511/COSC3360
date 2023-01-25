#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

int main(int argc, char** argv){

    prio_queue<char> pq;

    pq.enqueue('A', 3);
    pq.enqueue('B', 1);
    pq.enqueue('C', 3);
    pq.enqueue('D', 2);
    // pq.enqueue('\0', 6);

    // pq.print();

    pq.process();
    // pq.printTree();

    return 0;
}