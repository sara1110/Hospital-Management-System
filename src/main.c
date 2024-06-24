#include "include\cmdui_utils.h"
#include "include\dataDefs.h"
#include "include\doctor.h"
#include "include\staffNurse.h"
#include "include\hospitalAdmin.h"
#include "include\patient.h"

void displayInitialMenu() {
    Menu initialMenu;
    initialMenu.title = "HOSPITAL MANAGEMENT SYSTEM ";
    initialMenu.noptions = 5;
    char *options[] = {
        "1. Patient Access",
        "2. Doctor Access",
        "3. Staff Nurse Access",
        "4. Hospital Admin Access",
        "5. Exit" };
    initialMenu.options=options;

    printMenu(&initialMenu);
}

void initialMenu(SystemState *sysState) {
    int choice;
    char tempBuffer[8];
    int invalidChoice=0;

    do {
        clearScreen();
        displayInitialMenu();
        if(invalidChoice) {
            displayErrorMsg(INVALIDCHOICEMSG);
            invalidChoice=0;
        }

        // Get input
        printf(">>");
        scanf("%7s", &tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");

        // Process input
        switch(choice) {
            case 1: {
                // Patient access
                patientAccess();
                break;
            }
            case 2: {
                // Doctor access
                doctorLoggedOutMenu(sysState);
                break;
            }
            case 3: {
                // Staff nurse access
                staffNurseLoggedOutMenu();
                break;
            }
            case 4: {
                // Hospital admin access
                hospitalAdminLogin(sysState);
                break;
            }
            case 5:
                break;
            default:
                invalidChoice=1;
                break;
        }

    } while(5 != choice);
}

int main(int argc, char **argv) {

    // This should be kept as a 'singleton' (only one instance)
    // representing the state of the system.
    SystemState sysState;

    // This should be later implemented 
    // in a specific file
    char adminPassw[] = "passpass";
    strcpy(sysState.adminPassw, adminPassw);

    // Trigger program start
    initialMenu(&sysState);
    
    // Once the program finishes, clear the screen again
    clearScreen();

    return 0;
}