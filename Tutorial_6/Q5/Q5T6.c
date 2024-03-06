#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBERS_COUNT 5

int total_sum = 0;
sem_t semaphore;

void* factorial(void *arg) {
    int numIn = *((int*)arg);
    int val = 1;
    for(int i = 1; i <= numIn; i++){
        val *= i;
    }
    sem_wait(&semaphore);
    total_sum += val;
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main() {
    pthread_t tids[NUMBERS_COUNT];
    sem_init(&semaphore, 0, 1);

    int numbers[NUMBERS_COUNT];

    printf("Enter %d numbers:\n", NUMBERS_COUNT);
    for (int i = 0; i < NUMBERS_COUNT; i++) {
        scanf("%d", &numbers[i]);
    }

    // Fork process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
    	FILE *file = fopen("numbers.txt", "w");
        	if (file == NULL) {
        	    perror("Error opening file");
        	    exit(EXIT_FAILURE);
        	}

        // Write numbers to file
        for (int i = 0; i < NUMBERS_COUNT; i++) {
            fprintf(file, "%d\n", numbers[i]);
        }

        fclose(file);
        wait(NULL); // Wait for child process to finish
        // Read total sum from "sum.txt"
        FILE *sumFile = fopen("sum.txt", "r");
        int t_sum;
        fscanf(sumFile, "Total sum: %d", &t_sum);
        fclose(sumFile);
        printf("The total sum is %d\n", t_sum);
        
    } else { // Child process
        FILE *file = fopen("numbers.txt", "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < NUMBERS_COUNT; i++) {
            fscanf(file, "%d", &numbers[i]);
        }
        fclose(file);

        // Create threads
        for (int i = 0; i < NUMBERS_COUNT; i++) {
            pthread_create(&tids[i], NULL, factorial, (void *)&numbers[i]); // Pass the address of the number directly
        }

        // Join threads
        for (int j = 0; j < NUMBERS_COUNT; j++) {
            pthread_join(tids[j], NULL);
        }

        sem_destroy(&semaphore);

        // Write total sum to "sum.txt"
        FILE *sumFile = fopen("sum.txt", "w");
        if (sumFile == NULL) {
            perror("Error opening sum file");
            exit(EXIT_FAILURE);
        }
        fprintf(sumFile, "Total sum: %d\n", total_sum);
        fclose(sumFile);

        // Exit the child process
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}

