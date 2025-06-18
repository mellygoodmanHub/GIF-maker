#ifndef LINKEDLISTH
#define LINKEDLISTH

#define FALSE 0
#define TRUE !FALSE

// Frame struct
typedef struct Frame
{
    char* name;
    unsigned int duration;
    char* path;  
} Frame;

// Link (node) struct
typedef struct FrameNode
{
    Frame* frame;
    struct FrameNode* next;
} FrameNode;

// Function declarations:

// Init's a frame with values:
FrameNode* initFrame(FrameNode* head);
// Creates a new frame with init frame values:
FrameNode* createFrame(const char* path, unsigned int duration, const char* name);
// Find a frame:
FrameNode* findFrame(FrameNode* head, const char* name, FrameNode** prev);
// Find's frame to free using freeFrame:
int deleteSpecificFrame(FrameNode** head, const char* name);
// Free's an induvidaul frame:
void freeFrame(FrameNode* frameNode);
// Free all the frames in the GIF:
void freeAllList(FrameNode* head);
// Print's all the frames in a list:
void printFramesList(FrameNode* head);
// When adding a frame to the project:
void insertFrameAtEnd(FrameNode** head, FrameNode* newFrame);
// Change the index of a frame:
int changeFrameIndex(FrameNode** head, const char* name, int index, int count);
// Change a duration of a single frame:
void changeFrameDuration(FrameNode* head, const char* name, unsigned int duration);
// Change duration for all frames:
void changeAllFramesDurations(FrameNode* head, unsigned int duration);
// Count loaded proeject frames:
int countFrames(FrameNode** head);
#endif
