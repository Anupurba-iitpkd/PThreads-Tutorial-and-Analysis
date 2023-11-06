#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

//chefs: threads
//stove: shared data

#define MAX_THREADS 10
#define MAX_PRODUCERS 4

pthread_mutex_t mutexStove[MAX_PRODUCERS];
pthread_mutex_t mutex;
int stoveFuel[MAX_PRODUCERS] = {100,100,100,100};

//function explaining the work done by a mutex lock. 
void* routine(void* arg){
	char* msg = (char *)arg;
	pthread_mutex_lock(&mutex);
	printf("%s: Acquired Lock\n",msg);
	sleep(1);
	pthread_mutex_unlock(&mutex);

}

//function explaining the work done by a mutex try lock. 
void* subRoutine(void* arg)
{
	char* msg = (char*) arg;
	pthread_mutex_trylock(&mutex);
	printf("%s: Got Lock\n",msg);
	sleep(1);
	pthread_mutex_unlock(&mutex);
}

void* cooking(void* arg)
{
	char* msg = (char*) arg;
	for(int i=0;i<MAX_PRODUCERS;i++)
	{
		if(pthread_mutex_trylock(&mutexStove[i]) == 0 )
		{
			int fuelNeeded = (rand() % 20)+1;
			if(stoveFuel[i]-fuelNeeded<=0) printf("%s: Not enough fuel...Fuel Needed=%d Going home\n", msg, fuelNeeded);
			else
			{
				stoveFuel[i] -= fuelNeeded;
				usleep(500000);
				printf("%s: Stove used= %d Used fuel= %d, Fuel left= %d\n", msg, i, fuelNeeded, stoveFuel[i]);
			}
			pthread_mutex_unlock(&mutexStove[i]);
			break;	
		}
		else
		{
			if(i==3) 
			{	
				printf("%s: No Stove available now... Waiting\n", msg);
				usleep(3000000);
				i=-1;
			}
		}
	
	}


}


int main(int argc,char* argv)
{
	char msgs[10][9] = {"thread 1", "thread 2", "thread 3", "thread 4",
	"thread 5", "thread 6", "thread 7", "thread 8", "thread 9", "thread 0"};

	pthread_t threads[MAX_THREADS];

	for(int i=0;i<MAX_PRODUCERS;i++)
		pthread_mutex_init(&mutexStove[i], NULL);

	for (int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_create(&threads[i], NULL, cooking, (void *) msgs[i]) != 0) perror("Failed to create thread.");
	}
	for(int i=0;i<MAX_THREADS;i++)
	{
		if(pthread_join(threads[i], NULL) != 0) perror("Thread not terminated properly.");
	}

	for(int i=0;i<MAX_PRODUCERS;i++)
		pthread_mutex_destroy(&mutexStove[i]);


	return 0;
}
