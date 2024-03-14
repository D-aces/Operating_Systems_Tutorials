#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PROCESS "./process"

int main(void) {
	pid_t pid;
	int waitPID = 0;
	int status;
	pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		// Child process
		execl(PROCESS, PROCESS, NULL); // Executing the specified command
		perror("execl"); // exec only returns if there is an error
		return EXIT_FAILURE; // If execl fails
	} else {
		// Parent process
		sleep(5); // Allow child process to start
		if (kill(pid, SIGTSTP) != 0) { // Sending SIGTSTP to the child process
			perror("kill");
			return EXIT_FAILURE;
		}
		sleep(10);
		if (kill(pid, SIGCONT) != 0) { // Sending SIGCONT to the child process
			perror("kill");
			return EXIT_FAILURE;
		}
		if(waitpid(pid, &status, 0) != 0){// Wait for child process to terminate
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}
