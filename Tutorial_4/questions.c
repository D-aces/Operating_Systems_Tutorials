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

// Initializes the array of questions for the game
void initialize_game(void)
{
	// Initial values to be added to the struct
	int cat_count = 0;
	char question_answer_key[NUM_CATEGORIES][NUM_QUESTIONS][2][MAX_LEN] = {
	{
            {"What is the most commonly used programming language for web development?", "The answer is JavaScript."},
            {"What does HTML stand for?", "It stands for HyperText Markup Language."},
            {"What is the purpose of CSS in web development?", "CSS is used for styling web pages."},
            {"What is an array in programming?", "An array is a data structure that stores a collection of elements."},
        },
        {
            {"What is a sorting algorithm?", "A sorting algorithm is an algorithm that arranges elements in a specified order."},
            {"What is the time complexity of the bubble sort algorithm?", "The time complexity of bubble sort is O(n^2)."},
            {"What is the difference between quicksort and mergesort?", "Quicksort is an in-place sorting algorithm, while mergesort is not."},
            {"What is a binary search?", "Binary search is a search algorithm that finds the position of a target value within a sorted array."},
        },
        {
            {"What is a database?", "A database is an organized collection of structured information."},
            {"What is SQL?", "SQL (Structured Query Language) is a language used for managing relational databases."},
            {"What is the primary key in a database table?", "The primary key uniquely identifies each record in a database table."},
            {"What is the difference between SQL and NoSQL databases?", "SQL databases are relational databases, while NoSQL databases are non-relational."},
        }};
	int value = 200;

    // initialize each question struct and assign it to the questions array
     for(int i = 0; i < NUM_QUESTIONS; i++){
	     strcpy(questions[i].category, categories[cat_count]);
	     strcpy(questions[i].question, question_answer_key[cat_count][0][i]);
	     strcpy(questions[i].answer, question_answer_key[cat_count][i][1]);
	     questions[i].answered = false;
	     questions[i].value = value;
	     value += 200;
	     // Assuming an equal number of questions for each category
	     if((i + 1) % (NUM_QUESTIONS / NUM_CATEGORIES) == 0){
		     cat_count++;
		     value = 200;
	     }
     }
    
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    // print categories and dollar values for each unanswered question in questions array
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{

}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
    for(int i = 0; i < NUM_QUESTIONS; i++){
	   if(questions[i].category == category && questions[i].value == value){
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
	   if(questions[i].category == category && questions[i].value == value){
		  if(questions[i].answered){
			  return true;
		  }
	   }
    }
    return false;
}

// Updates the catalogue of available questions
void update_catalogue(char *category, int value){
	for(int i = 0; i < 12; i++){
		if((questions[i].category == category) && (questions[i].value == value)){
			questions[i].answered = true;
		}
	}
}
