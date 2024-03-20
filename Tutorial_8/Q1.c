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

// Creating new nodes
procTree* create(proc newProcess) {
	// Allocate memory for the new node
	procTree* newNode = malloc(sizeof(procTree));
	if (newNode == NULL) {
		return NULL;
	}
	// Set values
	newNode->process = newProcess;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	return newNode;
}

// Placing left child nodes
procTree* insertLeft(procTree* root, proc newProcess) {
	// Given the root node, create and set the newProcess as left child
	root->leftChild = create(newProcess);
	return root->leftChild;
}

// Placing right child nodes
procTree* insertRight(procTree* root, proc newProcess) {
	// Given the root node, create and set the newProcess as right child
	root->rightChild = create(newProcess);
	return root->rightChild;
}

// Function to generate the tree
void generateTree(procTree* root, proc newProcess) {
	// If root is NULL, create the root node
	if (root == NULL) {
		root = create(newProcess);
	} else {
		// If left child is NULL, set left child to new process
		if (root->leftChild == NULL) {
			insertLeft(root, newProcess);
		}
		// If right child is NULL, set right child to new process
		else if (root->rightChild == NULL) {
			insertRight(root, newProcess);
		}
	}
}

// Function to read processes from the text file and construct the tree
void read_file(procTree** root) {
	FILE* file = fopen("processes_tree.txt", "r");
	if (file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}
	char line[256];
	procTree* currentRoot = NULL; // Current root for subtree
	while (fgets(line, sizeof(line), file)) {
		proc newProcess;
		sscanf(line, "%[^,],%[^,],%d,%d", newProcess.parent, newProcess.name, &newProcess.priority, &newProcess.memory);
		// Check if newProcess.parent contains "NULL"
		if (strstr(newProcess.parent, "NULL") != NULL) {
			// Set it to NULL
			newProcess.parent[0] = '\0'; // Empty string represents NULL
		}
		// If currentRoot is NULL, set it to the root of the main tree
		if (currentRoot == NULL) {
			*root = create(newProcess);
			currentRoot = *root;
		} else {
			// Set left child of currentRoot
			if (currentRoot->leftChild == NULL) {
				insertLeft(currentRoot, newProcess);
			}
			// Set right child of currentRoot
			else if (currentRoot->rightChild == NULL) {
				insertRight(currentRoot, newProcess);
			}
			// If both left and right children are set, move to the next subtree
			if (currentRoot->leftChild != NULL && currentRoot->rightChild != NULL) {
				currentRoot = currentRoot->leftChild; // Set left child as new subtree root
			}
		}
	}
	fclose(file);
}

// Perform pre-order traversal on the binary tree
void preOrderTraversal(procTree* root) {
	if (root != NULL) {
		// Print root node contents
		printf("Parent Process name: %s, Process name: %s, Priority: %d, Memory: %d\n",
				root->process.parent, root->process.name, root->process.priority, root->process.memory);
		// Traverse left subtree
		if(root->leftChild != NULL){
			preOrderTraversal(root->leftChild);
		}
		// Traverse right subtree
		if(root->rightChild != NULL){
			preOrderTraversal(root->rightChild);
		}
	}
}

int main()
{
	procTree *root = NULL;
	read_file(&root);
	preOrderTraversal(root);
	return 0;	
}

