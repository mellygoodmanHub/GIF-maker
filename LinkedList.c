#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/view.h"
#include "../headers/LinkedList.h"
#include "../headers/tools.h"
#include "../headers/files.h"

// My constants:
#define STR_LEN 100
#define NULL_TERMINATOR 1
#define INVALID_INDEX 0
#define FIRST_POSITION 1
#define INSERTION_OFFSET 1
#define EXIT_FLAG 1

/*
The function initFrame deals with user input and file checking.
Input: The frame head.
Output: Frame node.
THE FUNCTION DOESNT CREATE THE FILE ITSELF BUT USES CREATEFRAME TO DO SO.
*/
FrameNode* initFrame(FrameNode* head)
{
    // Init frame tools:
    FILE* checkFile = NULL; // File check to make sure path is their
    FrameNode* frameNode = NULL; // Real frame node
    FrameNode* prev = NULL;
    // Init frame properties:
    char path[STR_LEN] = {0};
    char name[STR_LEN] = {0};
    unsigned int duration = 0;
    
    /* 
    Too many return values for function: 
    */

    // Frame path:
    printf("Please insert frame path:\n");
    myFgets(path, STR_LEN);
    
    do
    {
        // Frame name:
        printf("Please choose a name for that frame:\n");
        myFgets(name, STR_LEN);
        if(strcmp(name, "") == 0) // Make sure user didn't leave name blank
        {
            printf("Please don't leave name blank!\n");
        }
    } while(strcmp(name, "") == 0); // Continue looping until valid input

    // Frame duration:
    duration = durationInput();

    // Second loop(doesn't need do while)
    // Makes sure user input's a new name:
    while(findFrame(head, name, &prev))
    {
        printf("Name already exists! choose another name\n");
        myFgets(name, STR_LEN);
    }

    checkFile = fopen(path, "r"); // Check that the file the user inputed is valid:
    if(!checkFile)
    {
        printf("Can't find file! Frame will not be added\n\n");
    }
    else // Entered path is fine
    {
        fclose(checkFile);
        frameNode = createFrame(path, duration, name); // Use function to create a frame
        if(frameNode == NULL)
        {
            printf("Allocation error - failed to create node");
        }
        else
        {
            printf("*** Frame made successfully! ***\n");
        }
    }
    return frameNode;
}

/*
The function who actaully creates the node and frame.
Input: The frame path, the dur, and the frame name.
Output: The new frame node.
*/
FrameNode* createFrame(const char* path, unsigned int duration, const char* name)
{
    Frame* newFrame = NULL;
    FrameNode* newFrameNode = NULL; // Fake frame node
    // Frame info:
    int pathLen = 0;
    int nameLen = 0;
    // Create frame and node:
    newFrame = (Frame*)malloc(sizeof(Frame));
    newFrameNode = (FrameNode*)malloc(sizeof(FrameNode));
    if(!newFrame || !newFrameNode) // Allocation check
    {
        printf("Memory allocation failed for Frame or FrameNode.\n");
        free(newFrame);
        free(newFrameNode);
        return NULL;
    }

    // Calculate lengths once:
    pathLen = strlen(path);
    nameLen = strlen(name);

    // Allocate and copy path and name:
    newFrame -> path = (char*)malloc(pathLen + NULL_TERMINATOR);
    newFrame -> name = (char*)malloc(nameLen + NULL_TERMINATOR);
    if(!newFrame->path || !newFrame->name) // Check if memory was allocated correctly
    {
        printf("Memory allocation failed for path or name.\n");
        free(newFrame->path);
        free(newFrame->name);
        free(newFrame);
        free(newFrameNode);
        return NULL;
    }

    // Copy path and name to newFrame:
    strcpy(newFrame -> path, path);
    strcpy(newFrame -> name, name);

    // Copy dur and frame to the new frame:
    newFrame -> duration = duration;
    newFrameNode->frame = newFrame;
    // Init the next node in the list(tail)
    newFrameNode->next = NULL;

    return newFrameNode;
}
/*
The function free's an individual frame.
Input: The frame node.
Output: None.
*/
void freeFrame(FrameNode* frameNode)
{
    if(frameNode && frameNode->frame)  // If their is a frame
    {
        // Free details:
        free(frameNode -> frame -> name);
        free(frameNode -> frame->path);
        free(frameNode -> frame);
        free(frameNode); 
    }
}

