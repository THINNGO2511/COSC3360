#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

//getting position from string
vector<string> getPos(string input){
    vector<string> result;
    string s;
    stringstream ss(input);
    while(getline(ss, s, ' ')){
        result.push_back(s);
    }
    return result; 
}
//struct for threading
template <class T>
struct thread_struct {
    PNode<T> *root;
    string line;
    vector<pair<int, char> > list;// char with their position in the string
};

//threading function
template <class T>
void *decode(void *thread_arg){
    thread_struct<T> *thread_point = (thread_struct<T> *) thread_arg;

    vector<string> posNum = getPos(thread_point->line);
    char dataChar = getChar(thread_point->root, posNum[0], 0);

    for(int i = 1; i < posNum.size(); i++){
        int n = stoi(posNum[i]);
        thread_point->list.push_back(make_pair(n, dataChar));
    }
    return (NULL);
}

//getting character from path (say 111 is going right 3 times from root)
template <class T>
char getChar(PNode<T> *node, string path, int index) {
    if (path.empty()) {
        return '\0';
    }
    if (index == path.length()) {
        return node->data;
    }
    if (path[index] == '0') {
        return getChar(node->left, path, index + 1);
    }
    else if (path[index] == '1') {
        return getChar(node->right, path, index + 1);
    }
    return '\0';
}

int main(int argc, char** argv){
    string fileName, commandName;
    string inputString, inputCommand;
    int repetition, arr_size=0, num_threads=0;

    //getting input from files
    cin>>fileName;
    cin>>commandName;

    ifstream inFileName(fileName);
    ifstream inCommandName(commandName);

    //getting input from input file and push into huffmanTree
    prio_queue<char> pq;
    while(getline(inFileName, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
        arr_size += stoi(inputString.substr(2, inputString.size() - 1));// getting original message size
    }

    //processing tree and print it out
    pq.process();

    vector<pthread_t> threads;
    vector<thread_struct<char>*> threadsList;

    //getting input from command file with multiple threads, every input will have a dedicated thread
    while(getline(inCommandName, inputCommand)){
        num_threads+=1;
        thread_struct<char> *thread_data = new thread_struct<char>();
        thread_data->root = pq.getFront();
        thread_data->line = inputCommand;
        
        pthread_t thread;
        pthread_create(&thread, NULL, &decode<char>, (void*)thread_data);
        threads.push_back(thread);
        threadsList.push_back(thread_data);
    }

    //joining threads
    for (int i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    char arr[arr_size];// array of characters to print the original message

    //assigning char for original message
    for (int i = 0; i < threadsList.size(); i++) {
        for (int j = 0; j < threadsList[i]->list.size(); j++) {
            int pos = threadsList[i]->list[j].first;
            char data = threadsList[i]->list[j].second;
            arr[threadsList[i]->list[j].first] = threadsList[i]->list[j].second;
        }
    }

    //printing out the original message
    cout<<"Original message: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i];
    }
    return 0;
}

