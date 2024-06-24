#include "include\doctor.h"

int doctorValidateUser(const char* inUsername, const char* inPassword, SystemState *sysState) {
    int nDoctors;
    Doctor *doctors = getAllDoctors(&nDoctors);
    
    for (int i = 0; i < nDoctors; i++) {
        if (
            strcmp(doctors[i].name, inUsername) == 0 && 
            strcmp(doctors[i].password, inPassword) == 0) {
            
            sysState->currentlyLoggedUser.doctor = doctors[i];
            free(doctors);
            return 1; // Valid doctor
        }
    }
    free(doctors);
    return 0; // Invalid doctor
}

void doctorAddAppointment(SystemState *sysState) 
{
    clearScreen();
    char dateStr[9];
    char timeStr[7];
    int patientID;
    //char doctorName[50];

    printf("Enter date (DDMMYYYY): ");
    scanf("%8s", dateStr);
    printf("Enter time (HHMMSS): ");
    scanf("%6s", timeStr);
    printf("Enter patient ID:");
    scanf("%d", &patientID);

    Appointment newApmnt;

    // Convert string to Date
    newApmnt.patientID = patientID;
    strToDate(&newApmnt.appointmentDay, dateStr);
    strToDayTime(&newApmnt.startTime, timeStr);
    newApmnt.doctorID = sysState->currentlyLoggedUser.doctor.doctorID;

    int newApmntID = addAppointment(newApmnt);

    if(newApmntID != -1) {
        printf("Appointment Added successfuly!\n");
    } else {
        printf("Error when adding apointment\n");
    }
}

void doctorDisplayLoggedOutMenu() {
    printf("\n*** Doctor Access ***\n");
    printf("1. Login\n");
    printf("2. Go back\n");
    printf("Enter your choice: ");
}

void doctorDisplayLoggedInMenu() {
    printf("\n*** User Menu ***\n");
    printf("1. Add Appointment\n");
    printf("2. Secure EHR Access\n");
    printf("3. Reporting and Analytics\n");
    printf("4. Logout\n");
    printf("Enter your choice: ");
}

