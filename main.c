#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0; 

void* routine(void* msg){
/*	printf("Test from threads\n");
	sleep(3);
	printf("Ending Thread\n");  */
	char* m = (char* ) msg;
	for (int i=0;i<1000;i++)
	{
		printf("%s\n",m);
		mails += 1;
	} 

}

int main(int argc, char* argv[]){

	char* msg1="Thread 1";
	char* msg2="Thread 2";

	pthread_t t1, t2; 

	if (pthread_create(&t1, NULL, routine, (void*) msg1) != 0) return 1;
        if (pthread_create(&t2, NULL, routine, (void*) msg2)) return 2;
	if (pthread_join(t1, NULL)) return 3;
	if (pthread_join(t2, NULL)) return 4;


	printf("Main thread goes to sleep\n");
	sleep(5);

	printf("The value of mails is : %d\n", mails);
	return 0;
}
