#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t total_bellcurve_lock;
pthread_mutex_t total_grade_lock;
pthread_barrier_t barrier;

double total_bellcurve = 0;
double total_grade = 0;

void *read_grades(void *data) // Reads grades from file
{
    printf("Started reading grades.\n");
    FILE *file = fopen("grades.txt", "r"); // Open file
    if (file == NULL)
    {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        if (fscanf(file, "%d", (int *)data + i) != 1) // Read grade from file
        {
            fprintf(stderr, "Error reading grade from file\n");
            fclose(file);
            pthread_exit(NULL);
        }
    }
    fclose(file);
    pthread_barrier_wait(&barrier);    
    printf("Exiting read_grades thread.\n");
    return NULL; // Exit
}

void *save_bellcurve(void *data) {
    pthread_barrier_wait(&barrier);  
    int *grade = (int *)data;
    double dgrade = (double)*grade;
    pthread_mutex_lock(&total_bellcurve_lock); // Lock
    total_grade += dgrade;
    pthread_mutex_unlock(&total_bellcurve_lock); // Unlock

    pthread_mutex_lock(&total_grade_lock); // Lock
    total_bellcurve += dgrade * 1.50;
    pthread_mutex_unlock(&total_grade_lock); // Unlock

    return NULL; // Exit
}

void write_bellcurve(int *grades) {
    FILE *file = fopen("bellcurve.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 10; i++) {
        double bellcurve_grade = grades[i] * 1.50;
        fprintf(file, "%.2f\n", bellcurve_grade);
    }

    fclose(file);
}

int main() // Main function
{
    pthread_mutex_init(&total_bellcurve_lock, NULL);
    pthread_mutex_init(&total_grade_lock, NULL);
    pthread_barrier_init(&barrier, NULL, 1); // Initialize barrier with count 1

    int grades[10];
    memset(grades, 0, sizeof(int) * 10);

    pthread_t tids[11];
    pthread_create(&tids[0], NULL, read_grades, (void *)grades);

    pthread_join(tids[0], NULL);

    pthread_barrier_destroy(&barrier);

    for(int i = 0; i < 10; i++) {
        printf("%d ", grades[i]);
    }
    printf("\n");

    for(int i = 0; i < 10; i++) {
    	pthread_create(&tids[i + 1], NULL,  save_bellcurve, (void *)&grades[i]);
    }
    // simulates barrier
    for(int i = 0; i < 10; i++) {
        pthread_join(tids[i + 1], NULL);
    }

    printf("Average: %.2f\n", total_grade / 10);
    printf("Bellcurve Average: %.2f\n", total_bellcurve / 10);
    write_bellcurve(grades);
    
    return 0;
}
