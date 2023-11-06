#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0; 
pthread_mutex_t mutex;
int lock = 0;

void* routine(void* msg){
	char* m = (char* ) msg;
	for (int i=0;i<1000000;i++)
	{
		printf("%s\n",m);
/*		while(lock == 1);
		lock=1;
		mails+=1;
		lock=0; */
		pthread_mutex_lock(&mutex);
		mails += 1;
		pthread_mutex_unlock(&mutex); 
	} 

}

int main(int argc, char* argv[]){

	char* msg1="Thread 1";
	char* msg2="Thread 2";

	pthread_t t1, t2; 

	pthread_mutex_init(&mutex, NULL);
	if (pthread_create(&t1, NULL, routine, (void*) msg1) != 0) return 1;
        if (pthread_create(&t2, NULL, routine, (void*) msg2)) return 2;
	if (pthread_join(t1, NULL)) return 3;
	if (pthread_join(t2, NULL)) return 4;
	pthread_mutex_destroy(&mutex);

	printf("Main thread goes to sleep\n");
	sleep(5);

	printf("The value of mails is : %d\n", mails);
	return 0;
}
