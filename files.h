#ifndef FILES
#define FILES
#define STR_LEN 100
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/LinkedList.h"
#include "../headers/view.h"
#include "../headers/raylib.h"
#include "../headers/tools.h"

// Save a GIF to a file
// Returns 1 on success, 0 on failure
void saveProject(FrameNode** head);

// Load a GIF from a file
// Returns the head of the loaded frame list, or NULL if loading failed
void loadProject(FrameNode** head);

#endif