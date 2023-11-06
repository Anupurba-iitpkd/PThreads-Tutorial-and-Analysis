#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 2

void* routine(void* arg)
{

}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	char msgs[2][10] = {"thread 1", "thread 2"};
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_create(&threads[i], NULL, routine, (void*) msgs[i]) != 0) perror("Failed to create thread");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}

	return 0; 
}
