#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <cmath>
#include <algorithm>

#include "huffmanTree.h"
using namespace std;

static pthread_mutex_t bsem; 
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;  // Condition variable to control the turn

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
    int turn;
    vector<pair<int, char> > list;// char with their position in the string
};

//threading function
template <class T>
void *decode(void *thread_arg){
    thread_struct<T> *thread_point = (thread_struct<T> *) thread_arg;

    vector<string> posNum = getPos(thread_point->line);
    char dataChar = getChar(thread_point->root, posNum[0], 0);
    int Freq = getFreq(thread_point->root, posNum[0],0);

    cout<<"Symbol: "<<dataChar<<", Frequency: "<<Freq<<", Code: "<<posNum[0]<<endl;
    pthread_mutex_unlock(&bsem);

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

template <class T>
int getFreq(PNode<T> *node, string path, int index) {
    if (path.empty()) {
        return 0;
    }
    if (index == path.length()) {
        return node->prio;
    }
    if (path[index] == '0') {
        return getFreq(node->left, path, index + 1);
    }
    else if (path[index] == '1') {
        return getFreq(node->right, path, index + 1);
    }
    return 0;
}

int main(int argc, char** argv){
    string fileName, commandName;
    string inputString, inputCommand;
    int repetition, arr_size=0, num_threads=0;

    //initialize bsem to 1
    pthread_mutex_init(&bsem, NULL);

    //getting first size number input from files
    string size_num;
    cin >> size_num;
    cin.ignore();

    //number version of size_num
    int num = stoi(size_num);

    //n for while loop increment
    int n=0;

    //getting input from input file and push into huffmanTree
    prio_queue<char> pq;
    while(getline(cin, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
        arr_size += stoi(inputString.substr(2, inputString.size() - 1));// getting original message size
        
        if(n == num-1){
            break;
        }
        n++;
    }

    //processing tree
    pq.process();

    //vectors for containing threads
    vector<pthread_t> threads;
    vector<thread_struct<char>*> threadsList;

    //reset n
    n=0;

    //create turn;
    int turn = 0;

    //getting input from command file with multiple threads, every input will have a dedicated thread
    while(getline(cin, inputCommand)){
        num_threads+=1;

        thread_struct<char> *thread_data = new thread_struct<char>();
        thread_data->root = pq.getFront();
        thread_data->line = inputCommand;
        thread_data->turn = turn;
        
        pthread_t thread;

        pthread_mutex_lock(&bsem);
        
        pthread_create(&thread, NULL, &decode<char>, (void*)thread_data);
        threads.push_back(thread);
        threadsList.push_back(thread_data);

        if(n == num-1){
            break;
        }
        n++;
    }

    //joining threads
    for (int i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    //pushing back the characters into pair vector so sorting is easier and more flexible
    vector< pair<int, char> > arr;
    for (int i = 0; i < threadsList.size(); i++) {
        for (int j = 0; j < threadsList[i]->list.size(); j++) {
            arr.push_back(make_pair(threadsList[i]->list[j].first,threadsList[i]->list[j].second));
        }
    }

    sort(arr.begin(), arr.end());

    //printing out the original message
    cout<<"Original message: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i].second;
    }

    return 0;
}
