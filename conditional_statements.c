#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAX_THREADS 2

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

void* fuel_filling(void* arg)
{
	printf("Filling Fuel.\n");
	for(int i=0;i<5;i++)
	{
		pthread_mutex_lock(&mutexFuel);
		fuel+=15;
		printf("Filled Fuel ... %d\n", fuel);
		pthread_mutex_unlock(&mutexFuel);
		pthread_cond_signal(&condFuel);
		//the cond_signal method signals the waiting thread to proceed
		sleep(1);
	}
}

void* car(void* arg)
{
	printf("Here to get fuel.\n");
	pthread_mutex_lock(&mutexFuel);
	while(fuel<40)
	{
		printf("Fuel < 40Litres. Waiting\n");
		pthread_cond_wait(&condFuel, &mutexFuel);
		//the cond_wait function, releases the mutex locks and 
		//waits for a signal. 
		//equivalent to:
		//1. release mutex 2. wait 3. lock mutex
	}
	printf("Got Fuel. Now Going\n");
	fuel-=40;
	pthread_mutex_unlock(&mutexFuel);
}


int main(int argc, char* argv[])
{
	pthread_t threads[MAX_THREADS];
	pthread_mutex_init(&mutexFuel,NULL);
	pthread_cond_init(&condFuel,NULL);
	for (int i=0;i<MAX_THREADS;i++)
	{
		if(i==MAX_THREADS-1)
		{
			if(pthread_create(&threads[i], NULL, fuel_filling, NULL)!= 0 ) perror("Failed to create pthread");
		} 
		else 
		{
			if(pthread_create(&threads[i], NULL, car, NULL) != 0) perror("Failed to create thread");
		}
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread");
	}

	pthread_mutex_destroy(&mutexFuel);
	pthread_cond_destroy(&condFuel);
	return 0;
}
