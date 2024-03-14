#define _XOPEN_SOURCE 700 // required for barriers to work

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_PROCESS 100

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

proc pop(queue** head) {
	if (*head == NULL) {
		proc empty;
		strcpy(empty.name, "");
		empty.priority = -1;
		empty.pid = -1;
		empty.runtime = -1;
		return empty;
	}

	// Setup a temp next node
	queue* temp = *head;
	// 
	*head = temp->next;
	proc process = temp->process;
	free(temp);
	return process;
}

proc* delete_name(char *name, queue** head) {
	queue* current = *head;
	queue* prev = NULL;
	while (current != NULL) {
		if (strcmp(current->process.name, name) == 0) {
			if (prev == NULL) {
				*head = current->next;
			} else {
				prev->next = current->next;
			}
			return &(current->process);
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}


proc* delete_pid(int pid, queue** head) {
	queue* current = *head;
	queue* prev = NULL;
	while (current != NULL) {
		if (current->process.pid == pid) {
			if (prev == NULL) {
				*head = current->next;
			} else {
				prev->next = current->next;
			}
			return &(current->process);
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

int main()
{
	int init_pid = 0;
	proc *hold_lp_process;
	int hold_counter = 0;
	FILE *file = fopen("processes_q5.txt", "r");

	if (file == NULL) {
		printf("error opening file.\n");
		return 1;
	}

	queue *head = NULL;
	char line[256];
	
	// Allocate memory
	hold_lp_process = malloc(sizeof(proc) * MAX_PROCESS);
	if (hold_lp_process == NULL) {
		printf("Memory allocation failed.\n");
		fclose(file);
		return 1;
	}

	// Read file and push priority 0 processes to the struct
	while (fgets(line, sizeof(line), file)) {
		proc new_process;
		new_process.pid = init_pid;
		sscanf(line, "%[^,],%d,%d", new_process.name, &new_process.priority, &new_process.runtime);

		if (new_process.priority > 0) { // Hold the non 0 priority processes in a dynamic array 
			if (hold_counter < MAX_PROCESS) {
				hold_lp_process[hold_counter] = new_process;
				hold_counter++;
			} else { 
				printf("Maximum number of processes exceeded.\n");
				break;
			}
		} else {

			push(new_process, &head);
		}
	}

	// Add all the held processes to struct
	if (hold_counter > 0) {
		for(int i = 0; i < hold_counter; i++){
			push(hold_lp_process[i], &head);
		}
	}
	// After all processes have been added to linked list, iterate through and print
	// name, priority, pid, runtime of each process 
	queue* current = head;
	while (head != NULL) {
		proc current_process = pop(&head);
		printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", current_process.name,
				current_process.priority, current_process.pid, current_process.runtime);
	}
	return 0;
}
