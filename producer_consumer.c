#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>

#define MAX_THREADS 3

int buffer[10]; 
int count =0;

pthread_mutex_t mutex;

sem_t bufferEmpty, bufferFull;

void* producer(void* arg)
{
	while(1)
	{
		//produce
		int x = rand()%100;
		sem_wait(&bufferEmpty);
		pthread_mutex_lock(&mutex);
		buffer[count] = x;
		printf("Producer produced %d\n", x);
		count++;	
		pthread_mutex_unlock(&mutex);
		sem_post(&bufferFull);
		sleep(1);
	}
}

void* consumer(void * arg)
{
	int y;
	while(1)
	{

		//Remove from the buffer:
		sem_wait(&bufferFull);
		pthread_mutex_lock(&mutex);
		y = buffer[count-1];
		count--;	
		pthread_mutex_unlock(&mutex);
		sem_post(&bufferEmpty);
		//consume
		printf("Consumer consumed %d\n", y);
		//sleep(1);
	}
}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	char msgs[2][10] = {"thread 1", "thread 2"};
	pthread_mutex_init(&mutex, NULL);
	sem_init(&bufferEmpty, 0, 10);
	sem_init(&bufferFull, 0, 0);
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(i==2)
		{
			if(pthread_create(&threads[i], NULL, producer, (void*) msgs[i]) != 0) perror("Failed to create thread");
		}
		else
		{
			if(pthread_create(&threads[i], NULL, consumer , (void*) msgs[i]) != 0) perror("Failed to create thread");
		}
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}
	pthread_mutex_destroy(&mutex);
	sem_destroy(&bufferEmpty);
	sem_destroy(&bufferFull); 
	return 0; 
}
