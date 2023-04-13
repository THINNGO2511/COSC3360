#include <pthread.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cmath>

static pthread_mutex_t bsem;    // Mutex semaphore
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;  // Condition variable to control the turn
static int turn; // Index to control access to the turn array
static int maxEvenThread;

void *printEvenOdd(void *void_ptr_argv)
{
    //  std::cout << "I am child thread " << /*variable name*/ << std::endl;
    int threadNum = * (int *) void_ptr_argv;
    pthread_mutex_lock(&bsem);
    while(threadNum != turn){
        pthread_cond_wait(&waitTurn, &bsem);
    }
    pthread_mutex_unlock(&bsem);
    std::cout<<"I am Thread"<< threadNum<<std::endl;
    pthread_mutex_lock(&bsem);
    turn = turn + 2;
    pthread_mutex_unlock(&bsem);
    if(turn >= maxEvenThread){
        turn=1;
    }
    pthread_cond_broadcast(&waitTurn);
    return nullptr;
}

int main()
{
    int nThreads;
    std::cin >> nThreads;   // number of Threads from STDIN
    
    pthread_mutex_init(&bsem, NULL); // Initialize bsem to 1
 	
 	pthread_t *tid= new pthread_t[nThreads];
	int *threadNumber = new int[nThreads];
	turn = 0;// initialize the turn here
	
	// Determine the max thread number for even threads
	if ((nThreads - 1)%2 == 0)
	    maxEvenThread = nThreads - 1;
	else
	    maxEvenThread = nThreads - 2;
	   //maxEvenThread = nThreads;
	for(int i=0;i<nThreads;i++)
	{
        threadNumber[i] = i;// initialize the thread number here (remember to follow the rules from the specifications of the assignment)
        // call pthread_create here
        pthread_create(&tid[i], NULL, printEvenOdd, &threadNumber[i]);
	}
	// Call pthread_join
	for (int i = 0; i < nThreads; i++){
        pthread_join(tid[i], NULL);
	}
    delete [] threadNumber;
    delete [] tid;
	return 0;
}