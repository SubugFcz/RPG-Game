//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"

#include "globals.h"
#include "hardware.h"

///////////////////////////////
// Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP 0
#define BOTTOM 1
static void draw_speech_line(const char *line, int which);

///////////////////////////////
// Drawing function declarations
///////////////////////////////

//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics

// Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***

void draw_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************

    // 1. Draw a speech bubble at the bottom of the screen
    //  Hint: You can stack ULCD rectangles to make a border

    uLCD.filled_rectangle(0, 106, 128, 128, BLUE);
    uLCD.filled_rectangle(2, 108, 126, 128, BLACK);
}

void erase_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************

    // 1. Erase the speech bubble at the bottom of the screen
    //  Hint: You can overwrite the bubble with a empty (black) one

    uLCD.filled_rectangle(0, 106, 128, 128, BLACK);
    uLCD.filled_rectangle(0, 9, 128, 14, WHITE);     // Top
    uLCD.filled_rectangle(0, 13, 2, 114, WHITE);     // Left
    uLCD.filled_rectangle(0, 114, 128, 117, WHITE);  // Bottom
    uLCD.filled_rectangle(124, 14, 128, 117, WHITE); // Right
}

void draw_speech_line(const char *line, int which)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Set the location which line of text will go the uLCD
    // Hint: Change the y coordinate based on which line (top or bottom)

    int k = 14;
    if (which != 0)
    {
        k = 15;
    }

    // 2. For each character in the text, write it to uLCD
    for (int i = 0; line[i] && i < 17; i++) // We can simplify by limiting each line to 17 char
    {
        // TODO: Write the character
        uLCD.text_char(line[i], i + 1, k, LGREY);
        wait_ms(30);
    }
}

void speech_bubble_wait()
{
    //****************
    // TODO: Implement
    //****************

    // 1. Keep waiting until the action button is pressed
    //   Hint: What type of loop can we use to stay until a condition is met?

    // Keep waiting until the action button is pressed
    GameInputs inputs;
    bool wait = true;
    do
    {
        inputs = read_inputs();
        if (inputs.b1 == 0) // Action button is pressed
        {
            wait = false;
        }
    } while (wait);
}

void speech(const char *line1, const char *line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    // 2. Pass in to long_speech with the corresponding number of lines

    // Place the two lines into an array of lines
    const char *lines[2] = {line1, line2};
    // Call long_speech with the corresponding number of lines
    long_speech(lines, 2);
}

void long_speech(const char *lines[], int n)
{

    //****************
    // TODO: Implement
    //****************

    // 1. Create a speech bubble

    // 2. For each lines, display only two lines at a time
    //    If two lines are displayed, make sure to wait (call the wait function)

    // 3. Erase the speech bubble when you are done

    // Create a speech bubble
    draw_speech_bubble();
    // Display the lines two at a time
    for (int i = 0; i < n; i += 2)
    {
        // Display the first line
        draw_speech_line(lines[i], TOP);
        // If there is a second line, display it and wait for input
        if (i + 1 < n)
        {
            draw_speech_line(lines[i + 1], BOTTOM);
        }
        speech_bubble_wait();
        uLCD.filled_rectangle(0, 106, 128, 128, BLUE);
        uLCD.filled_rectangle(2, 108, 126, 128, BLACK);
    }
    // Erase the speech bubble when done
    erase_speech_bubble();
}
