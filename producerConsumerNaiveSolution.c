#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAX_THREADS 2

int fuel = 0;
pthread_mutex_t mutexFuel;

void* fuel_filling(void* arg)
{
	printf("Filling Fuel.\n");
	for(int i=0;i<5;i++)
	{
		pthread_mutex_lock(&mutexFuel);
		fuel+=15;
		printf("Filled Fuel ... %d\n", fuel);
		pthread_mutex_unlock(&mutexFuel);
		sleep(1);
	}
}



/*
void* car(void* arg)
{
	printf("Here to get fuel.\n");
	pthread_mutex_lock(&mutexFuel);
	if(fuel >= 40)	
	{
		fuel -= 40;
		printf("Fuel now left: %d\n", fuel);
	}
	else
	{
		printf("No fuel. Waiting\n");
	}
	pthread_mutex_unlock(&mutexFuel);
}*/

void* car(void* arg)
{
	printf("Here to get fuel.\n");
	pthread_mutex_lock(&mutexFuel);
	while(fuel<40)
	{
		printf("Fuel < 40Litres. Waiting\n");
		pthread_mutex_unlock(&mutexFuel);
		sleep(1);
	}
	fuel-=40;
	pthread_mutex_unlock(&mutexFuel);
}


int main(int argc, char* argv[])
{
	pthread_t threads[MAX_THREADS];
	pthread_mutex_init(&mutexFuel, NULL);

	for (int i=0;i<MAX_THREADS;i++)
	{
		if(i==1)
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
	return 0;
}
