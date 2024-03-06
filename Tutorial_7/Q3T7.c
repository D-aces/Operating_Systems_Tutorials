#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PROCESS "/process"

int main(void){	
	pid_t pid;
	pid = fork();
	
	if(pid < 0){
		return -1;
	}
	if(pid == 0){
		exec(PROCESS, NULL);
		sleep(5);
	}
	else{
		wait(NULL);
	}

	return EXIT_SUCCESS;
}

