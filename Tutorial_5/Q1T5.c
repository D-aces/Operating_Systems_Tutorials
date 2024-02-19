#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int timer = 3;

void* hello_world(){
	sleep(rand() % timer + 1);
	printf("hello world\n");
	pthread_exit(NULL);
}

void* goodbye(){
	sleep(rand() % timer + 1);
	printf("goodbye\n");
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t ptid[2];
	pthread_create (&ptid[0], NULL, hello_world, NULL);
	pthread_create(&ptid[1], NULL, goodbye, NULL);

	pthread_join(ptid[0],NULL);
	pthread_join(ptid[1],NULL);
	return 0;
}

