#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for fork
#include <sys/wait.h> // for wait

int main(void)
{
	// Create an array of process ids  
	pid_t pid;
	// Create children processes
	pid = fork();
	if(pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}	    
	if(pid == 0) {
		// Code for child process
		sleep(1); // Wait for 1 second
		printf("Child process\n");
		exit(EXIT_SUCCESS); // Child process exits after its work is done
	}
	// Code for the parent process
	if(pid > 0) {
		wait(NULL);
		printf("Parent Process\n");
	}
	return 0;
}

