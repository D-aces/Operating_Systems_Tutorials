#define _XOPEN_SOURCE 700 // required for barriers to work

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

struct proc {
	char name[256];
	int priority;
	int pid;
	int runtime; // in seconds
}

struct queue {
	proc process;
	struct *next = NULL;
}

void push(proc process, queue** head) {

	// Logic to add each process to the linked list (queue).
	queue* new_node = (*queue)malloc(sizeof(queue));
	if (new_node == NULL) {
		printf("Memory allocation failed.");
		return;
	}
	
	new_node->process = process;
	new_node->next = *head;
	*head = new_node;
}

int main()
{
	FILE *file = fopen("processes.txt", "r")	
	if (file == NULL) {
		printf("error opening file.\n");
		return 1;
	}
	
	queue
	// Create an instance of the proc struct and add it to the
	// linked list using the push() function.
	
	
	// After all processes have been added to linked list,
	// iterate through and print name, priority, pid, runtime of each process 
	
	return 0;
}
