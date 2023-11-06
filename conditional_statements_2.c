#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MAX_THREADS 5

int fuel = 0;
pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

void* fuel_filling(void* arg)
{
	printf("At %s. Filling Fuel.\n", (char*) arg);
	for(int i=0;i<5;i++)
	{
		pthread_mutex_lock(&mutexFuel);
		fuel+=15;
		printf("At %s Filled Fuel ... %d\n",(char*) arg, fuel);
		pthread_mutex_unlock(&mutexFuel);
		pthread_cond_broadcast(&condFuel);
		//the cond_signal method signals the waiting thread to proceed
		sleep(1);
	}
}

void* car(void* arg)
{
	printf("%s: Here to get fuel.\n", (char*) arg);
	pthread_mutex_lock(&mutexFuel);
	while(fuel<40)
	{
		printf("%s: Fuel < 40Litres. Waiting\n", (char*) arg);
		pthread_cond_wait(&condFuel, &mutexFuel);
		//the cond_wait function, releases the mutex locks and 
		//waits for a signal. 
		//equivalent to:
		//1. release mutex 2. wait 3. lock mutex
	}
	printf("%s: Got Fuel. Now Going\n", (char*) arg);
	fuel-=40;
	pthread_mutex_unlock(&mutexFuel);
}


int main(int argc, char* argv[])
{
	char* msg1 = "car1";
	char* msg2 = "car2";
	char* msg3 = "car3";
	char* msg4 = "car4";
	char* msg5 = "gas station";

	pthread_t threads[MAX_THREADS];
	pthread_mutex_init(&mutexFuel,NULL);
	pthread_cond_init(&condFuel,NULL);
	for (int i=0;i<MAX_THREADS;i++)
	{
		if(i==MAX_THREADS-1)
		{
			if(pthread_create(&threads[i], NULL, fuel_filling, (void*)  msg5)!= 0 ) perror("Failed to create pthread");
		} 
		if(i==0) 
		{
			if(pthread_create(&threads[i], NULL, car, (void*) msg1) != 0) perror("Failed to create thread");
		}
		if(i==1) 
		{
			if(pthread_create(&threads[i], NULL, car, (void*) msg2) != 0) perror("Failed to create thread");
		}
		if(i==2) 
		{
			if(pthread_create(&threads[i], NULL, car, (void*) msg3) != 0) perror("Failed to create thread");
		}
		if(i==3) 
		{
			if(pthread_create(&threads[i], NULL, car, (void*) msg4) != 0) perror("Failed to create thread");
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