void doctorSecureEHRAccess(SystemState *sysState) {
    clearScreen();
    int choice;
    do {
        printf("\n*** Secure EHR Access ***\n");
        printf("1. Display Patient EHR\n");
        printf("2. Modify Patient EHR\n");
        printf("3. Return to Doctor Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                doctorViewPatientEHRByID(sysState);
                break;
            case 2:
                doctorModifyPatientEHR(sysState);
                break;
            case 3:
                printf("Returning to doctor menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void displayMonthlyBarChart() {

}

void displayYearlyBarChart() {

}

void doctorReportingAndAnalytics(SystemState *sysState) {
    clearScreen();
    int choice;
    do {
        printf("\n*** Reporting and Analytics ***\n");
        printf("1. View Number of Appointments\n");
        printf("2. View All Appointments\n");
        printf("3. View Todays's appointments\n");
        printf("4. Show a Bar Chart of Appointments in a Month (per Day))\n");
        printf("5. Show a Bar Chart of Appointments in a Year (per Year)\n");
        printf("6. Return to Doctor Menu\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                int nAppointments;
                Appointment* apmnts = getAllAppointments(&nAppointments);
                clearScreen();
                printf("Total number of appointments: %d\n", nAppointments);
                free(apmnts);
                break;
            }
            case 2:
                doctorViewAllAppointments(sysState);
                break;
            case 3:
                doctorViewTodaysAppointments(sysState);
                break;
            case 4:
                doctorShowMonthlyAppointments(sysState);
                break;
            case 5:

                break;
            case 6:
                // exit
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void doctorViewAllAppointments(SystemState *sysState) {
    
    int nAppointments;
    Appointment* apmnts = getAllAppointments(&nAppointments);
    char iapmntDateStr[9];
    iapmntDateStr[8] = '\0';
    char iapmntTimeStr[7];
    iapmntTimeStr[6] = '\0';
    
    clearScreen();
    printf("\n*** Appointments for Dr. %s ***\n", sysState->currentlyLoggedUser.doctor.name);
    for(int i=0; i<nAppointments; i++) {
        if(apmnts[i].doctorID==sysState->currentlyLoggedUser.doctor.doctorID) {

            printf("\n***** [%d] *****\n", apmnts[i].appointmentID);
            printf("Patient ID: %d\n", apmnts[i].patientID);
            
            dateToStr(iapmntDateStr, &apmnts[i].appointmentDay);
            printf("Date (DDMMYYYY): %8s\n", 
                iapmntDateStr
            );

            dayTimeToStr(iapmntTimeStr, &apmnts[i].startTime);
            printf("Start Time (HHMMSS): %6s\n",
                iapmntTimeStr
            );

            printf("**************\n");

        }
    }    
    
    free(apmnts);
}

void doctorViewTodaysAppointments(SystemState *sysState) {
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[20];
    clearStr(today);
    today[8] = '\0';

    Date todaysDate;
    //sprintf(today, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    sprintf(todaysDate.dateInChars.dayDigits,   "%02d", tm.tm_mday);
    sprintf(todaysDate.dateInChars.monthDigits, "%02d", tm.tm_mon + 1);
    sprintf(todaysDate.dateInChars.yearDigits,  "%04d", tm.tm_year+1900);

    dateToStr(today, &todaysDate);
    int nAppointments;
    Appointment* apmnts = getAllAppointments(&nAppointments);

    clearScreen();
    printf("Todays date (DDMMYYYY): %s\n", today);
    printf("\n*** Today's Appointments for Dr. %s ***\n", sysState->currentlyLoggedUser.doctor.name);

    for (int i = 0; i < nAppointments; i++) {
        
        char iapmntDateStr[9];
        iapmntDateStr[8] = '\0';
        char iapmntTimeStr[7];
        iapmntTimeStr[6] = '\0';
        dateToStr(iapmntDateStr, &apmnts[i].appointmentDay);
        dayTimeToStr(iapmntTimeStr, &apmnts[i].startTime);
        
        if(
            apmnts[i].doctorID==sysState->currentlyLoggedUser.doctor.doctorID
            && !strcmp(today, iapmntDateStr)) {
                
            printf("\n***** [%d] *****\n", apmnts[i].appointmentID);
            printf("Patient ID: %d\n", apmnts[i].patientID);
            
            printf("Date (DDMMYYYY): %8s\n", 
                iapmntDateStr
            );

            printf("Start Time (HHMMSS): %6s\n",
                iapmntTimeStr
            );

            printf("**************\n");
        }
    }
    
    free(apmnts);
}

void doctorShowMonthlyAppointments(SystemState *sysState) {

    char labels[31][LABELSTRSIZE];
    for(int i=0; i<31;i++) {
        if(i < 10) {
            sprintf(labels[i], "   %d", i+1);
        } else {
            sprintf(labels[i], "  %d", i+1);
        }
    }

    int heights[31];
    for(int i=0; i<31; i++) {
        heights[i]=0;
    }
    heights[30] = 20;

    int nApmnts;
    Appointment *apmnt = getAllAppointments(&nApmnts);
    //int dayCounter=1;
    char targetMonth[] = "01";
    char safeBuffer[3];
    safeBuffer[2]='\0';

    for(int i=0; i<nApmnts; i++) {
        strcpy(safeBuffer, apmnt[i].appointmentDay.dateInChars.dayDigits);
        int intDay = atoi(safeBuffer);
        printf("safebuffer == %s\n", safeBuffer);
        printf("atoi() day = %d\n", intDay);
        if(
            !strcmp(
                apmnt[i].appointmentDay.dateInChars.monthDigits,
                targetMonth
        ))
        {
            heights[intDay-1]++;
        }
    }

    int nValues=31;

    BarChart chart = generateBarChart(labels, heights, nValues, DEFBARDIFF);
    printf("Bar chart generated!\n");

    printBarChart(chart);

    printf("\n\n");
    getchar();

    freeBarChart(&chart);

}

void doctorShowYearlyAppointments(SystemState *sysState) {

}

void doctorViewPatientEHRByID(SystemState *sysState) {
    clearScreen();
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer

    Patient targetPatient = getPatient(patientID);

    if(targetPatient.pkID != -1) {

        int nEHRs;
        EHR *ehrs = getAllEHRs(&nEHRs);
        EHR targetEHR;

        int foundEHR=-1;
        for(int i=0; i<nEHRs; i++) {
            if(ehrs[i].patientID==targetPatient.pkID) {
                foundEHR=i;
                targetEHR= ehrs[i];
                break;
            }
        }

        clearScreen();
        if(foundEHR != -1) {
            
            char readableBuffer[150];
            clearStr(readableBuffer);
            strcpy(readableBuffer, targetEHR.medicalHistory);
            replaceCharBy(readableBuffer, '_', ' ');
            printf("\n*** MEDICAL HISTORY ***\n");
            printf("%s\n", readableBuffer);

            clearStr(readableBuffer);
            strcpy(readableBuffer, targetEHR.prescriptions);
            replaceCharBy(readableBuffer, '_', ' ');
            printf("\n*** PRESCRIPTIONS ***\n");
            printf("%s\n", readableBuffer);

            clearStr(readableBuffer);
            strcpy(readableBuffer, targetEHR.allergies);
            replaceCharBy(readableBuffer, '_', ' ');
            printf("\n*** ALLERGIES ***\n");
            printf("%s\n", readableBuffer);
        
        } else {
            printf("Fatal error: Patient does not have EHR!\n");
        }

        free(ehrs);

    } else {
        printf("Patient does not exist...\n");
    }

    getchar();
}

void doctorModifyPatientEHR(SystemState *sysState) {

    /*
    clearScreen();    
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer
    */

    int choice;
    do {
        clearScreen();
        printf("\n*** Modify Patient EHR ***\n");
        printf("1. Update Medical History\n");
        printf("2. Update Prescription\n");
        printf("3. Update Allergies\n");
        printf("4. Return\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer

        switch (choice) {
            case 1:
                doctorUpdateMedicalHistory();
                break;
            case 2:
                doctorUpdatePrescription();
                break;
            case 3:
                doctorUpdateAllergies();
                break;
            case 4:
                printf("Returning to secure EHR access...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

void doctorUpdateMedicalHistory() {
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    
    Patient targetPatient = getPatient(patientID); 

    if(targetPatient.pkID == -1) {
        printf("Patient does not exist...\n");
        getchar();
        return;
    }

    int nEHRs;
    EHR *ehrs = getAllEHRs(&nEHRs);
    int foundEHRID = -1;
    for(int i=0; i<nEHRs; i++) {
        if(targetPatient.pkID==ehrs[i].ehrID) {
            foundEHRID=i;
            break;
        }
    }

    if(foundEHRID == -1) {
        printf("Error: EHR does not exist!\n");
        getchar();
        return;
    }

    EHR newEHR = ehrs[foundEHRID];
    free(ehrs);
    
    printf("Enter new medical history: ");
    char buffer[100];
    fflush(stdin);
    fgets(buffer, 99, stdin);
    
    replaceCharBy(buffer, ' ', '_'); // replace white space by underscore

    strcpy(newEHR.medicalHistory, buffer);
    int setStatus = setEHR(foundEHRID, newEHR);

    if(setStatus != -1) {
        printf("Medical history updated successfully.\n");
    } else {
        printf("Unknown while updating medical history!\n");
    }
}

void doctorUpdatePrescription() {
    /*
    printf("Enter new prescription: ");
    fgets(patient->prescription, 255, stdin);
    patient->prescription[strcspn(patient->prescription, "\n")] = '\0';  // Remove newline character
    printf("Prescription updated successfully.\n");
    */
}

void doctorUpdateAllergies() {
    /*
    printf("Enter new allergies: ");
    fgets(patient->allergies, 255, stdin);
    patient->allergies[strcspn(patient->allergies, "\n")] = '\0';  // Remove newline character
    printf("Allergies updated successfully.\n");
    */
}

void doctorLogin(SystemState *sysState) {
    clearScreen();
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (doctorValidateUser(username, password, sysState)) {
        printf("Login successful!\n");
        int choice;
        do {
            clearScreen();
            doctorDisplayLoggedInMenu();
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    doctorAddAppointment(sysState);
                    break;
                case 2:
                    doctorSecureEHRAccess(sysState);
                    break;
                case 3:
                    doctorReportingAndAnalytics(sysState);
                    break;
                case 4:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 4);
    } else {
        printf("Invalid credentials. Please try again.\n");
    }
}

void doctorLoggedOutMenu(SystemState *sysState) {
    clearScreen();
    int choice;
    do {
        doctorDisplayLoggedOutMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                doctorLogin(sysState);
                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 2);

}
