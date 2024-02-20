/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Put global environment variables here

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens) {
	const char delim[2] = " ";
	char *token = strtok(input, delim);
	int i = 0;

	while (token != NULL) {
		tokens[i] = malloc(strlen(token) + 1);
		if (tokens[i] == NULL) {
			// Handle allocation failure if necessary
			fprintf(stderr, "Failed to allocate memory for token\n");
			exit(EXIT_FAILURE);
		}

		strcpy(tokens[i], token);
		token = strtok(NULL, delim);
		i++;
	}
	tokens[i] = NULL;
}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players){
	for(int i = 0; i <= num_players; i++){
		printf("Name: %s\t Score: %d\n", players[i].name, players[i].score);
	}
}


int main(void)
{
	// An array of 4 players, may need to be a pointer if you want it set dynamically
	player players[NUM_PLAYERS];

	// Input buffer and commands
	char buffer[BUFFER_LEN] = { 0 };

	// Prompt for players names and init each players in the array
	for(int i = 0; i < NUM_PLAYERS; i++){
		printf("\nEnter player number %d's name\n", i+1);
		scanf("%s", players[i].name);
	}

	// Display the game introduction and initialize the questions
	initialize_game();

	// Perform an infinite loop getting command input from users until game ends
	while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
	{

		// Initialize token
		char *token[4] = {0};
		
		// Execute the game until all questions are answered
		int question_counter = sizeof(questions);
		bool answer;
		char *category;
		int question_value;
		char response[BUFFER_LEN] = {0};
		category = (char*) calloc(BUFFER_LEN, sizeof(char));

		if(question_counter > 0){
			for(int i = 0; i <= 4; i++){
				printf("\n---------------------------------\n");
				printf("It is %s's turn \nMake your selection (Format: Category dollar amount [Ex. Technology 100])", players[i].name);
				display_categories();
				printf("\n");
				scanf("%s %d", category, &question_value);

				if(already_answered(category, question_value)){
					printf("!!! Question has already been selected, please enter another choice !!!\n");
					i--;
				}
				else {
					display_question(category, question_value);
					
					scanf(" %[^\n]", response);
					
					tokenize(response, token);
					answer = valid_answer(category, question_value, token[2]);
					
					printf("\nYour Response Was: %s\n", token[2]);
				
					printf("%d\n", answer);	
					if(answer){
						printf("Answer is correct, please chose another question\n");
						players[i].score += question_value;
						i--;
					}
					else{
						printf("Buzz, that is incorrect \n");
					}
					update_catalogue(category, question_value);
					question_counter--;	
				}
			}
		}
		else {
			free(category);
			// Display the final results and exit
			show_results(players, NUM_PLAYERS);
			break;
		}
	}

	return EXIT_SUCCESS;
}
