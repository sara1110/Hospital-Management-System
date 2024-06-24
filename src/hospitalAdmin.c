#include "include\hospitalAdmin.h"

//Appointment apmntTempArr[50]; // REMOVE LATER
//EHR ehrTempArr[100]; // REMOVE LATER
// /Bill billTempArr[100]; // REMOVE LATER

//##########################################//
//######## MENU DISPLAY FUNCTIONS ##########//
//##########################################//

void hospAdminDisplayHospitalAdminMenu() {
    char *options[6] =
        {"1. User Management",
         "2. Schedule Management",
         "3. Bill Management",
         "4. EHR Management",
         "5. Inventory Management",
         "6. Logout"};

    Menu adminMenu = {
        .title = "HOSPITAL ADMINISTRATOR MENU",
        .noptions = 6,
        .options = options};

    printMenu(&adminMenu);
}

void hospAdminDisplayUserMngmntMenu() {
    char *options[4] =
        {"1. Patient Management",
         "2. Doctor Management",
         "3. Staff Nurse Management",
         "4. Go back"};
    Menu userMenu = {
        .title = "USER MANAGEMENT",
        .noptions = 4,
        .options = options};

    printMenu(&userMenu);
}

void hospAdminDisplayPatientMngmntMenu() {
    char *options[] = {
        "1. See all patients",
        "2. Search patient",
        "3. Modify patient's profile",
        "4. Register patient", 
        "5. Unregister patient",
        "6. Go back"
    };
    Menu patientMenu = {
        .title = "PATIENT MANAGEMENT",
        .noptions = 6,
        .options = options
    };

    printMenu(&patientMenu);
}

void hospAdminDisplayDoctorMngmtMenu() {
    char *options[] = {
        "1. See all doctors",
        "2. Search doctor",
        "3. Modify doctor's profile",
        "4. Register doctor", 
        "5. Unregister doctor",
        "6. Go back"
    };
    Menu doctorMenu = {
        .title = "DOCTOR MANAGEMENT",
        .noptions = 6,
        .options = options
    };

    printMenu(&doctorMenu);
}

void hospAdminDisplayStaffNurseMngmtMenu() {
    char *options[] = {
        "1. See all staff nurses",
        "2. Search staff nurse",
        "3. Modify staff nurse's profile",
        "4. Register staff nurse", 
        "5. Unregister staff nurse",
        "6. Go back"
    };
    Menu staffNurseMenu = {
        .title = "STAFF NURSE MANAGEMENT",
        .noptions = 6,
        .options = options
    };

    printMenu(&staffNurseMenu);
}

void displayAppointmentMngmntMenu() {
    char *options[] = {
        "1. See appointments at an specific day",
        "2. Add an appointment",
        "3. Reschedule an appointment",
        "4. Search for appointment (by ID)",
        "5. Cancel an appointment",
        "6. Go back"
    };
    Menu appointmentMenu = {
        .title = "APPOINTMENT MANAGEMENT ",
        .noptions = 6,
        .options = options
    };

    printMenu(&appointmentMenu);

}

void displayEHRMngmntMenu () {
    char* options[] = {
        "1. Search for a patient's EHR",
        "2. Modify a patient's EHR",
        "3. Go back"
    };
    Menu EHRmenu = {
        .title = "EHR MANAGEMENT",
        .noptions = 3,
        .options = options
    };

    printMenu(&EHRmenu);
}

void displayEHRmodifyMenu() {
    char *options[] = {
        "1. Set Patient Medical Condition",
        "2. Set Patient Prescription",
        "3. Set Patient Allergies",
        "4. Go back"
    };
    Menu modifyEHRmenu = {
        .title = "MODIFY PATIENT EHR",
        .noptions = 4,
        .options = options
    }; 

    printMenu(&modifyEHRmenu);
}

void displayBillMngmntMenu () {

}

void hospAdminDisplayInventoryMngmntMenu() {
    char* options[] = {
        "1. Display all the Inventory",
        "2. Search for an Item type",
        "3. Add an Item Type to the Inventory",
        "4. Add an Item Quantity to the Inventory",
        "5. Remove an Item Type from the Inventory",
        "6. Remove an Item Quantity from the Inventory",
        "7. Go back"
    };

    Menu invMngMenu = {
        .title="INVENTORY MANAGEMENT",
        .noptions=7,
        .options=options
    }; 

    printMenu(&invMngMenu);
}

void printDefaultSeparator() {
    char hbar[] = EXP_DOUBLEBAR_H;
    for(int i=0; i<DEFAULTSEPARATORLENGTH;i++) {
        printf("%s", hbar);
    }
    printf("\n");
}

//##############################//
//###### ENTRY POINT UI ########//
//##############################//

void hospitalAdminLogin(SystemState *sysState) 
{
    // REMOVE THIS LATER
    /*
    EHR tstEHR;
    tstEHR.ehrID=0;
    tstEHR.patientID=1;
    ehrTempArr[0] = tstEHR;
    strcpy(ehrTempArr[0].medicalHistory,  "Diagnosed_with_diabetes");
    //---------------------
    */

    if(NULL==sysState->adminPassw) {
        setCMDcolor(ANSI_RED);
        displayErrorMsg("FATAL ERROR: NULL HOSPITAL ADMIN PASSWORD");
        resetCMDcolor();
        waitForInput();
        return;
    }
    
    int accessGranted = 0;
    int exit = 0;
    
    do {
        clearScreen();
        char passInput[64]; 
        
        printDefaultSeparator();
        printYellow("HOSPITAL ADMIN LOGIN");
        printDefaultSeparator();
        
        printf("Enter Hospital Admin Password >> ");
        scanf("%s", passInput);
        printDefaultSeparator();

        // if both are equal
        if(!strcmp(sysState->adminPassw, passInput)) {
            
            displaySuccessMsg("Access granted");
            printYellow("< Input anything to continue >");

            waitForInput();

            accessGranted=1;
            hospitalAdminMenu(sysState); // enter to hospital menu
        } else {

            displayErrorMsg("Incorrect Password!");

            char errDecision[8];
            
            printYellow("< Input 'q' to exit, or any other input to try again >");
            printf(">>");
            scanf("%2s", errDecision);
            printf("\n");

            if(!strcmp(errDecision, "q")) {
                exit =1;
            }
        }
    } while (!accessGranted && !exit);
}

