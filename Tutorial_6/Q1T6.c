#define _XOPEN_SOURCE 700 // required for barriers to work

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for fork
#include <sys/wait.h> // for wait

#define NUM_PROC 2

int main(void)
{
    // Create an array of process ids  
    pid_t pid[NUM_PROC];

    // Create children processes
    for(int i = 0; i < NUM_PROC; i++){
        pid[i] = fork();
        if(pid[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid[i] == 0) {
            // Code for child processes
            sleep(1); // Wait for 1 second
            char filename[20];
            sprintf(filename, "child%d.txt", i + 1);
            FILE *file = fopen(filename, "r");
            if(file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            char line[100];
            fgets(line, sizeof(line), file);
            printf("Child %d: %s\n", i + 1, line);
            fclose(file);
            exit(EXIT_SUCCESS); // Child process exits after its work is done
        }
    }

    // Code for the parent process (master)
    // Write to files
    for(int i = 0; i < NUM_PROC; i++) {
        char filename[20];
        sprintf(filename, "child%d.txt", i + 1);
        FILE *file = fopen(filename, "w");
        if(file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "child %d\n", i + 1);
        fclose(file);
    }

    // Wait for all child processes to finish
    for(int i = 0; i < NUM_PROC; i++) {
        wait(NULL);
    }

    return 0;
}

