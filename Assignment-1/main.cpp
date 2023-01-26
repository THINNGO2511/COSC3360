#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

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

    pq.process();

    return 0;
}