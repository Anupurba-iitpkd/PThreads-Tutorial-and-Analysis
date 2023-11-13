#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 2

int turn, request_0=-1,request_1=-1;
int sharedVar;



void* routine(void* arg)
{
	while(1)
	{
	int index = *(int*) arg;
	if(index==0)
	{
		request_0=1;
		turn = 1;
		printf("[Thread %d] turn=%d\n",index, turn);
		while(request_1==1 && turn==1);
		sharedVar += 50;
		printf("[Thread %d] Shared variable current value = %d\n",index, sharedVar);
		request_0=0;
	}
	else
	{
		request_1 = 1;
		turn = 0;
		printf("[Thread %d] turn=%d\n",index, turn);
		while(request_0==1 && turn==0);
		sharedVar += 50;
		printf("[Thread %d] Shared variable current value = %d\n",index, sharedVar);
		request_1=0;
	}
	}	
}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	for(int i=0;i<MAX_THREADS;i++)
	{
		int* a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&threads[i], NULL, routine, (void*) a) != 0) perror("Failed to create thread");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}

	return 0; 
}
