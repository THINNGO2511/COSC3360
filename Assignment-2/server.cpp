// #include <iostream>
// #include <thread>
// #include <sstream>
// #include <string>

// #include "huffmanTree.h"
// #include <vector>
// #include <sys/wait.h> 

// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <iostream>

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

#include "huffmanTree.h"

using namespace std;

void fireman(int) 
{     
    while(waitpid(-1, NULL, WNOHANG) > 0){        
        break;    
    } 
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
    if(argc < 2){
        cout<<"Error no port provided"<<endl;
        exit(69);
    }

    struct sockaddr_in serv_addr, cli_addr;
    int sockfd, newsockfd, portno, clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout<<"socket error"<<endl;
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // convert port number to network byte order

    if (::bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        cout<<"bind error"<<endl;
        exit(1);
    }
    
    clilen = sizeof(cli_addr);

    string inputString;
    int repetition, arr_size=0;

    //getting input from input file and push into huffmanTree
    prio_queue<char> pq;
    int lineNo = 0;
    while(getline(cin, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
        lineNo++;
    }

    listen(sockfd, lineNo);

    //processing tree and print it out
    pq.process();

    signal(SIGCHLD, fireman);
    while(true){
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

        if (newsockfd < 0){
            cout<<"socket accept error"<<endl;
            exit(1);
        }

        if(fork() == 0){
            int n;
            char buffer[256];
            bzero(buffer, 256);
            n = read(newsockfd, buffer, sizeof(buffer));

            if (n < 0){
                cout<<"ERROR reading from socket"<<endl;
                exit(1);
            }

            string s = string(buffer);
            char c = getChar(pq.getFront(), s, 0);
            
            bzero(buffer, 256);
            buffer[0] = c;

            n = write(newsockfd, buffer, 256);
            if(n < 0){
                cout<<"ERROR writing to socket"<<endl;
                exit(1);
            }
            close(newsockfd);
            _exit(0);
        }
    }

    close(sockfd);
    return 0;
}

