/* Headers */
#include <stdio.h>//Standard input/output
#include <stdlib.h>//Standard library
#include <string.h>//String library
#include <unistd.h>//POSIX operating system API
#include <signal.h>//Signal handling
#include <sys/types.h>//Data types
#include <sys/wait.h>//Wait for process to change state

/* Constants */
#define MEMORY 1024//Memory size
#define MAX_PROCESSES 100//Maximum number of processes


/* Structs */
struct proc//Process structure
{
    char name[256];//Name of the process
    int priority;//Priority of the process
    int pid;//Process ID
    int address;//Memory address
    int memory;//Memory size
    int runtime;//Runtime
    int suspended;//Suspended flag
};

struct queue//Queue structure
{
    struct proc* items[MAX_PROCESSES];//Array of processes
    int front;//Front of the queue
    int rear;//Rear of the queue
    int size;//Size of the queue
};

struct queue priority, secondary;//Priority and secondary queues

/* Function prototypes */
void initialize_queue(struct queue* q)//Initialize queue
{
    q->front = 0;//Set front to 0
    q->rear = -1;//Set rear to -1
    q->size = 0;//Set size to 0
}

int is_empty(struct queue* q)//Check if queue is empty
{
    return q->size == 0;//Return true if size is 0
}

int is_full(struct queue* q)//Check if queue is full
{
    return q->size == MAX_PROCESSES;//Return true if size is equal to maximum number of processes
}

void push(struct queue* q, struct proc* item)//Push process to queue
{
    if (!is_full(q))//If queue is not full
    {
        q->rear = (q->rear + 1) % MAX_PROCESSES;//Increment rear
        q->items[q->rear] = item;//Add item to queue
        q->size++;//Increment size
    }
    else//If queue is full
    {
        printf("Queue is full\n");//Print error message
    }
}

struct proc* pop(struct queue* q)//Pop process from queue
{
    if (!is_empty(q))//If queue is not empty
    {
        struct proc* item = q->items[q->front];//Get item from queue
        q->front = (q->front + 1) % MAX_PROCESSES;//Increment front
        q->size--;//Decrement size
        return item;//Return item
    }
    else//If queue is empty
    {
        printf("Queue is empty\n");//Print error message
        return NULL;//Return NULL
    }
}

void execute_process(struct proc* process, int* avail_mem)//Execute process
{
    printf("Executing process: %s, Priority: %d, PID: %d, Memory: %d, Runtime: %d\n", process->name, process->priority, process->pid, process->memory, process->runtime);//Print process details
    int start_address = -1;//Start address
    for (int i = 0; i < MEMORY; i++)//Iterate through memory
    {
        int free_mem = 1;//Free memory flag
        for (int j = 0; j < process->memory; j++)//Iterate through memory
        {
            if (avail_mem[(i + j) % MEMORY] != 0)//If memory is not available
            {
                free_mem = 0;//Set free memory flag to false
                break;//Break loop
            }
        }
        if (free_mem)//If memory is available
        {
            start_address = i;//Set start address
            break;//Break loop
        }
    }
    if (start_address != -1)//If start address is not -1
    {
        process->address = start_address;//Set process address
        process->pid = fork();//Fork process
        if (process->pid == 0)//If child process
        {
            execl("./sigtrap", "sigtrap", NULL);//Execute sigtrap
            exit(0);//Exit process
        }
        else//If parent process
        {
            for (int i = 0; i < process->memory; i++)//Iterate through memory
                avail_mem[(start_address + i) % MEMORY] = 1;//Set memory to unavailable
        }
    }
    else//If start address is -1
    {
        printf("Not enough memory to execute process %s. Pushing back to queue.\n", process->name);//Print error message
        push(&secondary, process);//Push process to secondary queue
    }
}

void suspend_process(struct proc* process)//Suspend process
{
    if (process->suspended && process->pid != 0)//If process is suspended and process ID is not 0
    {
        printf("Resuming process: %s\n", process->name);//Print message
        kill(process->pid, SIGCONT);//Resume process
    } 
    else//If process is not suspended
    {
        process->pid = fork();//Fork process
        
        if (process->pid == 0)//If child process
        {
            execl("./sigtrap", "sigtrap", NULL);//Execute sigtrap
            exit(0);//Exit process
        }
    }
    
    sleep(1);//Run for 1 second
    
    printf("Suspending process: %s\n", process->name);//Print message
    kill(process->pid, SIGTSTP);//Suspend process
    process->suspended = 1;//Set suspended flag to true
    
    push(&secondary, process);//Push process to secondary queue
}

int main()//Main function
{
    initialize_queue(&priority);//Initialize priority queue
    initialize_queue(&secondary);//Initialize secondary queue

    FILE* file = fopen("processes_q2.txt", "r");//Open file
    if (!file)//If file does not exist
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct queue priority, secondary;//Priority and secondary queues
    initialize_queue(&priority);//Initialize priority queue
    initialize_queue(&secondary);//Initialize secondary queue
    
    int avail_mem[MEMORY];//Available memory
    memset(avail_mem, 0, sizeof(avail_mem));//Set memory to 0

    //Read processes from file
    char line[256];
    while (fgets(line, sizeof(line), file))//Read line from file
    {
        struct proc* new_process = malloc(sizeof(struct proc));//Allocate memory for new process
        sscanf(line, "%[^,], %d, %d, %d\n", new_process->name, &new_process->priority, &new_process->memory, &new_process->runtime);//Read process details from line
        new_process->pid = 0;//Set process ID to 0
        new_process->address = 0;//Set process address to 0
        new_process->suspended = 0;//Set suspended flag to false
        
        if (new_process->priority == 0)//If priority is 0
            push(&priority, new_process);//Push process to priority queue
        else
            push(&secondary, new_process);//Push process to secondary queue
    }
    fclose(file);//Close file

    //Execute priority processes
    while (!is_empty(&priority)) 
    {
        struct proc* process = pop(&priority);//Pop process from priority queue
        execute_process(process, avail_mem);//Execute process
        sleep(process->runtime);//Run for process runtime
        kill(process->pid, SIGTSTP);//Suspend process
        waitpid(process->pid, NULL, 0);//Wait for process to terminate
        for (int i = 0; i < process->memory; i++)//Iterate through memory
            avail_mem[(process->address + i) % MEMORY] = 0;//Set memory to available
        free(process);//Free process
    }

    //Execute secondary processes
    while (!is_empty(&secondary))
    {
        struct proc* process = pop(&secondary);//Pop process from secondary queue
        suspend_process(process);//Suspend process
        if (process->runtime == 1)//If runtime is 1
        {
            sleep(1);//Run for 1 second
            kill(process->pid, SIGINT);//Terminate process
            waitpid(process->pid, NULL, 0);//Wait for process to terminate
            for (int i = 0; i < process->memory; i++)//Iterate through memory
                avail_mem[(process->address + i) % MEMORY] = 0;//Set memory to available
            free(process);//Free process
        }
        else//If runtime is not 1
        {
            process->runtime--;//Decrement runtime
            push(&secondary, process);//Push process to secondary queue
        }
    }

    return 0;//Return 0