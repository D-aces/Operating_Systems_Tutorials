#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MEMORY 1024

struct proc {
	char name[256];
	int priority; // priority
	int pid; // PID
	int address; // address index
	int memory; // Memory address
	int runtime; // runtime
	bool suspended; // suspended flag
};

struct proc_queue {
	struct proc avail_mem[MEMORY];
	int front, rear;
};

// Function to add a process to the queue based on priority
void push(struct proc_queue *queue, struct proc process) {
	if (queue->rear == MEMORY - 1) {
	    	printf("Queue is full. Cannot add more processes.\n");
	    	return;
	}

	int pos = queue->rear;
	while (pos >= 0 && queue->avail_mem[pos].priority > process.priority) {
	    	queue->avail_mem[pos + 1] = queue->avail_mem[pos];
	    	pos--;
	}

	queue->avail_mem[pos + 1] = process;
	queue->rear++;
}

// Function to print the details of the processes in the queue
void printQueue(struct proc_queue *queue) {
	printf("Processes in the queue:\n");
	for (int i = queue->front; i <= queue->rear; i++) {
	    	printf("Name: %s, Priority: %d, Memory: %d, Runtime: %d\n", queue->avail_mem[i].name, queue->avail_mem[i].priority, queue->avail_mem[i].memory, queue->avail_mem[i].runtime);
	}
}

int main() {
	FILE *file = fopen("processes_q2.txt", "r");
	if (!file) {
	    	printf("Error opening file.\n");
	    	return 1;
	}

	struct proc_queue priority_queues[4];  // Priority queues from 0 to 3
	for (int i = 0; i < 4; i++) {
	    	priority_queues[i].front = 0;
	    	priority_queues[i].rear = -1;
	}

	char name[256];
	int priority, memory, runtime;

	// Read processes from file and populate the priority queues
	while (fscanf(file, "%[^,], %d, %d, %d\n", name, &priority, &memory, &runtime) == 4) {
	    	struct proc new_process;
	    	strcpy(new_process.name, name);
	    	new_process.priority = priority;
	    	new_process.memory = memory;
	    	new_process.runtime = runtime;

	    	if (priority >= 0 && priority <= 3) {
	        	push(&priority_queues[priority], new_process);
	    	} else {
	        	printf("Invalid priority for process %s. Skipping.\n", new_process.name);
	    	}
	}

	fclose(file);

	// Print the processes in each priority queue
	for (int i = 0; i < 4; i++) {
 	   	printf("Priority %d:\n", i);
	    	printQueue(&priority_queues[i]);
	}

	return 0;
}
