#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../headers/tools.h"

#define STR_LEN 100
#define INIT_CHOICE -1
#define LOWER_VALID_INPUT 0
#define UPPER_VALID_INPUT 8
#define CREATE_PROJECT 0
#define LOAD_PROJECT 1
#define PLACE_HOLDER 5
#define NOT_STRING 1

// Magshimim function:
void myFgets(char str[], int len)
{
    fgets(str, len, stdin);
    str[strcspn(str, "\n")] = '\0';
}
// My Magshimim funciton:
void clearInputBuffer(void)
{
	char c = '\0';
	do
	{
		// Nothing
	} while ((c = getchar()) != '\n' && c != EOF);
}

/*
The function prompts the user to get the mode(load, create new).
Input: None.
Output: The user mode.
*/
int userMode(void)
{
    int mode = INIT_CHOICE; // Init it as -1 so that it doesn't enter the valid range

    // Project menu:
    do
    {
        printf("Welcome to Magshimim Movie Maker! What would you like to do?\n");
        printf(" [0] Create a new project\n [1] Load existing project\n");
        scanf("%d", &mode);
        clearInputBuffer(); // Make sure this function correctly clears the input buffer

        if(mode != CREATE_PROJECT && mode != LOAD_PROJECT)
        {
            printf("Invalid input. Please enter 0 or 1.\n");
        }
        /*
        if(mode == LOAD_PROJECT)
        {
            mode = INIT_CHOICE: // "Not implemented yet" - Pre-version
        } 
        */
    } while(mode != CREATE_PROJECT && mode != LOAD_PROJECT); // Loop continues if mode is neither valid option

    return mode;
}

/*
The function menu prompts the user with the menu and collect their choice(including error handling).
Input: None.
Output: The valid user choice.
*/
int menu(void)
{
    int choice = 0;
	int result = 0;

    // Loop until valid input is received
    do
    {
		// Menu:
        printf("What would you like to do next?\n");
        printf(" [0] Exit\n");
        printf(" [1] Add new Frame\n");
        printf(" [2] Remove a Frame\n");
        printf(" [3] Change frame index\n");
        printf(" [4] Change frame duration\n");
        printf(" [5] Change duration of all frames\n");
        printf(" [6] List frames\n");
        printf(" [7] Play movie!\n");
        printf(" [8] Save project\n");
		
        result = scanf("%d", &choice); // Collect user input, scanf error
        clearInputBuffer();

        if(result != NOT_STRING || choice < LOWER_VALID_INPUT || choice > UPPER_VALID_INPUT) // Check if the user entered a valid input
        {
            printf("ERROR - Invalid choice. Please select a valid option.\n"); // User error feedback
        }
    } while((result != NOT_STRING) || (choice < LOWER_VALID_INPUT || choice > UPPER_VALID_INPUT)); // Continue looping if input is invalid

    return choice;
}
/*
The function durationInput collect's the user's input(frame, new dur)
Input: An input buffer.
Output: The user's dur input.
*/
int durationInput(void)
{
	int dur = 0;

    do
    {
        printf("Enter duration for the frame: ");
        scanf("%u", &dur);
        clearInputBuffer();

        if(dur <= 0)
        {
            printf("Enter a number that's larger then 0!!!!!!!\n");
        }
    } while(dur <= 0);

	return dur;
}
/*
The frameInputIndex function get's the user's frame.
Input: The min, max frames
Output: The frame index.
*/
int frameInputIndex(int min, int max)
{
	// Init local vars:
    int option = 0;
    int result = 0;

    do
    {
        result = scanf("%d", &option); // Get user input and procces code
        clearInputBuffer();

        if(result != NOT_STRING || option < min || option > max)  // Make sure input is in a valid range and it had a succesful input(!string)
        {
			printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
        }
    } while (result != NOT_STRING || option < min || option > max); // Loop until user input's a string

    return option;
}

