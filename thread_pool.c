#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 2

typedef struct Task{
	int a, b;
}Task;

Task taskQueue[256];

int taskCount=0;

pthread_mutex_t mutexTaskQueue;
pthread_cond_t condQueue;

void executeTask(Task* task)
{	
	int result = task->a + task->b;
	printf("The sum of %d and %d is =%d\n", task->a, task->b, result);

}

void submitTask(Task task)
{
	pthread_mutex_lock(&mutexTaskQueue);
	taskQueue[taskCount] = task;
	taskCount++;
	pthread_mutex_unlock(&mutexTaskQueue);
	pthread_cond_signal(&condQueue);
}

void* startThread(void* arg)
{
	while(1)
	{
		Task task;

		pthread_mutex_lock(&mutexTaskQueue);

		while(taskCount == 0)
		{
			pthread_cond_wait(&condQueue, &mutexTaskQueue);
		}

		found=1;
		task = taskQueue[0];
		for(int i=0;i<taskCount;i++)
		{
			taskQueue[i] = taskQueue[i+1];
		}
		taskCount--;
		pthread_mutex_unlock(&mutexTaskQueue);

		executeTask(&task);
	}

}

int main(int* argc,char* argv[])
{
	pthread_t threads[MAX_THREADS];
	pthread_mutex_init(&mutexTaskQueue, NULL);
	pthread_cond_init(&condQueue, NULL);
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_create(&threads[i], NULL, startThread, NULL) != 0) perror("Failed to create thread");
	}

	for(int i=0;i<100;i++)
	{
		Task t = {
			.a = rand() % 100,
			.b = rand() % 100
		};
		submitTask(t);

	}

	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Failed to terminate thread.");
	}
	pthread_mutex_destroy(&mutexTaskQueue);
	pthread_cond_destroy(&condQueue);
	return 0; 
}
