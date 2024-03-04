#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_NUM 5

int moving_sum[MAX_NUM] = {0};
sem_t semaphores[MAX_NUM];

struct ThreadData {
	int number;
	int index;
};

void* factorial(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    int fact = 1;

    for (int i = 1; i <= data->number; i++) {
        fact *= i;
    }

    int prev_sum = 0;
    if (data->index > 0) {
        sem_wait(&semaphores[data->index - 1]);
        prev_sum = moving_sum[data->index - 1];
    }

    sem_wait(&semaphores[data->index]);
    moving_sum[data->index] = prev_sum + fact;
    sem_post(&semaphores[data->index]);

    pthread_exit(NULL);
}

int main() {
	pthread_t threads[MAX_NUM];
	struct ThreadData data[MAX_NUM];

	for (int i = 0; i < MAX_NUM; i++) {
    		sem_init(&semaphores[i], 0, 1); // Initialize semaphore
	}

	for (int i = 0; i < MAX_NUM; i++) {
    		printf("Enter number %d: ", i + 1);
    		scanf("%d", &data[i].number);
    		data[i].index = i;
    		pthread_create(&threads[i], NULL, factorial, (void *)&data[i]);
	}

	for (int i = 0; i < MAX_NUM; i++) {
   	 	pthread_join(threads[i], NULL); // Wait for threads to finish
	}

	printf("Contents of moving_sum:\n");
	for (int i = 0; i < MAX_NUM; i++) {
	    	printf("%d ", moving_sum[i]);
	}
	printf("\n");

	for (int i = 0; i < MAX_NUM; i++) {
	    	sem_destroy(&semaphores[i]); // Cleanup semaphores
	}

	return 0;
}

