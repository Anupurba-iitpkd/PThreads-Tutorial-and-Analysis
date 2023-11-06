//barriers can be a good way to execute multiple threads together. 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 9

pthread_barrier_t barrier;


void* routine(void* arg)
{
	printf("%ld: waiting at the barrier\n", pthread_self());
	pthread_barrier_wait(&barrier);
	printf("%ld: Passed the barrier\n", pthread_self());

}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	pthread_barrier_init(&barrier, NULL, 3);

	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_create(&threads[i], NULL, routine, NULL) != 0) perror("Failed to create thread");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}
	pthread_barrier_destroy(&barrier);

	return 0; 
}
