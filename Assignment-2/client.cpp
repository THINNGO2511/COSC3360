// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include "huffmanTree.h"
// using namespace std;

#include <sstream>
#include <pthread.h>
#include <iostream>
#include <utility>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>

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
    char *hostname;
    int portNo;

    string line;
    vector<pair<int, char> > list;// char with their position in the string
};

//threading function
template <class T>
void *decode(void *thread_arg){
    cout<<"hi"<<endl;
    thread_struct<T> *thread_point = (thread_struct<T> *) thread_arg;

    vector<string> posNum = getPos(thread_point->line);//posNum[0] char 11, posNum[1][2][3] 1 3 5

    int sockfd, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cout<<"ERROR opening socket"<<endl;
        exit(0);
    }

    server = gethostbyname(thread_point->hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(thread_point->portNo);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        cout<<"ERROR connecting to server"<<endl;
        exit(0);
    }

    bzero(buffer, sizeof(buffer));
    // strcpy(buffer, posNum[0].c_str());
    n = write(sockfd, posNum[0], strlen(posNum[0]));

    if(n < 0) {
        cout<<"ERROR writing to socket"<<endl;
        exit(0);
    }

    n = read(sockfd,buffer, strlen(buffer));

    if(n < 0){
        cout<<"ERROR reading from socket"<<endl;
        exit(0);
    }

    char dataChar = buffer[0];

    for(int i = 1; i < posNum.size(); i++){
        int n = stoi(posNum[i]);
        thread_point->list.push_back(make_pair(n, dataChar));
    }

    close(sockfd);
    
    return (NULL);
}

int main(int argc, char** argv){

    if(argc < 3){
        cout<<"No port provided"<<endl;
        exit(69);
    }

    char* hostname = argv[1];
    int portNo = atoi(argv[2]);
    char buffer[256];

    string inputCommand;
    int arr_size, num_threads=0;

    vector<pthread_t> threads;
    vector<thread_struct<char>*> threadsList;
    string symbolCode;
    vector< vector < int > > posvec;
    vector <string> symbolCodes;

    //getting input from command file with multiple threads, every input will have a dedicated thread
    cout<<"hi"<<endl;
    while(getline(cin, inputCommand)){
        num_threads+=1;

        //arr_size
        istringstream my_stream(inputCommand);
        int position;
        vector<int> positions;

        my_stream >> symbolCode; //get the symbol binary code
        while (my_stream >> position) //get the positions for that specific symbol in the message
        {
            arr_size++;    //message size purposes
            //cout <<"position:" << position << endl;
            positions.push_back(position);
        }
        posvec.push_back(positions);
        symbolCodes.push_back(symbolCode);

        thread_struct<char> *thread_data = new thread_struct<char>();
        thread_data->line = inputCommand;
        thread_data->hostname = hostname;
        thread_data->portNo = portNo;
        
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

