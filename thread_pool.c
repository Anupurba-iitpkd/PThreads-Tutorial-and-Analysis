#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_THREADS 2

typedef struct Task{
	void (*taskFunction)(int, int);
	int arg1,arg2;
}Task;

Task taskQueue[256];

int taskCount=0;

pthread_mutex_t mutexTaskQueue;
pthread_cond_t condQueue;

void executeTask(Task* task)
{
	task->taskFunction(task->arg1, task->arg2);	
//	int result = task->a + task->b;
//	printf("The sum of %d and %d is =%d\n", task->a, task->b, result);

}

void sumAndProduct(int a, int b)
{
	int sum = a+b;
	int prod = a*b;
	printf("Sum and product of %d and %d is %d and %d respectively.\n",
			a,b,sum,prod);
}

void diffAndDiv(int a, int b)
{
	int diff = abs(a-b);
	int div = 0;
	if(a==0 || b==0) div = 0; 
	else div =  (int) a/b;
	printf("Difference and division of %d and %d is %d and %d respectively.\n", a,b,diff,div);
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
			.taskFunction = i%2==0?sumAndProduct:&diffAndDiv,
			.arg1 = rand()%100,
			.arg2 = rand()%200
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
