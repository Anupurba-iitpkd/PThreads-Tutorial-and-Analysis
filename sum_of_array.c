#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 20
#define MAX_THREADS 4

int arr[N] = {2,3,5,7,11,13,19,23,29,1,2,3,4,5,6,7,8,9,10,11};

void* subRoutine(void* arg)
{
	int startIndex = *(int*)arg;
	int subPartLength = (int) N/MAX_THREADS;
	int sum = 0;
	for(int i=0;i<subPartLength;i++)
	{
		sum = sum+arr[startIndex+i];
	}
	//using arg to return value to the thread_join method:
	*(int* )arg = sum; 
	printf("local sum=%d\n", sum);
	return arg;
}

int main(int argc, char* argv)
{
	pthread_t threads[MAX_THREADS];
	int subPartLength = (int) N/MAX_THREADS;
	int globalSum=0; 
	for(int i=0;i<MAX_THREADS;i++)
	{
		int *a = malloc(sizeof(int));
		*a = i*subPartLength;
		if(pthread_create(&threads[i], NULL, subRoutine, a) != 0) perror("Failed to create thread properly.");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		int* sum;
		if(pthread_join(threads[i],(void** ) &sum) != 0) perror("Failed to terminate thread properly.");
		globalSum += *(int*)sum;
		free(sum);
	}
	printf("Sum of elements=%d\n", globalSum);
	return 0;
}
