//Question #4 of Tutorial 6

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Constants */
#define BUFFER_SIZE 5
#define NUMBERS 10

/* Global variables */
int buffer[BUFFER_SIZE] = {0};
sem_t empty, full;
pthread_mutex_t mutex;

/* Functions */
void *producer(void *arg)//Producer function
{
    int *numbers = (int *)arg;//Casting the argument to an integer pointer

    for (int i = 0; i < NUMBERS; ++i)
    {
        usleep(rand() % 1000000);//Random delay

        sem_wait(&empty);//P()
        pthread_mutex_lock(&mutex);//Lock the mutex

        int index = 0;//Index of the buffer
        while (buffer[index] != 0)//Find the first empty slot
        {
            index++;
            index %= BUFFER_SIZE;
        }
        buffer[index] = numbers[i];//Put the number in the buffer
        printf("Produced %d\n", numbers[i]);//Print the number

        pthread_mutex_unlock(&mutex);//Unlock the mutex
        sem_post(&full);//V()
    }

    pthread_exit(NULL);//Exit the thread
}

void *consumer(void *arg)//Consumer function
{
    int consumed_count = 0;//Counter for the number of consumed numbers

    while (consumed_count < NUMBERS)//While the number of consumed numbers is less than the total number of numbers
    {
        usleep(rand() % 1000000);//Random delay

        sem_wait(&full);//P()
        pthread_mutex_lock(&mutex);//Lock the mutex

        for (int i = 0; i < BUFFER_SIZE; ++i)//Iterate through the buffer
        {
            if (buffer[i] != 0)//If the slot is not empty
            {
                printf("Consumed %d\n", buffer[i]);//Print the number
                buffer[i] = 0;//Empty the slot
                consumed_count++;//Increment the counter
                break;//Break the loop
            }
        }

        pthread_mutex_unlock(&mutex);//Unlock the mutex
        sem_post(&empty);//V()
    }

    pthread_exit(NULL);//Exit the thread
}

/* Main */
int main()
{
    srand(time(NULL));//Seed the random number generator

    sem_init(&empty, 0, BUFFER_SIZE);//Initialize the empty semaphore
    sem_init(&full, 0, 0);//Initialize the full semaphore
    pthread_mutex_init(&mutex, NULL);//Initialize the mutex

    int numbers[NUMBERS];//Array to store the numbers
    printf("Enter %d numbers: ", NUMBERS);//Prompt the user for input
    for (int i = 0; i < NUMBERS; ++i)//Iterate through the array
        scanf("%d", &numbers[i]);//Read the input

    pthread_t producer_thread, consumer_thread;//Producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, (void *)numbers);//Create the producer thread
    pthread_create(&consumer_thread, NULL, consumer, NULL);//Create the consumer thread

    pthread_join(producer_thread, NULL);//Join the producer thread
    pthread_join(consumer_thread, NULL);//Join the consumer thread

    printf("Contents of buffer after consumption:\n[ ");//Print the contents of the buffer
    for (int i = 0; i < BUFFER_SIZE; ++i)//Iterate through the buffer
        printf("%d ", buffer[i]);//Print the number
    printf("]\n");//Print a newline

    sem_destroy(&empty);//Destroy the empty semaphore
    sem_destroy(&full);//Destroy the full semaphore
    pthread_mutex_destroy(&mutex);//Destroy the mutex

    return 0;//Exit
}
