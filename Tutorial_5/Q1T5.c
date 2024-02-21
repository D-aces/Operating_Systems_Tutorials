#define _XOPEN_SOURCE 600//Required for barriers to work

/* Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int timer = 3;//Timer length

void* hello_world()//Prints hello world
{
	sleep(rand() % timer + 1);
	printf("hello world\n");
	pthread_exit(NULL);
}

void* goodbye()//Prints goodbye
{
	sleep(rand() % timer + 1);
	printf("goodbye\n");
	pthread_exit(NULL);
}

/* Main */
int main()
{
	pthread_t ptid[2];//Thread ID
	pthread_create (&ptid[0], NULL, hello_world, NULL);//Create thread
	pthread_create(&ptid[1], NULL, goodbye, NULL);//Create thread

	pthread_join(ptid[0],NULL);
	pthread_join(ptid[1],NULL);

	return 0;//Exit
}

