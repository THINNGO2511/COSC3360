#include <sstream>
#include <pthread.h>
#include <thread>
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

struct thread_struct{
    string line; // this string will take command line inputs and serves as input for getPos()
    vector< pair< int, char > > list; // this vector has < position, char > pairs to store characters and their positions
    char *hostname; // hostname of the server
    int portNo; // port of the server
};

void *decode(void * thread_arg){
    thread_struct * thread_point = (thread_struct *)thread_arg;

    vector< string> posNum = getPos(thread_point->line);//posNum[0] code: 11, posNum[1][2][3] = 1 3 5
    char dataChar;// 1 thread has 1 character with multiple positions

    //open socket for sending binary code and receiving char from server
    int sockfd, n;
    char buffer[256];

    struct sockaddr_in serv_addr;
    struct hostent *server;

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

    bzero(buffer, sizeof(buffer));//writing to server
    string what = posNum[0];
    strcpy(buffer, what.c_str());
    n = write(sockfd, buffer, sizeof(buffer));

    if(n < 0) {
        cout<<"ERROR writing to socket"<<endl;
        exit(0);
    }

    n = read(sockfd,buffer, strlen(buffer));

    if(n < 0){
        cout<<"ERROR reading from socket"<<endl;
        exit(0);
    }

    dataChar = buffer[0];

    for(int i = 1; i < posNum.size(); i++){
        int n = stoi(posNum[i]);
        thread_point->list.push_back(make_pair(n, dataChar));
    }

    close(sockfd);//close the socket
    return(NULL);
}

int main(int argc, char** argv){
    if(argc < 3){
        cout<<"ERROR, no port provided"<<endl;
        exit(1);
    }

    char* hostname = argv[1];
    int portNo = atoi(argv[2]);

    string inputCommand, symbolCode;
    int num_threads=0;
    vector<string> posNum;
    vector< vector <string> > posList;

    vector<pthread_t> threads;
    vector<thread_struct *> threadsList;

    //getting input from command file with multiple threads, every input will have a dedicated thread
    while(getline(cin, inputCommand)){
        num_threads+=1;

        //creating threads
        thread_struct * thread_data = new thread_struct();
        thread_data->line = inputCommand;
        thread_data->portNo = portNo;
        // cout<<portNo<<endl;
        thread_data->hostname = hostname;

        pthread_t thread;
        pthread_create(&thread, NULL, &decode, (void*)thread_data);
        threads.push_back(thread);
        threadsList.push_back(thread_data);
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
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i].second;
    }

    return 0;
}

