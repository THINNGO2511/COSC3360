#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "huffmanTree.h"
using namespace std;

vector<string> getPos(string input){
    vector<string> result;
    string s;
    stringstream ss(input);
    while(getline(ss, s, ' ')){
        result.push_back(s);
    }
    return result; 
}
template <class T>
struct thread_struct {
    PNode<T> *root;
    string line;
    string decodedString; // add this to store decoded string
};

template <class T>
void *decode(void *thread_arg){
    thread_struct<T> *thread_point = (thread_struct<T> *) thread_arg;

    vector<string> posNum = getPos(thread_point->line);
    char dataChar = getChar(thread_point->root, posNum[0], 0);

    // add decoded character to the string
    thread_point->decodedString += dataChar; 

    for(int i = 1; i < posNum.size(); i++){
        int n = stoi(posNum[i]);
        char dataChar = getChar(thread_point->root, posNum[i], -1);

        // add decoded character to the string
        thread_point->decodedString += dataChar; 
    }
    return (NULL);
}

template <class T>
char getChar(PNode<T> *node, string path, int index) {
    if(index == path.length() - 1)
        cout<<node->data<< node->prio << endl;
        return node->data;

    if(path[index] == '0')
        return getChar(node->left, path, index + 1);
    else
        return getChar(node->right, path, index + 1);
}

int main(int argc, char** argv){
    string fileName, commandName;
    string inputString, inputCommand;
    int repetition, arr_size=0, num_threads=0;

    cin>>fileName;
    cin>>commandName;

    ifstream inFileName(fileName);
    ifstream inCommandName(commandName);

    prio_queue<char> pq;
    while(getline(inFileName, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
        arr_size += stoi(inputString.substr(2, inputString.size() - 1));// getting original message size
    }

    vector<string> vect1;
    vector<char> vect2;
    vector<pair<string, char> >vect_combined;

    pq.process();
    // pq.transferData(vect1, vect2);
    // vect1.erase(vect1.begin());

    // for(int i = 0; i < vect1.size(); i++){
    //     vect_combined.push_back(make_pair(vect1[i], vect2[i]));
    // }

    vector<pthread_t> threads;
    vector<thread_struct<char>*> threadsList;

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

    for (int i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}