void hospitalAdminMenu(SystemState *sysState)
{
    int choice;
    int invalidChoice = 0;
    do
    {
        clearScreen();
        hospAdminDisplayHospitalAdminMenu();
        
        if (invalidChoice)
        {
            displayErrorMsg(INVALIDCHOICEMSG);
            
            invalidChoice = 0;
        }

        // Process input
        printf(">>");
        getSafeIntInput(&choice);

        switch (choice)
        {
        case 1: {
            hospAdminUserManagementMenu(sysState);
            break;
        }
        case 2: {
            hospAdminAppointmentMngmntMenu(sysState);
            break;
        }
        case 3: {
            hospAdminBillMngmntMenu(sysState);
            break;
        }
        case 4: {
            hospAdminEHRMngmntMenu(sysState);
            break;
        }
        case 5: {
            hospAdminInventoryMngmntMenu(sysState);
            break;
        }
        case 6: {
            break;
        }
        default: {
            invalidChoice = 1;
            break;
        }
        }

    } while (choice != 6);
}

void hospAdminUserManagementMenu(SystemState *sysState)
{
    int invalidChoice = 0;
    int choice;
    do
    {
        clearScreen();
        hospAdminDisplayUserMngmntMenu();
        if (invalidChoice)
        {
            /*
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            */
            displayErrorMsg(INVALIDCHOICEMSG);
            invalidChoice = 0;
        }

        // Get and Process Input
        printf(">>");
        getSafeIntInput(&choice);

        switch (choice)
        {
        case 1:
            hospAdminPatientManagementMenu(sysState);
            break;
        case 2:
            hospAdminDoctorManagementMenu(sysState);
            break;
        case 3:
            hospAdminStaffNurseManagementMenu(sysState);
            break;
        case 4:
            break;
        default:
            invalidChoice = 1;
            break;
        }

    } while (choice != 4);
}

//#####################################//
//######## PATIENT MANAGEMENT #########//
//#####################################//

void hospAdminPatientManagementMenu(SystemState *sysState) {
    int invalidChoice = 0;
    int choice;

    do {
        clearScreen();
        hospAdminDisplayPatientMngmntMenu();
        if (invalidChoice)
        {
            displayErrorMsg(INVALIDCHOICEMSG);
            invalidChoice = 0;
        }

        // Get and Process Input
        printf(">>");
        getSafeIntInput(&choice);

        switch(choice) {
            case 1:
                hospAdminSeeAllPatients(sysState);
                break;
            case 2:
                hospAdminSearchPatient(sysState);
                break;
            case 3:
                hospAdminModifyPatient(sysState);
                break;
            case 4:
                hospAdminRegisterPatient(sysState);
                break;
            case 5:
                hospAdminUnregisterPatient(sysState);
                break;
            case 6:
                break;
            default:
                invalidChoice=1;
                break;
        }
        
    } while (6!=choice);
}

void hospAdminSeeAllPatients(SystemState *sysState) {
    int nEntries = 0;

    clearScreen();

    printDefaultSeparator();
    printf("LOADING FROM PATIENT DATABASE\n");
    printDefaultSeparator();

    Patient* allPatients = getAllPatients(&nEntries);        
    printDefaultSeparator();

    if(0>nEntries || NULL==allPatients) {
        displayErrorMsg("Database loading error!");
        waitForInput();
        return;
    } else {
        displaySuccessMsg("Entries loaded successfully");
    }

    printYellow("| ID | IC/passport | name | password | isLocal |");

    int maxEntryDispCounter = 0;
    for(int i=0; i<nEntries;i++) {
        printPatientData(allPatients[i]);
        maxEntryDispCounter++;
        if(maxEntryDispCounter>=MAXENTRIESDISPLAY) {
            printYellow("< Enter anything to continue >");
            waitForInput();
            maxEntryDispCounter=0;
        }
    }
    
    printYellow("< Enter anything to go back >");
    waitForInput();

    free(allPatients);
    return;
}

void hospAdminSearchPatient(SystemState *sysState) {
    int inPatientID;

    clearScreen();
    printDefaultSeparator();
    printf("Enter Patient ID >>");

    getSafeIntInput(&inPatientID);

    printDefaultSeparator();
    printf("SEARCHING IN PATIENT DATABASE\n");
    printDefaultSeparator();

    Patient targetPatient = getPatient(inPatientID);
    printDefaultSeparator();

    if(targetPatient.pkID < 0) {
        displayErrorMsg("Patient not found...");
    } else {
        displaySuccessMsg("Patient found");

        printYellow("| ID | IC/passport | name | password | isLocal |");
        printPatientData(targetPatient);
    }

    printYellow("< Enter anything to go back >");

    waitForInput();
}

void hospAdminModifyPatient(SystemState *sysState) {

}

