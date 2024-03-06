#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PROCESS "./process"

void sigint_handler(int sig) {
	printf("\033[1;31mChild process received SIGINT signal, terminating...\n");
	printf("\033[0m"); // Reset color
	exit(EXIT_SUCCESS);
}

int main(void) {
	pid_t pid;

	pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		// Child process
		signal(SIGINT, sigint_handler); // Register SIGINT handler
		execl(PROCESS, PROCESS, NULL); // Executing the specified command
		perror("execl"); // exec only returns if there is an error
		return EXIT_FAILURE; // If execl fails
	} else {
		// Parent process
		sleep(5); // Allow child process to start
		if (kill(pid, SIGINT) != 0) { // Sending SIGINT to the child process
			perror("kill");
			return EXIT_FAILURE;
		}
		wait(NULL); // Wait for child process to terminate
	}

	return EXIT_SUCCESS;
}
