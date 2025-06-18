#ifndef TOOLSH
#define TOOLSH

#include "../headers/view.h"
#include "../headers/files.h"

// Function declarations:

// function handles the main menu:
int menu(void);
// Handles starter menu(load, create):
int userMode(void);
// Clear the buffer
void clearInputBuffer(void);
// Get user string input:
void myFgets(char str[], int len);
// Input for frame:
int frameInputIndex(int min, int max);
// Duration input:
int durationInput();
#endif
