#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Structure for the processes
typedef struct {
    char parent[256];
    char name[256];
    int priority;
    int memory; // in MB
} proc;

// Structure for the tree of processes
typedef struct Node {
    proc process;
    struct Node* leftChild;
    struct Node* rightChild;
} procTree;

// Function to create a new node for the tree
procTree* createNode(proc *newProcess) {
    procTree *newNode = malloc(sizeof(procTree));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->process = *newProcess;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}

// Function to insert a new process node into the tree
void insert(proc *newProcess, procTree **tree) {
    if (*tree == NULL) {
        *tree = createNode(newProcess);
        return;
    }

    proc *current_node = &((*tree)->process);

    if (strcmp(current_node->name, newProcess->parent) <= 0)
        insert(newProcess, &((*tree)->leftChild));
    else
        insert(newProcess, &((*tree)->rightChild));
}

// Function to read processes from the file and construct the tree
void read_file(procTree **root) {
    FILE *file = fopen("processes_tree.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    proc newProcess;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d,%d", newProcess.parent, newProcess.name, &newProcess.priority, &newProcess.memory);
        insert(&newProcess, root);
    }

    fclose(file);
}

void preOrderTraversal(procTree* root) {
    if (root != NULL) {
        // Print root node contents
        printf("Parent Process name:%-10s | Process name:%-10s | Priority:%-3d | Memory:%-4d\n",
            root->process.parent, root->process.name,
            root->process.priority, root->process.memory);

        // Traverse left subtree
        preOrderTraversal(root->leftChild);

        // Traverse right subtree
        preOrderTraversal(root->rightChild);
    }
}

// Main function
int main() {
    procTree *root = NULL;
    read_file(&root);
    preOrderTraversal(root);

    return 0;
}

