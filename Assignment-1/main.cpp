#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

int main(int argc, char** argv){
    string fileName, commandName;
    string inputString;
    int repetition;

    cin>>fileName;
    cin>>commandName;

    ifstream inFileName(fileName);
    ifstream inCommandName(commandName);

    prio_queue<char> pq;
    while(getline(inFileName, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
    }

    // pq.process();
    pq.print();

    return 0;
}