void hospAdminRegisterPatient(SystemState *sysState) {

    char tempBuffer[64];

    char inICPassport[64];
    char inName[64];
    char inPassw[64];
    int inIsLocal;

    int validatedAllInputs = 0;
    int inputFieldCounter = 0;
    clearScreen();
    printDefaultSeparator();
    
    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Patient IC/Passport >>");
                scanf("%63s", tempBuffer);
                strcpy(inICPassport, tempBuffer);

                // IC/Passport should not have special characters
                if(isAlphanumericOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    /*
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect IC/passport format, please input again\n");
                    resetCMDcolor();
                    */
                    displayErrorMsg("Incorrect IC/passport format, please input again");
                }

                break;
            }
            case 1: {
                printf("Enter Patient Name >>");
                scanf("%63s", tempBuffer);
                strcpy(inName, tempBuffer);

                // Name should alpha-only have Pascal Case format
                if(isAlphaOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    /*
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect name format, please input again\n");
                    resetCMDcolor();
                    */
                    displayErrorMsg("Incorrect name format, please input again");
                }               

                break;
            }
            case 2: {
                printf("Enter Patient Password >>");
                scanf("%63s", tempBuffer);
                strcpy(inPassw, tempBuffer);
                inputFieldCounter++;
                break;
            }
            case 3: {
                printf("Is the patient local? 1=yes, 0=no >>");
                getSafeIntInput(&inIsLocal);
                if(inIsLocal != 1 || inIsLocal !=0) {
                    displayErrorMsg("Invalid format, please enter either 1 or 0");
                } else {
                    validatedAllInputs=1;
                }

                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();
        
        scanf("%63s", tempBuffer);
        // Check the final decision
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
           displayErrorMsg(INVALIDINPUTMSG);
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {

        printDefaultSeparator();
        printf("ADDING PATIENT TO DATABASE\n");
        
        Patient newPatient;
        newPatient.pkID=0;
        strcpy(newPatient.personalID, inICPassport);
        strcpy(newPatient.name, inName);
        strcpy(newPatient.password, inPassw);
        newPatient.isLocal=inIsLocal;

        int addResult = addPatient(newPatient);
        printDefaultSeparator();

        if(-1!=addResult) {
            displaySuccessMsg("Patient registered successfully");
        }
        else {
            displayErrorMsg("Error when registering patient");
        }
    }
    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}

