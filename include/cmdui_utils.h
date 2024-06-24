#pragma once
#ifndef CMDUI_UTIL_H
#define CMDUI_UTIL_H

#ifdef _WIN32
#include<Windows.h>
#endif // _WIN32

#ifdef linux
#include <unistd.h>
#endif // linux

#include <stdio.h>
#include <string.h>

// ##### ANSI utility #####
typedef enum ANSIcolorEnum {
    ANSI_RED = 0,
    ANSI_GREEN,
    ANSI_YELLOW
} ANSIcolor;

typedef struct TerminalInfoStrc {
    int cmdColumns;
    int cmdRows;
} TerminalInfo;

/*
// Set cursor position at an specific position
void setCursorPosition(int x, int y);

// Set cursor position to 1,1
void resetCursorPosition();
*/

// Sets the terminal output to the color
// indicated by the ANSIcolor enum
void setCMDcolor(ANSIcolor clr);

// Resets the output color to its
// original state
void resetCMDcolor();

// Prints a line of green text
// (with '\n' line break)
void printGreen(char *txt);

// Prints a line of red text
// (with '\n' line break)
void printRed(char *txt);

// Prints a line of yellow text
// (with '\n' line break)
void printYellow(char *txt);

// Prints text in red using the '!>'
// prefix
void displayErrorMsg(char *errorMsg);

// Prints text in green using the '>>'
// prefix
void displaySuccessMsg(char *successMsg);

// Inband clearing command using 
// an ANSI code
void clearScreen();

// Waits for any user input, 
// halting execution
void waitForInput();

// Gets an integer from a char buffer
// safely, using atoi() function
void getSafeIntInput(int *input);

// Saves the contents of the opened 
// terminal to a buffer
void saveScreen(TerminalInfo* termInfo);

// Reloads the contents of the screen 
// buffer to the terminal
void restoreScreen(TerminalInfo* termInfo);

// ###### UI Utility ######

// Fast way to represent a printable Menu
typedef struct menuStrc
{
    char *title;
    int noptions;
    char **options;
} Menu;

#define THICKMENUBAR "|--|"
#define EXP_DOUBLEBAR_V {(char)186, '\0'}; // expanded ASCII double vertical bar
#define EXP_DOUBLEBAR_H {(char)205, '\0'}; // expanded ASCII double horizontal bar

// Prints a piece of text inside a box, in
// the same style as a Menu title
void printTextBox(char target[]);

// Prints a menu out of a Menu struct representing it
// (later move this to an utility file)
void printMenu(Menu *menuptr);

// #######################

#endif // CMDUI_UTIL_H