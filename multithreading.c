#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int g;

void* function(void* id){

	//printing the thread ID from within the thread:
	pthread_t th = pthread_self();
	printf("Thread ID: %ld\n", th);

	int* myId = (int *) id;
	static int i=0;
	i++;
	g+=2;
	printf("Thread ID: %d, Static: %d, Global: %d\n", *myId, i, g);
}

int main(int argv, char* argc){
	pthread_t tid;
//	printf("thread ID: Before creation: %ld\n", tid);

	for (int i=0;i<3;i++){
		if (pthread_create(&tid, NULL, function, (void*)&tid) != 0) return 1; 
//		pthread_join(tid,NULL);
	printf("thread ID: After creation: %ld\n", tid);

	}
	pthread_exit(NULL);
	return 0;

}
