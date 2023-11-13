#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define MAX_THREADS 16
#define SYSTEM_CAPACITY 12

sem_t semaphore;

void* routine(void* arg)
{
	printf("[%d thread] waiting in the login queue\n", *(int*) arg);
	//only one thread can execute the portion of code on whoch the semaphore is called at a time. 
	sem_wait(&semaphore);
	
	//Critical section starts:
	sleep(1);
	printf("[%d thread] Logged in\n", *(int*) arg);
	printf("[%d thread] Logged out\n" , *(int *) arg);
	free(arg);
	//Critical Section ends.
	
	sem_post(&semaphore);
}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	sem_init(&semaphore,0,SYSTEM_CAPACITY);
	for(int i=0;i<MAX_THREADS;i++)
	{
		int* a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&threads[i], NULL, routine, a) != 0) perror("Failed to create thread");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}

	sem_destroy(&semaphore);
	return 0; 
}
