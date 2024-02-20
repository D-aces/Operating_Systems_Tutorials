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
#include "questions.h"

question questions[NUM_QUESTIONS];

// List of 3 categories as array of strings
static char categories[NUM_CATEGORIES][MAX_LEN] = {
    "programming",
    "algorithms",
    "databases"
};

// Initializes the array of questions for the game
void initialize_game(void)
{
	// Initial values to be added to the struct
	int cat_count = 0;
	int cat_question_count = 0;

	char question_answer_key[NUM_CATEGORIES][NUM_QUESTIONS][2][MAX_LEN] = {
		{
			{"What is the most commonly used programming language for web development?", "JavaScript"},
			{"What does HTML stand for?", "HyperTextMarkupLanguage"},
			{"What is the purpose of CSS in web development?", "Styling"},
			{"What is an array in programming?", "DataStructure"},
		},
		{
			{"What is a sorting algorithm?", "AnAlogrithmThatArrangesElementsInASpecifiedOrder"},
			{"What is the time complexity of the bubble sort algorithm?", "O(n^2)"},
			{"What is the difference between quicksort and mergesort?", "QuicksortIsAnIn-placeSortingAlgorithm,WhileMergesortIsNot."},
			{"What is a binary search?", "ASearchAlgorithmThatFindsThePositionOfATargetValueWithinASortedArray"},
		},
		{
			{"What is a database?", "ACollectionOfStructuredInformation"},
			{"What is SQL?", "ALanguageUsedForManagingRelationalDatabases"},
			{"What is the primary key in a database table?", "UniquelyIdentifiesEachRecordInADatabaseTable"},
			{"What is the difference between SQL and NoSQL databases?", "SQLDatabasesAreRelationalDatabases,WhileNoSQLDatabasesAreNon-relational"},
		}};
	int value = 200;

	// initialize each question struct and assign it to the questions array
	for(int i = 0; i < NUM_QUESTIONS; i++){
		strcpy(questions[i].category, categories[cat_count]);
		strcpy(questions[i].question, question_answer_key[cat_count][cat_question_count][0]);
		strcpy(questions[i].answer, question_answer_key[cat_count][cat_question_count][1]);
		questions[i].answered = false;
		questions[i].value = value;
		value += 200;
		cat_question_count++;

		// Assuming an equal number of questions for each category
		if((i + 1) % (NUM_QUESTIONS / NUM_CATEGORIES) == 0){
			cat_count++;
			value = 200;
			cat_question_count = 0;
		}
		
	}

}


// Displays the question for the category and dollar value
void display_question(char *category, int value)
{ 
	for (int i = 0; i < NUM_QUESTIONS; i++) {
		if (strcmp(questions[i].category, category) == 0 && (questions[i].value == value)) {
			printf("\nFor %d points\n\tQuestion: %s\n\n", questions[i].value, questions[i].question); 
		}
	}
}

void display_answer(char *category, int value){
	for (int i = 0; i < NUM_QUESTIONS; i++) {
		if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
			printf("\nThe answer we were looking for was: \n\t%s\n\n", questions[i].answer); 
		}
	}
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
	// Look into string comparison functions
	for(int i = 0; i < NUM_QUESTIONS; i++){
		if(strcmp(questions[i].category, category) == 0 && questions[i].value == value){
			if(strcmp(questions[i].answer, answer) == 0){
				return true;
			}
		}
	}

	return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
	// lookup the question and see if it's already been marked as answered
	for(int i = 0; i < NUM_QUESTIONS; i++){
		if(strcmp(questions[i].category, category) == 0 && questions[i].value == value){
			if(questions[i].answered){
				return true;
			}
		}
	}
	return false;
}

// Count number of questions in a category
int category_question_count(char *category){
	int count = 0;
	for(int i = 0; i < NUM_QUESTIONS; i++){
		if(strcmp(questions[i].category, category) == 0)
			count++;
	}
	return count;
}

//Returns true if all the questions in the category have been answered
bool category_expended(char *category){
	int already_answered_counter = 0;
	int count = 0;

	for(int i = 0; i < NUM_QUESTIONS; i++){
		if(strcmp(questions[i].category, category) == 0)
			count++;

		if(already_answered(category, questions[i].value))
			already_answered_counter++;
	}

	if(already_answered_counter == count){
		return true;
	}
	else{
		return false;
	}
}

// Updates the catalogue of available questions
void update_catalogue(char *category, int value){
	for(int i = 0; i < 12; i++){
		if(strcmp(questions[i].category, category) == 0 && (questions[i].value == value)){
			questions[i].answered = true;
		}
	}
}


bool valid_category(char *category, int value){
	for (int i = 0; i < NUM_QUESTIONS; i++) {
		if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
			return true;
		}
    }
	return false;
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
	int value = 200;
	int cat_index = 0;
	// print categories and dollar values for each unanswered question in questions array
	for(int i = 0; i < NUM_CATEGORIES; i++){
		int cat_question_count = category_question_count(questions[cat_index].category); 

		// Skip expended categories
		if(category_expended(questions[cat_index].category)){
			cat_index += cat_question_count;
			continue;
		}
		
		printf("\n%-15s", questions[cat_index].category);

		for(int x = 0; x < cat_question_count; x++){
			if(questions[cat_index].answered){
				printf("-%-15s", "---");
			}
			else{
				printf("$%-15d", questions[x].value);
			}
			value += 200;
			cat_index++;
		}
	}
}
