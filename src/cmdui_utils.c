#include "include\cmdui_utils.h"

/*
// Set cursor position at an specific position
void setCursorPosition(int x, int y) {
    #ifdef _WIN32
    HANDLE cmdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {.X=x,.Y=y};
    SetConsoleCursorPosition(cmdHandle, coord);
    #endif
}

// Set cursor position to 1,1
void resetCursorPosition() {
    #ifdef _WIN32
    HANDLE cmdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {.X=0,.Y=0};
    SetConsoleCursorPosition(cmdHandle, coord);
    #endif
}
*/

// Sets the terminal output to the color
// indicated by the ANSIcolor enum
void setCMDcolor(ANSIcolor clr) {
    switch(clr) {
        case ANSI_RED:
            printf("\033[31m");
            break;
        case ANSI_GREEN:
            printf("\033[32m");
            break;
        case ANSI_YELLOW:
            printf("\033[33m");
            break;
        default:
            break;
    }
}

// Prints a line of green text
// (with '\n' line break)
void printGreen(char *txt) {
    setCMDcolor(ANSI_GREEN);
    printf("%s\n", txt);
    resetCMDcolor();
}

// Prints a line of red text 
// (with '\n' line break)
void printRed(char *txt) {
    setCMDcolor(ANSI_RED);
    printf("%s\n", txt);
    resetCMDcolor();
}

// Prints a line of yellow text
// (with '\n' line break)
void printYellow(char *txt) {
    setCMDcolor(ANSI_YELLOW);
    printf("%s\n", txt);
    resetCMDcolor();
}

// Prints text in red using the '!>'
// prefix
void displayErrorMsg(char *errorMsg) {
    setCMDcolor(ANSI_RED);
    printf("!> %s\n", errorMsg);
    resetCMDcolor();
}

// Prints text in green using the '>>'
// prefix
void displaySuccessMsg(char *successMsg) {
    setCMDcolor(ANSI_GREEN);
    printf(">> %s\n", successMsg);
    resetCMDcolor();
}

// Resets the output color to its
// original state
void resetCMDcolor() {
    printf("\033[m");
}

// Inband clearing command using 
// an ANSI code
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #endif
    #ifdef __linux__
        system("clear");
    #endif
}

// Waits for any user input, 
// halting execution
void waitForInput() {
    fflush(stdin); 
    getchar();
    fflush(stdin);
}

// Gets an integer from a char buffer
// safely, using atoi() function
void getSafeIntInput(int *input) {
    char tempBuffer[16];
    scanf("%16s", tempBuffer);
    int inInt = atoi(tempBuffer);
    printf("\n");

    *input=inInt;
}

/*
// Saves the contents of the opened 
// terminal to a buffer (??)
void saveScreen(TerminalInfo* termInfo) {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        termInfo->cmdColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        termInfo->cmdRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #endif // _WIN32

    #ifdef linux
    #endif // linux
}

// Reloads the contents of the screen 
// buffer to the terminal
void restoreScreen(TerminalInfo* termInfo) {
    #ifdef _WIN32
        
    #endif // _WIN32

    #ifdef linux
    #endif // linux
}
*/

void printTextBox(char target[]) {

    int boxWidth=0;
    int textWidth = strlen(target);
    char vbarStr[] = EXP_DOUBLEBAR_V;
    char hbarStr[] = EXP_DOUBLEBAR_H;
    int barLength = strlen(vbarStr);

    boxWidth+= textWidth+(2*barLength)+2;

    // First print the top
    for (int i = 0; i < boxWidth; i++)
        printf(hbarStr);
    printf("\n");

    printf(vbarStr);

    // at least one space before the string
    printf(" ");
    // print left whitespace padding to center the title
    int sidepadding = (boxWidth - ((2 * barLength) + (boxWidth + 2))) / 2;
    for (int i = 0; i < sidepadding; i++)
        printf(" ");
    
    // Print title line in yellow
    setCMDcolor(ANSI_YELLOW);
    printf(target);
    resetCMDcolor();

    // print right padding
    for (int i = 0; i < sidepadding; i++)
        printf(" ");
    printf(" ");

    //if (0 == textWidth % 2)
        //printf(" "); // fix alignment for even text width
    
    // (odd width is normally aligned)
    printf(vbarStr);
    printf("\n");

    // Print title/options separator
    printf(vbarStr);
    // padding to the right
    for (int i = 0; i < boxWidth - (2 * barLength); i++)
        printf(hbarStr);
    printf(vbarStr);
    printf("\n");
}

// Prints a menu out of a Menu struct representing it
// (later move this to an utility file ?)
void printMenu(Menu *menuptr) {
    int menuWidth = 0;
    int titleWidth = strlen(menuptr->title);
    char vbarStr[] = EXP_DOUBLEBAR_V;
    char hbarStr[] = EXP_DOUBLEBAR_H;
    int barLength = strlen(vbarStr);
    int maxOptionWidth = 0;

    // Calculate the max length among options
    for (int i = 0; i < menuptr->noptions; i++)
    {
        int nextLen = strlen(menuptr->options[i]);
        maxOptionWidth = (nextLen > maxOptionWidth) ? nextLen : maxOptionWidth;
    }

    // Calculate final menu size
    menuWidth = (titleWidth > maxOptionWidth) ? titleWidth : maxOptionWidth;
    menuWidth += (2 * barLength) + 2;

    // Print the menu:

    // First print the top
    for (int i = 0; i < menuWidth; i++)
        printf(hbarStr);
    printf("\n");

    printf(vbarStr);

    // at least one space before the string
    printf(" ");
    // print left whitespace padding to center the title
    int sidepadding = (menuWidth - ((2 * barLength) + (titleWidth + 2))) / 2;
    for (int i = 0; i < sidepadding; i++)
        printf(" ");
    
    // Print title line in yellow
    setCMDcolor(ANSI_YELLOW);
    printf(menuptr->title);
    resetCMDcolor();

    // print right padding
    for (int i = 0; i < sidepadding; i++)
        printf(" ");
    printf(" ");

    if (0 == titleWidth % 2)
        printf(" "); // fix alignment for even title width

    // (odd width is normally aligned)
    printf(vbarStr);
    printf("\n");

    // Print title/options separator
    printf(vbarStr);
    // padding to the right
    for (int i = 0; i < menuWidth - (2 * barLength); i++)
        printf(hbarStr);
    printf(vbarStr);
    printf("\n");

    // Print options
    for (int i = 0; i < menuptr->noptions; i++)
    {
        // int offset = barLength + 1 + strlen(menuptr->options[i]);
        // int offsetPadding = menuWidth - (offsetPadding + barLength);
        int offsetPadding = (menuWidth - ((2 * barLength) + (strlen(menuptr->options[i]) + 2)));

        printf(vbarStr);
        printf(" ");
        printf(menuptr->options[i]);

        // whitespace padding on the right of the option
        for (int j = 0; j < offsetPadding; j++)
            printf(" ");
        printf(" ");
        printf(vbarStr);
        printf("\n");
    }

    // Print bottom line
    printf(vbarStr);
    for (int i = 0; i < menuWidth - (2 * barLength); i++)
        printf(hbarStr);
    printf(vbarStr);
    printf("\n");
}