/*
The function free's the entire list-list(gif).
input: The list head.
Output: None.
*/
void freeAllList(FrameNode* head)
{
    FrameNode* temp = NULL;

    while(head)
    {
        temp = head -> next; // Store the next node in temp
        freeFrame(head); // Free the memory of the current head
        head = temp; // Set head to the next node (temp)
    }
}
/*
The function changes the frame index.
Input: The frame head, it's name and it's place(index)
Output: None.
*/
int changeFrameIndex(FrameNode** head, const char* name, int index, int count)
{
    // Security checks:
    if(index > count)
    {
        printf("The place you chose is bigger than the number of frames (%d).\n", count);
        return EXIT_FLAG;
    }
    if(!head || !(*head) || !name || index <= INVALID_INDEX)
    {
        printf("Invalid parameters.\n");
        return EXIT_FLAG;
    }
    // Init me vars
    FrameNode* targetNode = NULL;
    FrameNode* targetPrevious = NULL;
    FrameNode* current = *head;
    int totalFrames = 0;
    int i = 0;

    // Find traget frame node:
    targetNode = findFrame(*head, name, &targetPrevious);
    if(!targetNode)
    {
        printf("No frame found with the given name.\n");
        return EXIT_FLAG;
    }

    // Detach target node from its current position:
    if(targetPrevious == NULL)  // If target node is the head node
    {
        *head = targetNode -> next; // Move head pointer to the next node
    }
    else
    {
        targetPrevious -> next = targetNode -> next; // Linking previous node directly to its next node
    }

    // Insert target node at the new position:
    if(index == FIRST_POSITION)
    {
        // Insert at head:
        targetNode -> next = *head;
        *head = targetNode;
    }
    else
    {
        // Find the insertion point:
        current = *head;
        for(i = FIRST_POSITION; i < index - INSERTION_OFFSET && current; i++)
        {
            current = current -> next; // Move the curr until we find point
        }
        if(current == NULL)
        {
            printf("Unexpected error: insertion point not found.\n");
            return EXIT_FLAG;
        }

        // Insert after current:
        targetNode -> next = current -> next;
        current -> next = targetNode;
    }
    return 0; // I'm not using one return god dammit!
}

/*
The function changes a node duration.
Input: The frame head, name, and ofc unsigned dur.
Output: None.
*/
void changeFrameDuration(FrameNode* head, const char* name, unsigned int duration)
{
    FrameNode* target = findFrame(head, name, NULL);  // Use findFrame to locate the node

    if(target && target -> frame)
    {
        target -> frame-> duration = duration;  // Update duration if frame found
    }
    else
    {
        printf("Frame with the name '%s' not found.\n", name);  // Print error if not found
    }
}
/*
The function changes the duration of all nodes(frames).
Input: The head of the linked-list, the dur you want.
*/
void changeAllFramesDurations(FrameNode* head, unsigned int duration)
{
    while(head) // Change each node
    {
        // If the frame exits(not NULL)
        if(head -> frame)
        {
            head -> frame -> duration = duration; // Change the dur
        }
        head = head -> next;
    }
}

FrameNode* findFrame(FrameNode* head, const char* name, FrameNode** prev)
{
    FrameNode* prevNode = NULL; // To keep track of the prenode
    FrameNode* result = NULL;   // Will store the matching node if found

    // Initialize the output prev pointer to NULL!!!!!!!!!! (many errors if not)
    if(prev != NULL)
    {
        *prev = NULL;
    }

    // Traverse the list until the end or until a match is found
    while(head != NULL && result == NULL)
    {
        // Check if the current node and its name are valid: o(n) - 1
        if(head -> frame != NULL && head -> frame -> name != NULL)
        {
            // Compare the frame name with the target name:
            if(strcmp(head->frame->name, name) == 0)
            {
                result = head;  // Match found
            }
        }

        // If no match yet, move to the next node
        if(result == NULL)
        {
            prevNode = head;
            head = head->next;
        }
    }

    // If match was found and prev was requested, set it(VERY IMPORTANT)
    if(result != NULL && prev != NULL)
    {
        *prev = prevNode;
    }

    return result;  // Return the matching node, or NULL if not found
}


/*
The function prints the frames in the list.
Input : frames list(head).
Output: None.
*/
void printFramesList(FrameNode* head)
{

    printf("Name\t\tDuration\tPath\n");
    printf("----\t\t--------\t----\n");

    while(head != NULL) // Loop over and print stuff
    {
        if(head->frame)
        {
            // Print more stuff:
            printf("%s\t\t", head -> frame -> name ? head->frame->name : "NULL");
            printf("%u ms\t\t", head -> frame -> duration);
            printf("%s\n", head -> frame -> path ? head -> frame -> path : "NULL");
        }
        // Move to next node
        head = head -> next;
    }
}

/*
The function insert the new frame at the end.
Input : head, new frame.
Output: None.
*/
void insertFrameAtEnd(FrameNode** head, FrameNode* newFrame)
{
    FrameNode* curr = NULL;
    // If list empty make new thingi(many errors):
    if(!*head)
    {
        *head = newFrame;
    }
    else
    {
        // Look to the end of the list:
        curr = *head;
        while(curr -> next)
        {
            curr = curr -> next;
        }
        // Set new node at the end:
        curr -> next = newFrame;
    }
}
/*
The function delete's a frame based on user input.
Input: The list head, the frame name.
output: If it got deleted.
*/
int deleteSpecificFrame(FrameNode** head, const char* name)
{
    FrameNode* prev = NULL;
    FrameNode* frameNode = findFrame(*head, name, &prev); // Use function to find frame
    int result = TRUE; // Set it to TRUE(used in other lib)

    if(frameNode) // If frame was found
    {
        if(!prev) // head frame
        {
            *head = frameNode -> next; // Move head to the next node
        }
        else
        {
            prev -> next = frameNode -> next; // Point to node after frameNode, effectively removing frameNode from the chain
        }

        freeFrame(frameNode); // Free them frame
    }
    else
    {
        printf("The frame was not found\n\n");
        result = FALSE;
    }

    return result;
}
/*
The function counts the amount of frames in a project.
Input: The head of the project.
Output: The amount of frames.
*/
int countFrames(FrameNode** head)
{
    int count = 0;
    FrameNode* tmp = *head;
    while(tmp)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}
