#define _XOPEN_SOURCE 700 // required for barriers to work

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <string.h>
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

void push(proc new_process, queue** head) {
	queue *current = *head;
	
	// If the list is empty, make the new node the head
	if (current == NULL) {
		*head = (queue*)malloc(sizeof(queue));
		(*head)->process = new_process;
		(*head)->next = NULL;
		return;
	}

	// Otherwise, traverse to the end of the list
	while (current->next != NULL) {
		current = current->next;
	}

	// Allocate memory for the new node and insert it at the end
	current->next = (queue*)malloc(sizeof(queue));
	current->next->process = new_process;
	current->next->next = NULL;
}

proc pop(queue** head) {
	if (*head == NULL) {
		proc empty;
		strcpy(empty.name, "");
		empty.priority = -1;
		empty.pid = -1;
		empty.runtime = -1;
		return empty;
	}

	queue* temp = *head;
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

void print_process(proc *process) {
	printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", process->name,
			process->priority, process->pid, process->runtime);
}

void print_list(queue *head) {
	queue* current = head;
	while(current != NULL) {
		proc process = current->process;
		print_process(&process);
		current = current->next;
	}
}

void read_file(queue **head){
	FILE *file = fopen("processes.txt", "r");	
	char line[256];	

	if (file == NULL) {
		printf("error opening file.\n");
		exit(0);
	}

	// Create an instance of the proc struct and add it to the
	// linked list using the push() function.
	while (fgets(line, sizeof(line), file)) {
		proc new_process;
		sscanf(line, "%[^,],%d,%d,%d", new_process.name, &new_process.priority, 
		&new_process.pid, &new_process.runtime);
		push(new_process, head);
	}

	fclose(file);
}

int main()
{	
	queue *head = NULL;

	// read the process from the file and add them to the linked list	
	read_file(&head);
	
	//delete process named "emacs"
	delete_name("emacs", &head);
	//delete process with pid 12235
	delete_pid(12235, &head);

	// print the new list, but using pop
	proc current_process;
	while (head != NULL) {
		current_process = pop(&head);
		print_process(&current_process);
	}
	
	return 0;
}
