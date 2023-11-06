#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void* roll_dice(){
	int value = (rand() % 6)+1; 
	int * res =(int*) malloc(sizeof(int));
	*res = value;
	printf("The thread result is : %d\n",value);
	return (void *) res;
}


int main(int argc, char* argv[]){
	srand(time(NULL));
	int* result;

	pthread_t th; 
	if(pthread_create(&th, NULL, roll_dice, NULL) != 0) return 1; 
	pthread_exit(NULL);
	if(pthread_join(th,(void **) &result) != 0) return 2; 
	printf("Result=%d\n", *result);
	free(result);
	return 0; 
}
