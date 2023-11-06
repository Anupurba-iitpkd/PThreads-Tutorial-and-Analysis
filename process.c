#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{	
	int x=2;
	int pid = fork();

	if (pid == -1) return 1; 
	if (pid == 0)
	{
		printf("This is the child process.\nID of child process: %d\n", getpid());
		x++;
	}
	sleep(2);
	printf("The value of x:%d\n",x);

	if(pid != 0) 
	{
		printf("This is the parent process.\nID of child process: %d & ID of parent process: %d\n", pid,getpid());
		wait(NULL);
	}


	return 0;
}
