#define _XOPEN_SOURCE 700 // required for barriers to work

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct {
	char name[256];
	int priority;
	int pid;
	int runtime; // in seconds
} proc;

typedef struct Node {
	proc process;
	struct Node* next;
} queue;

void push(proc process, queue** head) {

	// Logic to add each process to the linked list (queue).
	queue* new_node = (queue*)malloc(sizeof(queue));
	if (new_node == NULL) {
		printf("Memory allocation failed.");
		return;
	}
	
	new_node->process = process;
	new_node->next = *head;
	*head = new_node;
};

int main()
{
	FILE *file = fopen("processes.txt", "r");	
	if (file == NULL) {
		printf("error opening file.\n");
		return 1;
	}
	
	queue* head = NULL;
	char line[256];
	
	// Create an instance of the proc struct and add it to the
	// linked list using the push() function.
	
	while (fgets(line, sizeof(line), file)) {
		proc new_process;
		sscanf(line, "%[^,],%d,%d,%d", new_process.name, &new_process.priority, 
		&new_process.pid, &new_process.runtime);
		push(new_process, &head);
		}
		
	fclose(file);
	
	// After all processes have been added to linked list, iterate through and print
	// name, priority, pid, runtime of each process 
	
	queue* current = head;
	while (current != NULL) {
		printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", current->process.name,
		current->process.priority, current->process.pid, current->process.runtime);
		current = current->next;
	}
	
	return 0;
}

