/*********************************
* Class: MAGSHIMIM Final Project *
* Play function                  *
**********************************/

#include "../headers/view.h"
#include <stdio.h>

/**
play the movie!!
display the images each for the duration of the frame one by one and close the window
input: a linked list of frames to display
output: none
**/
void play(FrameNode* list)
{
    if (!list)
    {
        printf("Movie is empty. Nothing to play.\n");
        // Optionally, show a message in a Raylib window for a short duration
        InitWindow(1000, 1000, "Hello There!");
        // BeginDrawing();
        // ClearBackground(RAYWHITE);
        // DrawText("Movie is empty.", 50, 40, 20, DARKGRAY);
        // EndDrawing();
        // WaitTime(2.0); // Wait 2 seconds
        // CloseWindow();
        return;
    }

    FrameNode* head = list;
    int playCount = 0;

    // Determine window size from the first image
    // This assumes all images might be different sizes; the window will adapt to the first.
    // A more robust solution might be to find the max width/height in the list
    // or allow the user to specify dimensions.
    Image firstImage = LoadImage(head->frame->path);
    if (firstImage.data == NULL)
    {
        printf("Could not load the first image: %s\n", head->frame->path);
        // Potentially try next image or abort
        return;
    }
    const int screenWidth = firstImage.width;
    const int screenHeight = firstImage.height;
    UnloadImage(firstImage); // Unload CPU copy, we'll reload it in the loop or load as texture

    InitWindow(screenWidth, screenHeight, "Magshimim Movie Player - Raylib");
    SetTargetFPS(60); // A good default, though WaitTime will cause variable frame display time

    while (!WindowShouldClose()) // Check for window close request
    {
        FrameNode* currentFrameNode = head; // Start from the beginning of the list for each repeat
        // int imgNum = 1; // If you need to track image number for messages

        while (currentFrameNode != NULL && !WindowShouldClose())
        {
            Image rayImage = LoadImage(currentFrameNode->frame->path);

            if (rayImage.data == NULL)
            {
                printf("Could not open or find image: %s (Frame: %s)\n", currentFrameNode->frame->path, currentFrameNode->frame->name);
                // Skip this frame or show a placeholder
                // For now, just advance and try to wait the duration
                WaitTime((float)currentFrameNode->frame->duration / 1000.0f);
                currentFrameNode = currentFrameNode->next;
                continue;
            }

            // Resize window if current image is larger than current window
            // This is optional and can be jarring.
            // A common approach is to fit image to window or use a fixed window size.
            // For simplicity here, we assume the initial window size is sufficient or
            // images are scaled down if needed (DrawTexturePro offers scaling).
            // if (rayImage.width > GetScreenWidth() || rayImage.height > GetScreenHeight()) {
            //     SetWindowSize(rayImage.width, rayImage.height);
            // }


            Texture2D texture = LoadTextureFromImage(rayImage);
            UnloadImage(rayImage); // Unload CPU image data once texture is on GPU

            BeginDrawing();
            ClearBackground(BLACK); // Clear screen to black (or any other color)

            // Calculate position to draw the texture centered (optional)
            int posX = (GetScreenWidth() - texture.width) / 2;
            int posY = (GetScreenHeight() - texture.height) / 2;
            if (posX < 0) posX = 0; // ensure it's not off-screen if image is wider
            if (posY < 0) posY = 0; // ensure it's not off-screen if image is taller


            DrawTexture(texture, posX, posY, WHITE); // Draw current frame
            EndDrawing();

            // Wait for the frame's duration
            // Convert duration from milliseconds to seconds for WaitTime
            WaitTime((float)currentFrameNode->frame->duration / 1000.0f);

            UnloadTexture(texture); // Release texture memory

            currentFrameNode = currentFrameNode->next;
            // imgNum++;
        }
        // list = head; // Already done by setting currentFrameNode = head at start of outer loop
        playCount++;
    }

    CloseWindow(); // Close window and free Raylib resources
}