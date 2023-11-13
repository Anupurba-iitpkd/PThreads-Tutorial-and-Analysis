//barriers can be a good way to execute multiple threads together. 
//Problem Statement:
//Every thread rolls a dice and saves its value in an array. 
//The main thread calculates the winner then. 
//Each thread prints a message saying whether the won or lost. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 8

int diceValues[8] = {0,0,0,0,0,0,0,0};
int status[8] = {0};
pthread_barrier_t barrierRollDice,barrierCalculateWinner;

void* rollDice(void* args)
{
	int index = *(int* )args;
	diceValues[index] = rand()%6+1;
	pthread_barrier_wait(&barrierRollDice);
	pthread_barrier_wait(&barrierCalculateWinner);
	if(status[index] == 1) printf("[%d Rolled to %d] I won\n", index, diceValues[index]);
	else printf("[%d rolled to %d] I lost\n", index, diceValues[index]);
}


void* routine(void* arg)
{
	printf("%ld: waiting at the barrier\n", pthread_self());
	pthread_barrier_wait(&barrierRollDice);
	printf("%ld: Passed the barrier\n", pthread_self());

}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	int max = 0;
	pthread_barrier_init(&barrierRollDice, NULL, MAX_THREADS+1);
	pthread_barrier_init(&barrierCalculateWinner, NULL, MAX_THREADS+1);
	for(int i=0;i<MAX_THREADS;i++)
	{
		int* a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&threads[i], NULL, rollDice, a) != 0) perror("Failed to create thread");
	}


	pthread_barrier_wait(&barrierRollDice);

	//calculating the winner:
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(diceValues[i]>max) max=diceValues[i];
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(diceValues[i] == max) status[i] = 1;
		else status[i] = 0;
	}

	pthread_barrier_wait(&barrierCalculateWinner);
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}
	pthread_barrier_destroy(&barrierCalculateWinner);

	return 0; 
}