void hospAdminUnregisterPatient(SystemState *sysState) {
    char tempBuffer[64];
    int inPatientID;
    int IDValidated=0;

    clearScreen();
    printDefaultSeparator();

    do {
        printf("Enter patient ID >>");
        scanf("%8s", tempBuffer);
        
        if(isNumberOnlyString(tempBuffer)) {
            inPatientID = atoi(tempBuffer);
            IDValidated=1;
        } else {
            //printRed("!> Incorrect ID format, please input again\n");
            displayErrorMsg("Incorrect ID format, please input again");
        }

    } while (!IDValidated);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();

        scanf("%s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
            displayErrorMsg(INVALIDINPUTMSG);
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {        
        printDefaultSeparator();
        printf("REMOVING PATIENT FROM DATABASE\n");
        int removeResult = removePatient(inPatientID);

        if(-1!=removeResult) {
            displaySuccessMsg("Patient removed successfully");
        }
        else {
            displayErrorMsg("Error when removing patient");
        }
    }
    
    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}

//####################################//
//######## DOCTOR MANAGEMENT #########//
//####################################//

void hospAdminDoctorManagementMenu(SystemState *sysState) {
    int invalidChoice = 0;
    int choice;
    char tempBuffer[32];

    do {
        clearScreen();
        hospAdminDisplayDoctorMngmtMenu();
        if(invalidChoice) {
            displayErrorMsg(INVALIDCHOICEMSG);
            invalidChoice = 0;
        }

        // Get and Process Input
        printf(">>");
        getSafeIntInput(&choice);

        switch(choice) {
        case 1:
            hospAdminSeeAllDoctors(sysState);
            break;
        case 2:
            hospAdminSearchDoctor(sysState);
            break;
        case 3:
            hospAdminModifyDoctor(sysState);
            break;
        case 4:
            hospAdminRegisterDoctor(sysState);
            break;
        case 5:
            hospAdminUnregisterDoctor(sysState);
            break;
        case 6:
            break;
        default:
            invalidChoice=1;
            break;
        }

    } while(6!=choice);
}

void hospAdminSeeAllDoctors(SystemState *sysState) {
    int nEntries = 0;

    clearScreen();

    printDefaultSeparator();
    printf("LOADING FROM DOCTOR DATABASE\n");
    printDefaultSeparator();

    Doctor* allDoctors = getAllDoctors(&nEntries);        
    printDefaultSeparator();

    if(0>nEntries || NULL==allDoctors) {
        setCMDcolor(ANSI_RED);
        printf("!> Database loading error!\n");
        resetCMDcolor();
        scanf("%s");
        return;
    } else {
        setCMDcolor(ANSI_GREEN);
        printf(">> Entries loaded successfully\n");
        resetCMDcolor();
    }

    printDefaultSeparator();
    setCMDcolor(ANSI_YELLOW);
    printf("| ID | IC/passport | name | password | specialization(s) |\n");
    resetCMDcolor();

    int maxEntryDispCounter = 0;
    for(int i=0; i<nEntries;i++) {
        printDoctorData(allDoctors[i]);
        maxEntryDispCounter++;
        if(maxEntryDispCounter>=MAXENTRIESDISPLAY) {
            setCMDcolor(ANSI_YELLOW);
            printf("< Enter anything to continue >\n");
            resetCMDcolor();

            waitForInput();
            maxEntryDispCounter=0;
        }
    }

    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    
    //scanf("%d", &dummyInt);
    waitForInput();

    free(allDoctors);
    return;
}

void hospAdminSearchDoctor(SystemState *sysState) {
    int inDoctorID;
    char tempBuffer[16];

    clearScreen();
    printDefaultSeparator();
    printf("Enter Doctor ID >>");
    
    scanf("%15s", tempBuffer);
    inDoctorID = atoi(tempBuffer);
    printf("\n");

    printDefaultSeparator();
    printf("SEARCHING IN DOCTOR DATABASE\n");
    printDefaultSeparator();
    Doctor targetDoctor = getDoctor(inDoctorID);
    printDefaultSeparator();
    if(targetDoctor.doctorID < 0) {
        setCMDcolor(ANSI_RED);
        printf("!> Doctor not found...\n");
        resetCMDcolor();
    } else {
        setCMDcolor(ANSI_GREEN);
        printf(">> Doctor found\n");
        resetCMDcolor();

        setCMDcolor(ANSI_YELLOW);
        printf("| ID | IC/passport | name | password | specialization(s) |\n");
        resetCMDcolor();
        printDoctorData(targetDoctor);
    }
    
    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();
}

void hospAdminModifyDoctor(SystemState *sysState) {}

void hospAdminRegisterDoctor(SystemState *sysState) {
    //int inPatientID;
    char tempBuffer[64];

    char inICPassport[64];
    char inName[64];
    char inPassw[64];
    char specializationStr[256];

    int validatedAllInputs = 0;
    int inputFieldCounter = 0;
    clearScreen();
    printDefaultSeparator();
    
    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Doctor IC/Passport >>");
                scanf("%63s", tempBuffer);
                strcpy(inICPassport, tempBuffer);
                // IC/Passport should not have special characters
                if(isAlphanumericOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect IC/passport format, please input again\n");
                    resetCMDcolor();
                }

                break;
            }
            case 1: {
                printf("Enter Doctor Name >>");
                scanf("%63s", tempBuffer);
                strcpy(inName, tempBuffer);
                // Name should alpha-only have Pascal Case format
                if(isAlphaOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect name format, please input again\n");
                    resetCMDcolor();
                }               

                break;
            }
            case 2: {
                printf("Enter Doctor Password >>");
                scanf("%63s", tempBuffer);
                strcpy(inPassw, tempBuffer);
                inputFieldCounter++;
                break;
            }
            case 3: {
                printf("Enter the Doctor Specializations (as a string) >>");
                scanf("%63s", tempBuffer);
                strcpy(specializationStr, tempBuffer);
                //inIsLocal = atoi(tempBuffer);
                printf("\n");

                validatedAllInputs=1;
                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();
        scanf("%63s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("Invalid input, please try again...\n");
            resetCMDcolor();
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {

        printDefaultSeparator();
        printf("ADDING DOCTOR TO DATABASE\n");
        Doctor newDoctor;
        newDoctor.doctorID=0;
        strcpy(newDoctor.personalID, inICPassport);
        strcpy(newDoctor.name, inName);
        strcpy(newDoctor.password, inPassw);
        strcpy(newDoctor.specializationsStr, specializationStr);
        //newPatient.isLocal=inIsLocal;

        int addResult = addDoctor(newDoctor);
        printDefaultSeparator();

        if(-1!=addResult) {
            setCMDcolor(ANSI_GREEN);
            printf(">> Doctor registered successfully\n");
            resetCMDcolor();
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("!> Error when registering doctor\n");
            resetCMDcolor();
        }
    }
    printDefaultSeparator();

    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();
}

void hospAdminUnregisterDoctor(SystemState *sysState) {
    char tempBuffer[64];
    int inDoctorID;
    int IDValidated=0;

    clearScreen();
    printDefaultSeparator();

    do {
        printf("Enter Doctor ID >>");
        scanf("%63s", tempBuffer);
        if(isNumberOnlyString(tempBuffer)) {
            inDoctorID = atoi(tempBuffer);
            IDValidated=1;
        } else {
            setCMDcolor(ANSI_RED);
            printf("!> Incorrect name format, please input again\n");
            resetCMDcolor();
        }

    } while (!IDValidated);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();
        scanf("%s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("Invalid input, please try again...\n");
            resetCMDcolor();
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {        
        printDefaultSeparator();
        printf("REMOVING DOCTOR FROM DATABASE\n");
        int removeResult = removeDoctor(inDoctorID);

        if(-1!=removeResult) {
            setCMDcolor(ANSI_GREEN);
            printf(">> Doctor removed successfully\n");
            resetCMDcolor();
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("!> Error when removing doctor\n");
            resetCMDcolor();
        }
    }
    
    printDefaultSeparator();
    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();    
}

//#########################################//
//######## STAFF NURSE MANAGEMENT #########//
//#########################################//

void hospAdminStaffNurseManagementMenu(SystemState *sysState) {
    int invalidChoice = 0;
    int choice;
    char tempBuffer[32];

    do {
        clearScreen();
        hospAdminDisplayStaffNurseMngmtMenu();
        if(invalidChoice) {
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            invalidChoice = 0;
        }

        // Get and Process Input:
        // (convert string to int to avoid bug)
        printf(">>");
        scanf("%31s", tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");

        switch(choice) {
        case 1:
            hospAdminSeeAllStaffNurses(sysState);
            break;
        case 2:
            hospAdminSearchStaffNurse(sysState);
            break;
        case 3:
            hospAdminModifyStaffNurse(sysState);
            break;
        case 4:
            hospAdminRegisterStaffNurse(sysState);
            break;
        case 5:
            hospAdminUnregisterStaffNurse(sysState);
            break;
        case 6:
            break;
        default:
            invalidChoice=1;
            break;
        }        

    } while(6!=choice);
}

void hospAdminSeeAllStaffNurses(SystemState *sysState) {
    int nEntries = 0;

    clearScreen();

    printDefaultSeparator();
    printf("LOADING FROM STAFF NURSE DATABASE\n");
    printDefaultSeparator();

    StaffNurse* allStaffNurses = getAllStaffNurses(&nEntries);        
    printDefaultSeparator();

    if(0>nEntries || NULL==allStaffNurses) {
        setCMDcolor(ANSI_RED);
        printf("!> Database loading error!\n");
        resetCMDcolor();
        scanf("%s");
        return;
    } else {
        setCMDcolor(ANSI_GREEN);
        printf(">> Entries loaded successfully\n");
        resetCMDcolor();
    }

    printDefaultSeparator();
    setCMDcolor(ANSI_YELLOW);
    printf("| ID | IC/passport | name | password | functional unit(s) |\n");
    resetCMDcolor();

    int maxEntryDispCounter = 0;
    for(int i=0; i<nEntries;i++) {
        printStaffNurseData(allStaffNurses[i]);
        maxEntryDispCounter++;
        if(maxEntryDispCounter>=MAXENTRIESDISPLAY) {
            setCMDcolor(ANSI_YELLOW);
            printf("< Enter anything to continue >\n");
            resetCMDcolor();

            waitForInput();
            maxEntryDispCounter=0;
        }
    }

    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    
    waitForInput();

    free(allStaffNurses);
    return;
}

void hospAdminSearchStaffNurse(SystemState *sysState) {
    int inStaffNurseID;
    char tempBuffer[16];

    clearScreen();
    printDefaultSeparator();
    printf("Enter Staff Nurse ID >>");
    
    scanf("%15s", tempBuffer);
    inStaffNurseID = atoi(tempBuffer);
    printf("\n");

    printDefaultSeparator();
    printf("SEARCHING IN STAFF NURSE DATABASE\n");
    printDefaultSeparator();
    StaffNurse targetStaffNurse = getStaffNurse(inStaffNurseID);
    printDefaultSeparator();
    if(targetStaffNurse.staffNurseID < 0) {
        setCMDcolor(ANSI_RED);
        printf("!> Staff Nurse not found...\n");
        resetCMDcolor();
    } else {
        setCMDcolor(ANSI_GREEN);
        printf(">> Staff Nurse found\n");
        resetCMDcolor();

        setCMDcolor(ANSI_YELLOW);
        printf("| ID | IC/passport | name | password | functional unit(s) |\n");
        resetCMDcolor();
        printStaffNurseData(targetStaffNurse);
    }
    
    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();
}

void hospAdminModifyStaffNurse(SystemState *sysState) {}

void hospAdminRegisterStaffNurse(SystemState *sysState) {
    //int inPatientID;
    char tempBuffer[64];

    char inICPassport[64];
    char inName[64];
    char inPassw[64];
    char functionalUnitStr[256];

    int validatedAllInputs = 0;
    int inputFieldCounter = 0;
    clearScreen();
    printDefaultSeparator();
    
    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Staff Nurse IC/Passport >>");
                scanf("%63s", tempBuffer);
                strcpy(inICPassport, tempBuffer);
                // IC/Passport should not have special characters
                if(isAlphanumericOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect IC/passport format, please input again\n");
                    resetCMDcolor();
                }

                break;
            }
            case 1: {
                printf("Enter Staff Nurse Name >>");
                scanf("%63s", tempBuffer);
                strcpy(inName, tempBuffer);
                // Name should alpha-only have Pascal Case format
                if(isAlphaOnlyString(tempBuffer)) {
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect name format, please input again\n");
                    resetCMDcolor();
                }               

                break;
            }
            case 2: {
                printf("Enter Staff Nurse Password >>");
                scanf("%63s", tempBuffer);
                strcpy(inPassw, tempBuffer);
                inputFieldCounter++;
                break;
            }
            case 3: {
                printf("Enter the Staff Nurse functional unit (as a string) >>");
                scanf("%63s", tempBuffer);
                strcpy(functionalUnitStr, tempBuffer);
                //inIsLocal = atoi(tempBuffer);
                printf("\n");

                validatedAllInputs=1;
                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();
        scanf("%63s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("Invalid input, please try again...\n");
            resetCMDcolor();
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {

        printDefaultSeparator();
        printf("ADDING STAFF NURSE TO DATABASE\n");
        StaffNurse newStaffNurse;
        newStaffNurse.staffNurseID=0;
        strcpy(newStaffNurse.personalID, inICPassport);
        strcpy(newStaffNurse.name, inName);
        strcpy(newStaffNurse.password, inPassw);
        strcpy(newStaffNurse.functionalUnitStr, functionalUnitStr);
        //newPatient.isLocal=inIsLocal;

        int addResult = addStaffNurse(newStaffNurse);
        printDefaultSeparator();

        if(-1!=addResult) {
            setCMDcolor(ANSI_GREEN);
            printf(">> Staff Nurse registered successfully\n");
            resetCMDcolor();
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("!> Error when registering staff nurse\n");
            resetCMDcolor();
        }
    }
    printDefaultSeparator();

    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();    
}

void hospAdminUnregisterStaffNurse(SystemState *sysState) {
    char tempBuffer[64];
    int inStaffNurseID;
    int IDValidated=0;

    clearScreen();
    printDefaultSeparator();

    do {
        printf("Enter Staff Nurse ID >>");
        scanf("%63s", tempBuffer);
        if(isNumberOnlyString(tempBuffer)) {
            inStaffNurseID = atoi(tempBuffer);
            IDValidated=1;
        } else {
            setCMDcolor(ANSI_RED);
            printf("!> Incorrect name format, please input again\n");
            resetCMDcolor();
        }

    } while (!IDValidated);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >> ");
        resetCMDcolor();
        scanf("%s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        }
        else if(!strcmp("n", tempBuffer)) {
            operationCancelled=1;
            finalDecisionValid=1;
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("Invalid input, please try again...\n");
            resetCMDcolor();
        }
    } while(!finalDecisionValid);

    if(!operationCancelled) {        
        printDefaultSeparator();
        printf("REMOVING STAFF NURSE FROM DATABASE\n");
        int removeResult = removeStaffNurse(inStaffNurseID);

        if(-1!=removeResult) {
            setCMDcolor(ANSI_GREEN);
            printf(">> Staff Nurse removed successfully\n");
            resetCMDcolor();
        }
        else {
            setCMDcolor(ANSI_RED);
            printf("!> Error when removing staff nurse\n");
            resetCMDcolor();
        }
    }
    
    printDefaultSeparator();
    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();        
}

//##################################################//
//######## APPOINTMENT/SCHEDULE MANAGEMENT #########//
//##################################################//

void hospAdminAppointmentMngmntMenu(SystemState *sysState) {
    int invalidChoice = 0;
    int choice;
    char tempBuffer[32];

    do {
        clearScreen();
        displayAppointmentMngmntMenu();
        if(invalidChoice) {
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            invalidChoice = 0;
        }

        // Get and Process Input:
        // (convert string to int to avoid bug)
        printf(">>");
        scanf("%31s", tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");

        switch(choice) {
        case 1:
            hospAdminseeApointmentsAtSpfcDay(sysState);
            break;
        case 2: 
            hospAdminAddAppointment(sysState);
            break;
        case 3:
            hospAdminRescheduleAppointment(sysState);
            break;
        case 4:
            hospAdminSearchAppointment(sysState);
            break;
        case 5:
            hospAdminCancelAppointment(sysState);
            break;
        case 6:
            break;
        default:
            invalidChoice=1;
            break;
        }        

    } while(6!=choice);
}

void hospAdminseeApointmentsAtSpfcDay(SystemState *sysState) {

}

void hospAdminAddAppointment(SystemState *sysState) {

}

void hospAdminRescheduleAppointment(SystemState *sysState) {

}

void hospAdminSearchAppointment(SystemState *sysState) {

}

void hospAdminCancelAppointment(SystemState *sysState) {

}

//#######################################//
//##########  BILL MANAGEMENT  ##########//
//#######################################//

void hospAdminBillMngmntMenu(SystemState *sysState) {

}

void hospAdminSeeBillsFromUser(SystemState *sysState) {

}

void hospAdminSearchBillFromUser(SystemState *sysState) {

}
    
//#######################################//
//########### EHR MANAGEMENT ############//
//#######################################//

void hospAdminEHRMngmntMenu(SystemState* sysState) {
    int invalidChoice = 0;
    int choice;
    //char tempBuffer[32];

    do {
        clearScreen();
        displayEHRMngmntMenu();

        if(invalidChoice) {
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            invalidChoice = 0;
        }

        // Get and Process Input:
        // (convert string to int to avoid bug)
        
        printf(">>");
        getSafeIntInput(&choice);
        
        /*
        scanf("%31s", tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");
        */

        switch(choice) {
        case 1:
            hospAdminSearchEHRFromPatient(sysState);
            break;
        case 2:
            hospAdminSetPatientEHR(sysState);
            break;
        case 3:
            break;
        default:
            invalidChoice=1;
            break;
        }  

    } while(3!=choice);
}

/*
void hospAdminSeeEHRsFromPatient(SystemState *sysState) {

}
*/

void hospAdminSearchEHRFromPatient(SystemState *sysState) {
    //char tempBuffer[8];
    int inPatientID;

    clearScreen();
    printDefaultSeparator();

    printf("Enter patient ID >>");
    getSafeIntInput(&inPatientID);

    Patient targetEHRpatient = getPatient(inPatientID);
    EHR targetEHR;
    if(-1 != targetEHRpatient.pkID) {        
        // patient found
        printGreen(">> Patient found");

        // load the EHR entries
        int nEHRs;
        EHR* loadedEHRs = getAllEHRs(&nEHRs);

        // do linear search on EHRs
        int ehrFoundID = -1;
        for(int i=0; i<nEHRs; i++) {
            if(loadedEHRs[i].patientID==targetEHRpatient.pkID) {
                ehrFoundID=i;
                targetEHR = loadedEHRs[i];
                //strcpy(targetEHR.medicalHistory, ehrTempArr[i].medicalHistory);
            }
        }

        if(-1 != ehrFoundID) {
            printDefaultSeparator();
            printEHRData(targetEHR);
        } else {
            // error
            printRed("!> Fatal Error: User does not have an EHR");
        }

    } else {
        // error: patient not found
        printRed("!> Patient not found in database...");
    }

    printYellow("< Input anything to continue >");
    waitForInput();
}

void hospAdminSetPatientEHR(SystemState *sysState) {
    
    int inPatientID;

    clearScreen();
    printDefaultSeparator();

    printf("Enter patient ID >>");
    getSafeIntInput(&inPatientID);

    Patient targetEHRpatient = getPatient(inPatientID);
    EHR targetEHR;
    if(-1 != targetEHRpatient.pkID) {        
        // patient found
        printGreen(">> Patient found");
        
        // load the EHR entries
        int nEHRs;
        EHR* loadedEHRs = getAllEHRs(&nEHRs);

        // do linear search on EHRs
        int ehrFoundID = -1;
        for(int i=0; i<nEHRs; i++) {
            if(loadedEHRs[i].patientID==targetEHRpatient.pkID) {
                ehrFoundID=i;
            }
        }

        if(-1 != ehrFoundID) {
            sysState->currentlyLoggedUser.patient = targetEHRpatient;
        
        } else {
            // error
            printRed("!> Fatal Error: User does not have an EHR");
            printYellow("Input anything to continue");
            waitForInput();
            return;
        }
    } else {
        // error: patient not found
        printRed("!> Patient not found in database...");
        printYellow("Input anything to continue");
        waitForInput();
        return;
    }
    
    // Continue to available options if user and its EHR
    // is found

    int invalidChoice = 0;
    int choice;

    do {
        // clearScreen();
        printDefaultSeparator();
        displayEHRmodifyMenu();

        if(invalidChoice) {
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            invalidChoice = 0;
        }

        // Get and Process Input:
        // (convert string to int to avoid bug)
        
        printf(">>");
        getSafeIntInput(&choice);
        
        /*
        scanf("%31s", tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");
        */

        switch(choice) {
        case 1:
            hospAdminAddEHRHistory(sysState);
            break;
        case 2:
            hospAdminAddEHRPrescription(sysState);
            break;
        case 3:
            hospAdminAddEHRAllergy(sysState);
            break;
        case 4:
            break;
        default:
            invalidChoice=1;
            break;
        }  

    } while(4!=choice);
}

void hospAdminAddEHRHistory(SystemState *sysState) {

}

void hospAdminAddEHRPrescription(SystemState *sysState) {

}

void hospAdminAddEHRAllergy(SystemState *sysState) {

}

//#######################################//
//######## INVENTORY MANAGEMENT #########//
//#######################################//

void hospAdminInventoryMngmntMenu(SystemState *sysState)
{
    int invalidChoice = 0;
    int choice;
    char tempBuffer[10];

    do {
        clearScreen();
        hospAdminDisplayInventoryMngmntMenu();
        if(invalidChoice) {
            setCMDcolor(ANSI_RED);
            printf("Invalid choice, please try again...\n");
            resetCMDcolor();
            invalidChoice=0;
        }

        // Get and Process Input
        printf(">>");
        scanf("%s", tempBuffer);
        choice = atoi(tempBuffer);
        printf("\n");

        switch(choice) {
        case 1:
            hospAdminDisplayInventory(sysState);
            break;
        case 2: 
            hospAdminSearchInInventory(sysState);
            break;
        case 3:
            hospAdminRegisterInvItemType(sysState);
            break;
        case 4:
            hospAdminAddQuantityToInventory(sysState);
            break;
        case 5:
            hospAdminUnregisterInvItemType(sysState);
            break;
        case 6:
            hospAdminRemoveQuantityFromInventory(sysState);
            break;
        case 7: break;
        default: 
            invalidChoice=1;
            break;
        }

    } while(choice != 7);
}

void hospAdminDisplayInventory(SystemState *sysState) {

    int nEntries=0;

    clearScreen();

    printDefaultSeparator();
    printf("LOADING FROM INVENTORY DATABASE\n");
    printDefaultSeparator();

    InventoryItemType *allInvItems = getAllInventoryItems(&nEntries);
    printDefaultSeparator();

    if(0>nEntries || NULL==allInvItems) {
        displayErrorMsg("Database loading error!");
        waitForInput();
        return;
    } else {
        displaySuccessMsg("Entries loaded successfuly");
    }

    printDefaultSeparator();
    
    printYellow("| ID | name | description | category | quantity |");

    int maxEntryDispCounter=0;
    for(int i=0; i<nEntries; i++) {
        printInventoryItemData(allInvItems[i]);
        maxEntryDispCounter++;
        if(maxEntryDispCounter>=MAXENTRIESDISPLAY) {            
            printYellow("< Enter anything to continue >");
            waitForInput();
            maxEntryDispCounter=0;
        }
    }

    printYellow("< Enter anything to continue >");
    waitForInput();

    free(allInvItems);
    return;
}

void hospAdminSearchInInventory(SystemState *sysState) {
    int invItemID;
    char tempBuffer[16];

    clearScreen();
    printDefaultSeparator();
    printf("Enter Item ID >>");

    scanf("%15s", tempBuffer);
    invItemID = atoi(tempBuffer);
    printf("\n");

    printDefaultSeparator();
    printf("SEARCHING IN INVENTORY DATABASE\n");
    printDefaultSeparator();
    InventoryItemType targetInvItem = getInventoryItem(invItemID);
    printDefaultSeparator();

    if(targetInvItem.invItemID<0) {
        setCMDcolor(ANSI_RED);
        printf("!> Inventory Item not found...\n");
        resetCMDcolor();
    } else {
        setCMDcolor(ANSI_GREEN);
        printf(">> Inventory Item found\n");
        resetCMDcolor();

        setCMDcolor(ANSI_YELLOW);
        printf("| ID | name | description | quantity |\n");
        resetCMDcolor();
        printInventoryItemData(targetInvItem);
    }

    setCMDcolor(ANSI_YELLOW);
    printf("< Enter anything to go back >\n");
    resetCMDcolor();
    waitForInput();
}

void hospAdminRegisterInvItemType(SystemState *sysState) {

    char tempBuffer[256];

    char inName[64];
    char inDescription[256];
    char inCategory[64];

    int validatedAllInputs=0;
    int inputFieldCounter=0;
    clearScreen();
    printDefaultSeparator();

    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Item name >>");
                scanf("%63s", tempBuffer);
                
                // Item name cannot have special characters
                if(isAlphanumericOnlyString(tempBuffer)) {
                    strcpy(inName, tempBuffer);
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect Item name format, please input again\n");
                    resetCMDcolor();
                }
                break;
            }
            case 1: {
                printf("Enter Item description >>");
                scanf("%255s", tempBuffer);
                // Item description cannot have special characters
                if(isAlphanumericOnlyString(tempBuffer)) {
                    strcpy(inDescription, tempBuffer);
                    inputFieldCounter++;
                } else {
                    setCMDcolor(ANSI_RED);
                    printf("!> Incorrect Item description format, please input again\n");
                    resetCMDcolor();
                }
                break;
            }
            case 2: {
                printf("Enter Item Category >>");
                scanf("%63s", tempBuffer);
                
                // checking needed ?
                strcpy(inCategory, tempBuffer);

                inputFieldCounter++;
                validatedAllInputs=1;
                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >>");
        resetCMDcolor();

        scanf("%2s", tempBuffer);
        
        // Decision must be 'y' or 'n'
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        } else if(!strcmp("n", tempBuffer)) {
            finalDecisionValid=1;
            operationCancelled=1;
        } else {
            setCMDcolor(ANSI_RED);
            printf("Invalid input, please try again...\n");
            resetCMDcolor();
        }

    } while (!finalDecisionValid);

    if(!operationCancelled) {
        printDefaultSeparator();
        printf("ADDING INVENTORY ITEM TYPE TO DATABASE\n");
        InventoryItemType newItem;
        newItem.invItemID=0;
        strcpy(newItem.name, inName);
        strcpy(newItem.description, inDescription);
        strcpy(newItem.category, inCategory);
        newItem.quantity=0;

        int addResult = addInventoryItem(newItem);
        printDefaultSeparator();

        if(-1!=addResult) {
            displaySuccessMsg("Inventory Item registered successfuly");
        }
        else {
            displayErrorMsg("Database error during item registration!");
        }
    }
    
    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}

void hospAdminAddQuantityToInventory(SystemState *sysState) {
    char tempBuffer[32];
    int targetItemID;
    int quantityAddition;

    int validatedAllInputs=0;
    int inputFieldCounter=0;

    clearScreen();
    printDefaultSeparator();

    // Get and validate input
    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Item ID >>");
                scanf("%31s", tempBuffer);
                // Item ID should not have letters or special characters
                if(isNumberOnlyString(tempBuffer)) {
                    targetItemID = atoi(tempBuffer);
                    inputFieldCounter++;
                } else {
                    displayErrorMsg("Incorrect Item ID format, please try again");
                }
                break;
            }
            case 1: {
                printf("Enter quantity to add: ");
                scanf("%31s", tempBuffer);                
                // Quantity should not have letters or special characters
                if(isNumberOnlyString(tempBuffer)) {
                    quantityAddition = atoi(tempBuffer);

                    // Quantity should not be negative
                    if(quantityAddition >= 0) {
                        inputFieldCounter++;
                        validatedAllInputs=1;
                    } else {
                        displayErrorMsg("Quantity cannot be negative, please try again");
                    }
                    
                } else {
                    displayErrorMsg("Incorrect Quantity format, please try again");
                }
                
                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    printDefaultSeparator();
    printf("LOOKING FOR INVENTORY ITEM TYPE\n");
    printDefaultSeparator();
    InventoryItemType targetItem = getInventoryItem(targetItemID);
    if(-1!=targetItem.invItemID) {
        displaySuccessMsg("Inventory Item Type found");
        printf("Item type details:\n");
        printYellow("| ID | name | description | quantity |");
        printInventoryItemData(targetItem);
    } else {
        displayErrorMsg("Inventory Item Type not found...");
        waitForInput();
        return;
    }
    printDefaultSeparator();

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        printYellow("Are you sure you want to proceed? (y)es / (n)o >>");
        scanf("%2s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        } else if(!strcmp("n", tempBuffer)) {
            finalDecisionValid=1;
            operationCancelled=1;
        } else {
            printRed(INVALIDINPUTMSG);
        }

    } while(!finalDecisionValid);

    if(!operationCancelled) {
        printDefaultSeparator();
        printf("ADDING ITEM QUANTITY\n");
        printDefaultSeparator();

        targetItem.quantity+=quantityAddition;
        int setResult = setInventoryItem(targetItemID, targetItem);

        if(-1!=setResult) {
            displaySuccessMsg("Item quantity added successfuly");
        } else {
            displayErrorMsg("Error while adding item quantity");
        }
    }

    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}

void hospAdminUnregisterInvItemType(SystemState *sysState) {
    int inTargetID;
    int IDWasValidated=0;

    clearScreen();
    printDefaultSeparator();

    // Get and validate Item ID
    do {
        printf("Enter Item ID >>");
        getSafeIntInput(&inTargetID);
        
        if(inTargetID > 0) {
            IDWasValidated=1;
        } else {
            displayErrorMsg("Invalid input, please try again");
        }

    } while(!IDWasValidated);

    int finalDecisionValid=0;
    int operationCancelled=0;
    char tempBuffer[2];

    do {
        setCMDcolor(ANSI_YELLOW);
        printf("Are you sure you want to proceed? (y)es / (n)o >>");
        resetCMDcolor();

        scanf("%2s", tempBuffer);

        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        } else if(!strcmp("n", tempBuffer)) {
            finalDecisionValid=1;
            operationCancelled=1;
        } else {
            displayErrorMsg(INVALIDINPUTMSG);
        }

    } while(!finalDecisionValid);

    if(!operationCancelled) {
        printDefaultSeparator();
        printf("REMOVING INVENTORY ITEM TYPE FROM DATABASE\n");

        int removeResult = removeInventoryItem(inTargetID);
        printDefaultSeparator();

        if(-1!=removeResult) {
            displaySuccessMsg("Item successfuly unregistered");
        }
        else {
            displayErrorMsg("Database error when unregistering item!");
        }
    }

    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}

void hospAdminRemoveQuantityFromInventory(SystemState *sysState) { 
    char tempBuffer[32];
    int targetItemID;
    int quantitySubstraction;

    int validatedAllInputs=0;
    int inputFieldCounter=0;

    clearScreen();
    printDefaultSeparator();

    // Get and validate input
    do {
        switch(inputFieldCounter) {
            case 0: {
                printf("Enter Item ID >>");
                scanf("%31s", tempBuffer);
                // Item ID should not have letters or special characters
                if(isNumberOnlyString(tempBuffer)) {
                    targetItemID = atoi(tempBuffer);
                    inputFieldCounter++;
                } else {
                    displayErrorMsg("Incorrect Item ID format, please try again");
                }
                break;
            }
            case 1: {
                printf("Enter quantity to substract: ");
                scanf("%31s", tempBuffer);                
                // Quantity should not have letters or special characters
                if(isNumberOnlyString(tempBuffer)) {
                    quantitySubstraction = atoi(tempBuffer);

                    // Quantity should not be negative
                    if(quantitySubstraction >= 0) {
                        inputFieldCounter++;
                        validatedAllInputs=1;
                    } else {
                        displayErrorMsg("Quantity cannot be negative, please try again");
                    }
                    
                } else {
                    displayErrorMsg("Incorrect Quantity format, please try again");
                }
                
                break;
            }
            default:
                break;
        }
    } while(!validatedAllInputs);

    printDefaultSeparator();
    printf("LOOKING FOR INVENTORY ITEM TYPE\n");
    printDefaultSeparator();
    InventoryItemType targetItem = getInventoryItem(targetItemID);
    if(-1!=targetItem.invItemID) {
        displaySuccessMsg("Inventory Item Type found");
        printf("Item type details:\n");
        printYellow("| ID | name | description | quantity |");
        printInventoryItemData(targetItem);
    } else {
        displayErrorMsg("Inventory Item Type not found...");
        waitForInput();
        return;
    }
    printDefaultSeparator();

    int finalDecisionValid=0;
    int operationCancelled=0;
    do {
        printYellow("Are you sure you want to proceed? (y)es / (n)o >>");
        scanf("%2s", tempBuffer);
        if(!strcmp("y", tempBuffer)) {
            finalDecisionValid=1;
        } else if(!strcmp("n", tempBuffer)) {
            finalDecisionValid=1;
            operationCancelled=1;
        } else {
            printRed(INVALIDINPUTMSG);
        }

    } while(!finalDecisionValid);

    if(!operationCancelled) {
        printDefaultSeparator();
        printf("SUBSTRACTING ITEM QUANTITY\n");
        printDefaultSeparator();

        targetItem.quantity-=quantitySubstraction;
        
        // Make sure the quantity was not negative
        if(targetItem.quantity >= 0) {
            int setResult = setInventoryItem(targetItemID, targetItem);

            if(-1!=setResult) {
                displaySuccessMsg("Item quantity added successfuly");
            } else {
                displayErrorMsg("Error while adding item quantity");
            }
        } else {
            displayErrorMsg("Error: Substracted quantity is higher than the available item stock");
        }
    }

    printDefaultSeparator();
    printYellow("< Enter anything to go back >");
    waitForInput();
}