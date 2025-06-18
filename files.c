#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/files.h"
#include "../headers/LinkedList.h"
#include "../headers/tools.h"

// My constants:
#define MAX_LINE_LEN 500
#define STR_LEN 100
#define MAX_DURATION_STR_LEN 20
#define PNG_EXTENSION ".png"
#define JPG_EXTENSION ".jpg"
#define EXTENSION_LEN 4
#define VALID_COMMA_COUNT 2
#define FIRST_COMMA 1
#define SECOND_COMMA 2
#define DURATION_OFFSET 1
#define PATH_EXTENSION_VALID 0
#define SAVE_SUCCESS 0
#define SAVE_FAILURE 1
#define NULL_TERMINATOR 1
#define FAILED_MALLOC "Failed to allocate memory for FrameNode.\n"

/*
The function parses the lines in the file to upload data.
Input: A line, the head of the list.
output: None.
*/
void parseLines(char* line, FrameNode** head)
{
    // Init name and path:
    char name[STR_LEN] = {0};
    char path[STR_LEN] = {0};
    char durationStr[MAX_DURATION_STR_LEN] = {0}; // Temporary buffer to hold duration string
    // Info:
    int duration = 0; // TO apply to frame
    int durationLen = 0; // Very diffrent
    int i = 0;
    int commaCount = 0;
    int pathLen = 0;
    // Index's:
    int pathStart = 0;
    int pathEnd = 0;
    int durationStart = 0;
    int durationEnd = 0;
    int nameStart = 0;

    line[strcspn(line, "\n")] = '\0'; // Make sure every line is NULLED

    while(line[i] != '\0') // While the char isn't the end of the line
    {
        if(line[i] == ',') // Check if it's a comma
        {
            commaCount++; // Add to comma count
            if(commaCount == FIRST_COMMA)
            {
                pathEnd = i; // The end of path
                durationStart = i + DURATION_OFFSET; // Start of dur
            }
            else if(commaCount == SECOND_COMMA) // If it's the dur end
            {
                durationEnd = i;
                nameStart = i + DURATION_OFFSET; // Name start
                break;
            }
        }
        i++; // Move to next char
    }

    if(commaCount == VALID_COMMA_COUNT) // If their is't invalid data
    {
        pathLen = pathEnd - pathStart; // Amazing
        if(pathLen > 0 && pathLen < STR_LEN) // Valid path
        {
            strncpy(path, line + pathStart, pathLen); // Copy oer 
            path[pathLen] = '\0';

            durationLen = durationEnd - durationStart;  // Calculate length of the duration
            if(durationLen > 0)
            {
                // Get len:
                strncpy(durationStr, line + durationStart, durationLen);
                durationStr[durationLen] = '\0';
                duration = atoi(durationStr);

                strcpy(name, line + nameStart);

                pathLen = strlen(path);
                // Check if the path ends with a valid image file extension (PNG or JPG)
                if((pathLen >= EXTENSION_LEN && strcmp(path + pathLen - EXTENSION_LEN, PNG_EXTENSION) == PATH_EXTENSION_VALID) || (pathLen >= EXTENSION_LEN && strcmp(path + pathLen - EXTENSION_LEN, JPG_EXTENSION) == PATH_EXTENSION_VALID))
                {
                    // Allocate memory for a new FrameNode
                    FrameNode* newFrame = (FrameNode*)malloc(sizeof(FrameNode));
                    if(!newFrame) 
                    {
                        printf(FAILED_MALLOC);
                        exit(1);
                    }

                    // Allocate memory for the Frame inside the new node
                    newFrame -> frame = (Frame*)malloc(sizeof(Frame));
                    if(!newFrame -> frame)
                    {
                        printf(FAILED_MALLOC);
                        free(newFrame); // cleanup previously allocated memory
                        exit(1);
                    }

                    // Allocate memory for the name string and copy the provided name into it
                    newFrame -> frame -> name = (char*)malloc(strlen(name) + NULL_TERMINATOR);
                    if(!newFrame -> frame -> name) 
                    {
                        printf(FAILED_MALLOC);
                        free(newFrame -> frame);
                        free(newFrame);
                        exit(1);
                    }

                    strcpy(newFrame -> frame -> name, name);

                    // Allocate memory for the path string and copy the provided path into it:
                    newFrame->frame->path = (char*)malloc(strlen(path) + NULL_TERMINATOR);
                    if(!newFrame->frame->path) 
                    {
                        printf(FAILED_MALLOC);
                        free(newFrame->frame->name);
                        free(newFrame->frame);
                        free(newFrame);
                        exit(1);
                    }
                    strcpy(newFrame->frame->path, path);

                    // Set the duration value inside the Frame
                    newFrame -> frame -> duration = duration;

                    // Initialize the next pointer to NULL (end of linked list)
                    newFrame -> next = NULL;

                    // Insert the newly created node at the end of the linked list
                    insertFrameAtEnd(head, newFrame);
                }
            }
        }
    }
}
/*
The funciton saves a project.
Input: The head of a linkedList.
Output None.
*/
void saveProject(FrameNode** head)
{
    // Init file vars:
    FrameNode* curr = *head;
    FILE* projectFile = NULL;
    char filePath[STR_LEN] = {0};
    // Get project path:
    printf("Please enter file path to save project in: ");
    myFgets(filePath, STR_LEN);

    projectFile = fopen(filePath, "w"); // Test file
    if(projectFile) // If the files isn't NULL
    {
        while(curr)
        {
            // Print to file stats"
            fprintf(projectFile, "%s,%d,%s\n", curr -> frame -> path, curr -> frame -> duration, curr -> frame -> name);
            curr = curr -> next; // Move to next node
        }
        fclose(projectFile); // Close them file
        printf("Project saved successfully!\n");
    }
    else
    {
        printf("Error: Can't open file. Please try again with another path.\n");
    }
}
/*
The function loads the project.
Input: The linkedList head.
Output: None.
*/
void loadProject(FrameNode** head)
{
    // File vars:
    FILE* projectFile = NULL;
    char line[MAX_LINE_LEN] = {0};
    char filename[STR_LEN] = {0};
    
    // Get path:
    printf("Enter the path of the project (including project name):\n");
    myFgets(filename, STR_LEN);
    
    // Open file
    projectFile = fopen(filename, "r");
    if(projectFile) // Only proceed if file opened successfully
    {
        while(fgets(line, MAX_LINE_LEN, projectFile))
        {
            parseLines(line, head);
        }

        fclose(projectFile);
        printf("Project loaded successfully!\n");
    }
    else
    {
        printf("Error! Unable to open file!!\n");
    }
}
