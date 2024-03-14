

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

/*  
 * AUTHORS: Dmitri Rios Nadeau, Noah Toma, Rolf-Jaden Sibal
 * Tutorial 7, Question 5
 */

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
	int init_pid = 0;
	proc *hold_lp_process = (proc*)malloc(sizeof(proc));
	int hold_counter = 0;
	FILE *file = fopen("processes_q5.txt", "r");	
	char line[256];	

	if (file == NULL) {
		printf("error opening file.\n");
		exit(0);
	}

	// Create an instance of the proc struct and add it to the
	// linked list using the push() function.
	while (fgets(line, sizeof(line), file)) {
		proc new_process;
		new_process.pid = init_pid;
		sscanf(line, "%[^,],%d,%d", new_process.name, &new_process.priority, &new_process.runtime);
		if(new_process.priority == 0){ // Adds priority 0 processes to the queue first
			push(new_process, head);
		} else { // Stores lower all other priority process in a dynamic array of processes to be added after
			hold_lp_process = (proc *)realloc(hold_lp_process, (hold_counter + 1) * sizeof(proc));			
			hold_lp_process[hold_counter] = new_process;
			hold_counter++;
		}
	}

	// Add all the held processes to struct
	if (hold_counter > 0) {
		for(int i = 0; i < hold_counter; i++){
			push(hold_lp_process[i], head);
		}
	}

	fclose(file);
	free(hold_lp_process);
}

int main()
{
	int status;
	pid_t pid;	
	queue *head = NULL;

	// Read the processes from the file and add them to the linked list
	read_file(&head);

	// Perform the 0th priority processes
	while (head != NULL && head->process.priority == 0) {
		proc process = pop(&head);
		pid = fork(); // Create a child process
		if (pid < 0) { // error check
			perror("fork()");
			return EXIT_FAILURE;
		}
		if (pid == 0) { 
			// Child process
			execl(process.name, process.name, NULL); // Execute the process
								 // If execl fails, exit child process
			exit(EXIT_FAILURE); // Exit child process
		}
		// Parent process
		process.pid = pid; // Set the PID for the process struct
		sleep(process.runtime); // Allow child process to start
		if (kill(pid, SIGINT) != 0) { // Sending SIGINT to the child process
			perror("kill");
			return EXIT_FAILURE;
		}
		if (waitpid(pid, &status, 0) != 0) {
			print_process(&process);
		}
	}
	// Perform the other priority processes
	while (head != NULL) {
		proc process = pop(&head);
		pid = fork(); // Create a child process
		if (pid < 0) { // error check
			perror("fork()");
			return EXIT_FAILURE;
		}
		if (pid == 0) {
			// Child process
			execl(process.name, process.name, NULL); // Execute the process
								 // If execl fails, delete the process struct
			delete_name(process.name, &head);
			exit(EXIT_FAILURE); // Exit child process
		}
		// Parent process
		process.pid = pid; // Set the PID for the process struct
		sleep(process.runtime); // Allow child process to start
		if (kill(pid, SIGINT) != 0) { // Sending SIGINT to the child process
			perror("kill");
			return EXIT_FAILURE;
		}
		if (waitpid(pid, &status, 0) != 0) {
			print_process(&process);
		}
	}

	return 0;
}
