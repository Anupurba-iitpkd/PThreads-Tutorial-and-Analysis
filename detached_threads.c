///////////////////////////////Detached Threads////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 2

void* routine(void* arg)
{
	sleep(1);
	printf("Execution Completed.\n");
}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	char msgs[2][10] = {"thread 1", "thread 2"};
	pthread_attr_t detachedThread; 
	pthread_attr_init(&detachedThread);
	pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);
	for(int i=0;i<MAX_THREADS;i++)
	{
		//First way to create detached threads:
		//we are passing &detachedThread attribute, in which, all the other attribute fields are set to their default initial state. 
		////but the attribute for detached thread is initialized to PTHREAD_CREATE_DETACHED.
		if(pthread_create(&threads[i], &detachedThread, routine, (void*) msgs[i]) != 0) perror("Failed to create thread");

		//2nd way to create detached threads. 
		//this creates a thread normally first and then sets it's detached state. 
//		pthread_detach(threads[i]);

		//2nd way is not a good way. 
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}
	pthread_exit(0);	
}
