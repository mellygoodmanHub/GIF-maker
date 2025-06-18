#include <stdio.h>
#include "../headers/LinkedList.h"
#include "../headers/view.h"
#include "../headers/raylib.h"
#include "../headers/tools.h"
#include "../headers/files.h"

// Constants:
#define CREATE_PROJECT 0
#define LOAD_PROJECT 1
#define LOAD_PLACEHOLDER 1
#define FIRST_MENU 10
#define SECOND_MENU 20
#define STR_LEN 100
#define FRAME_INDEX_START 1

// Menu-options:
enum options {EXIT = 0, ADD_FRAME, REMOVE_FRAME, CHANGE_INDEX, CHANGE_DURATION, CHANGE_ALL_DURATIONS, PRINT_FRAMES, PLAY ,SAVE};

int main(void)
{
    // Init the nodes and the framelist(the gif):
    FrameNode* frameList = NULL;
    FrameNode* frameNode = NULL;
    FrameNode* prev = NULL;
    // Buffer for input size:
    char inputBuffer[STR_LEN] = {0};
    enum options option = 0; // Init the user's option to 0(exit)
    int mode = 0;
    int index = 0; // Frame index
    int framesCount = 0; // Frames count
    unsigned int duration = 0; // unsigned int sucks for user input(instead check for negative values)
    
    mode = userMode(); // Use the menu function to get the user's input

    if(mode == LOAD_PROJECT) // If the user wants to load the god damn project
    {
        loadProject(&frameList); // Load it
        framesCount = countFrames(&frameList); // Add the frame count
    }
    // Do while the user doesn't want to exit
    do
    {
        option = menu();
        switch(option)
        {
            case EXIT:
                freeAllList(frameList);
                printf("Bye!\n");
                break;
            case ADD_FRAME:
                printf("*** Creating new frame ***\n");
                frameNode = initFrame(frameList); // Init the frame before entering info
                if(frameNode) // Check if build was succseful
                {
                    insertFrameAtEnd(&frameList, frameNode); // Insert a frame at the end to "add a frame"
                    framesCount++; // Add it to the frame count
                }
                break;
            case REMOVE_FRAME:
                printf("Enter the name of the frame you wish to erase\n"); // Prompt user
                myFgets(inputBuffer, STR_LEN); // User input
                if(deleteSpecificFrame(&frameList, inputBuffer)) // Make sure it works
                {
                    framesCount--; // Decrease frame count
                    printf("*** SUCCESSFULY REMOVED FRAME ***\n");
                }
                else
                {
                    printf("*** FAILED TO REMOVED FRAME ***\n");

                }
                break;
            case CHANGE_INDEX:
                printf("Enter the name of the frame:\n"); // Prompt user
                myFgets(inputBuffer, STR_LEN); // User input
                if(!findFrame(frameList, inputBuffer, &prev)) // If the frame wasn't found
                {
                    printf("This frame does not exist\n");
                    break; // Exit choice
                }

                printf("Enter the new index in the movie you wish to place the frame:\n"); // Prompt user
                index = frameInputIndex(FRAME_INDEX_START, framesCount); // User function to handle user input
                // NOTE INPUTBUFFER IS THE NAME FOR NOW!!!
                if(changeFrameIndex(&frameList, inputBuffer, index, framesCount)) // Change the frame based on the input
                {
                    printf("*** FRAME INDEX SUCCESSFULLY CHANGED ***\n");
                }
                else
                {
                    printf("*** FAILED TO CHANGE FRAME INDEX ***\n");
                }
                break;
            case CHANGE_DURATION:
                // Frame input:
                printf("Enter the name of the frame:\n");
                myFgets(inputBuffer, STR_LEN);
                // Collect dur input:
                duration = durationInput(); // Use functions to get dur
                changeFrameDuration(frameList, inputBuffer, duration); // Change the frame
                break;
            case CHANGE_ALL_DURATIONS:
                printf("Enter duration for all frames:\n"); // Prompt
                scanf("%u", &duration); // User input
                clearInputBuffer();

                changeAllFramesDurations(frameList, duration); // Handles all error's
                break;
            case PRINT_FRAMES:
                printFramesList(frameList); // Simple function to print
                break;
            case PLAY:
                play(frameList); // Use built in "view" function
                break;
            case SAVE:
                if(!frameList) // If the GIF is empty
                {
                    printf("No frames to save! Please add some frames first.\n");
                    break;
                }
                saveProject(&frameList);
                break;
            default:
                printf("Invalid option, try again!\n");
        }
    } while (option != EXIT);

    return 0;
}
