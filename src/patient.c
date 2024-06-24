#include "include\patient.h"

/*
PatientPS patients[MAX_PATIENTS_PS];
*/

Patient loggedInPatient; 

//LOGIN FUNCTION 
bool patientLogin(const char* username, const char* password) {
    
    bool patientFound = false;

    int nPatients;
    Patient* patients = getAllPatients(&nPatients);
 
    for (int i = 0; i < nPatients; i++) {
        if ((strcmp(patients[i].name, username) == 0)
        && (strcmp(patients[i].password, password) == 0)) {
            
            loggedInPatient = patients[i];
            patientFound = true;

            break;
        }
    }

    free(patients);
    return patientFound;
}

//APPOINTMENTS MENU 
void patientViewAppointments() {

    int nApmnts;
    Appointment *apmnts = getAllAppointments(&nApmnts);

    printf("Appointments\n");

    int appointmentCount = 0;
    for (int i = 0; i < nApmnts; i++) {
        if (apmnts[i].patientID == loggedInPatient.pkID) {
            printf("-------------------------------\n");
            printf("Appointment %d:\n", apmnts[i].appointmentID);
            printf("Doctor ID: %d\n", apmnts[i].doctorID);
            
            char dateStr[9];
            dateToStr(dateStr, &apmnts[i].appointmentDay);
            char timeStr[7];
            dayTimeToStr(timeStr, &apmnts[i].startTime);
            printf("Date: %s\n", dateStr);
            printf("Time: %s\n", timeStr);
            printf("-------------------------------\n");
            appointmentCount++;
        }
    }

    if (appointmentCount == 0) {
        printf("No appointments found.\n");
    }
}

void patientRescheduleAppointment() {
    
    printf("Rescheduled Appointment\n");
    printf("Enter appointment ID: ");
    int targetAppointmentID;
    scanf("%d", &targetAppointmentID);

    Appointment targetApmnt = getAppointment(targetAppointmentID);

    if(targetApmnt.appointmentID == -1) {
        printf("Error: Appointment does not exist...\n");
        getchar();
        return;
    }

    if(targetApmnt.patientID != loggedInPatient.pkID) {
        printf("Error: This appointment does not belong to you...\n");
        getchar();
        return;
    }

    printf("Enter new preferred date (DDMMYYYY): "); 
    char dateStr[9];
    scanf("%s", dateStr);
    if(NULL==dateStr) {
        printf("Error: Invalid date format\n");
        getchar();
        return;
    }
    strToDate(&targetApmnt.appointmentDay, dateStr);

    printf("Enter new preferred time (HHMMSS): ");
    char timeStr[7];
    scanf("%s", timeStr);
    if(NULL==timeStr) {
        printf("Error: Invalid day time format\n");
        getchar();
        return;
    }
    strToDayTime(&targetApmnt.startTime, timeStr);

    // Remove and re-add appointment
    removeAppointment(targetAppointmentID);
    int reScheduleStatus = addAppointment(targetApmnt);

    if(reScheduleStatus != -1) {
        printf("Appointment successfuly re-scheduled\n");
    } else {
        printf("Database error when re-scheduling appointment!\n");
    }
}

void patientCancelAppointment() {
    int appointmentNumber;
    printf("Enter the appointment ID to cancel: ");
    scanf("%d", &appointmentNumber);

    Appointment target = getAppointment(appointmentNumber);
    if(target.appointmentID == -1) {
        printf("Error: Appointment not found...\n");
        getchar();
        return;
    }

    if(target.patientID != loggedInPatient.pkID) {
        printf("Error: This appointment does not belong to you\n");
        getchar();
        return;
    }

    int cancelStatus = removeAppointment(appointmentNumber);
    if(cancelStatus != -1) {
        printf("Appointment cancelled successfuly\n");
    } else {
        printf("Database Error when removing appointment!\n");
    }
}

void patientBookAppointment() {

    printf("Enter doctor ID: ");
    int doctorID;
    scanf("%d", &doctorID);

    if(doctorID<0) {
        printf("Error, invalid doctor ID!");
        getchar();
        return;
    }

    printf("Enter date (DDMMYYYY): ");
    char dateStr[9];
    scanf("%s", dateStr);
    //Date date;

    printf("Enter time (HHMMSS): ");
    char timeStr[7];
    scanf("%s", timeStr);
    //DayTime time;

    Appointment newApmnt;
    newApmnt.appointmentID=0;
    newApmnt.doctorID=doctorID;
    strToDate(&newApmnt.appointmentDay, dateStr);
    strToDayTime(&newApmnt.startTime, timeStr);
    
    int addAppointmentID = addAppointment(newApmnt);
    if(addAppointmentID != -1) {
        printf("Appointment successfuly added!\n");
        printf("Your appointment ID: %d\n", addAppointmentID);
    } else {
        printf("Error when adding appointment!\n");
    }
    getchar();
}

//EHR SECTION 
/*
void patientInitializeEHR(EHRPS *ehr, int patientID) {
    ehr->ehrID = patientID;
    ehr->patientID = patientID; // Initialize patientID here
    strcpy(ehr->medicalHistory, "");
    strcpy(ehr->prescriptions, "");
    strcpy(ehr->billingInformation, "");
    strcpy(ehr->allergies, "");
}
*/

void patientDisplayEHR() {

    printf("Loading EHR...\n");
    
    int nEHRs;
    EHR *ehrs = getAllEHRs(&nEHRs);
    EHR targetEHR;
    int foundID = -1;
    for(int i=0; i<nEHRs; i++) {
        if(ehrs[i].ehrID==loggedInPatient.pkID) {
            foundID=i;
            targetEHR = ehrs[i];
        }
    }

    free(ehrs);
    if(foundID!=-1) {
        printf("EHR loaded\n");
    } else {
        printf("Fatal error: EHR not found! Contact admin for technical fixing\n");
        getchar();
        return;
    }

    char readableBuffer[100];
    clearStr(readableBuffer);
    strcpy(readableBuffer, targetEHR.medicalHistory);
    replaceCharBy(readableBuffer, '_', ' ');
    printf("\n== HISTORY ==\n");
    printf("%s\n", readableBuffer);
    printf("-------------------\n");
    
    clearStr(readableBuffer);
    strcpy(readableBuffer, targetEHR.prescriptions);
    replaceCharBy(readableBuffer, '_', ' ');
    printf("\n== PRESCRIPTIONS ==\n");
    printf("%s\n", readableBuffer);
    printf("-------------------\n");
    

    clearStr(readableBuffer);
    strcpy(readableBuffer, targetEHR.allergies);
    replaceCharBy(readableBuffer, '_', ' ');
    printf("\n== ALLERGIES ==\n");
    printf("%s\n", readableBuffer);
    printf("-------------------\n");

    getchar();
}

//MANAGE PAYMENT SECTION
void patientViewBillingHistory() {
    int nbills;
    Bill *bills = getAllBills(&nbills);

    printf("Billing History");
    for(int i=0; i<nbills; i++) {
        if(bills[i].patientID==loggedInPatient.pkID) {
                printf("\n-----------------------------\n");
                printf("Bill ID = [%d]\n", bills[i].billID);
                printf("Patient ID: %d\n", bills[i].patientID);
                char invoiceDateStr[9];
                dateToStr(invoiceDateStr, &bills[i].invoiceDate); // convert date to string
                printf("Invoice Date (DDMMYYYY): %s\n", invoiceDateStr);
                printf("Service Provider: %s\n", bills[i].serviceProvider);
                printf("Amount Due: %.2f\n", bills[i].amountDue);            
                printf("\n-----------------------------\n");
        }
    }
}

/*
void patientViewBillingInformation(int patientIndex) {
    //patientVerify(patientIndex);

    
    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Billing Information: \n%s\n", patients[patientIndex].ehr.billingInformation);
}
*/

/*
void patientManageInsuranceClaims(int patientIndex) {

    int choice;
    printf("Insurance Claims Management:\n");
    printf("1. Submit Insurance Claim\n");
    printf("2. Check Insurance Claim Status\n");
    printf("3. Update Insurance Claim\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            patientSubmitInsuranceClaim(patientIndex);
            break;
        case 2:
            patientCheckInsuranceClaimStatus(patientIndex);
            break;
        case 3:
            patientUpdateInsuranceClaim(patientIndex);
            break;
        default:
            printf("Invalid choice.\n");
    }
}
*/

    /*

void patientSubmitInsuranceClaim(int patientIndex) {
    //patientVerify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    int availableClaimSlot = -1;
    for (int i = 0; i < MAX_CLAIMS_PS; i++) {
        if (patients[patientIndex].claims[i].claimID == 0) {
            availableClaimSlot = i;
            break;
        }
    }

    if (availableClaimSlot == -1) {
        printf("No available slots for new insurance claims.\n");
        return;
    }

    printf("Enter claim amount: ");
    float claimAmount;
    scanf("%f", &claimAmount);

    int claimID = availableClaimSlot + 1;
    patients[patientIndex].claims[availableClaimSlot].claimID = claimID;
    patients[patientIndex].claims[availableClaimSlot].patientID = patients[patientIndex].patientID;
    strcpy(patients[patientIndex].claims[availableClaimSlot].status, "Pending");
    
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(patients[patientIndex].claims[availableClaimSlot].dateSubmitted, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    patients[patientIndex].claims[availableClaimSlot].claimAmount = claimAmount;

    printf("Insurance claim submitted successfully! Claim ID: %d\n", claimID);
}
*/

/*
void patientCheckInsuranceClaimStatus(int patientIndex) {
    //patientVerify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Enter claim ID to check status: ");
    int claimID;
    scanf("%d", &claimID);

    for (int i = 0; i < MAX_CLAIMS_PS; i++) {
        if (patients[patientIndex].claims[i].claimID == claimID) {
            printf("Claim ID: %d\n", claimID);
            printf("Date Submitted: %s\n", patients[patientIndex].claims[i].dateSubmitted);
            printf("Status: %s\n", patients[patientIndex].claims[i].status);
            printf("Claim Amount: %.2f\n", patients[patientIndex].claims[i].claimAmount);
            return;
        }
    }

    printf("Claim ID not found.\n");
}
*/


/*
void patientUpdateInsuranceClaim(int patientIndex) {
    //patientVerify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Enter claim ID to update: ");
    int claimID;
    scanf("%d", &claimID);

    for (int i = 0; i < MAX_CLAIMS_PS; i++) {
        if (patients[patientIndex].claims[i].claimID == claimID) {
            printf("Enter new status (Pending, Approved, Rejected): ");
            char newStatus[20];
            scanf("%s", newStatus);

            strcpy(patients[patientIndex].claims[i].status, newStatus);
            printf("Claim status updated successfully!\n");
            return;
        }
    }

    printf("Claim ID not found.\n");
}
*/

void patientMainMenu() {
    int choice;
    bool logout = false;

    do {
        printf("\n== Patient Management System ==\n");
        printf("1. Appointments\n");
        printf("2. EHR\n");
        printf("3. Manage Payment\n");
        printf("4. Logout\n");
        printf("================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAppointmentsMenu();
                break;
            case 2:
                displayEHRMenu();
                break;
            case 3:
                displayPaymentMenu();
                break;
            case 4:
                logout = true;
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    } while (!logout);
}

void displayAppointmentsMenu() {
    int choice;
    bool backToMainMenu = false;

    while (!backToMainMenu) {
        printf("\n=== Appointments Menu ===\n");
        printf("1. View Appointments\n");
        printf("2. Book Appointment\n");
        printf("3. Reschedule Appointment\n");
        printf("4. Cancel Appointment\n");
        printf("5. Back to Main Menu\n");
        printf("=========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientViewAppointments();
                break;
            case 2:
                patientBookAppointment();
                break;
            case 3:
                patientRescheduleAppointment();
                break;
            case 4:
                patientCancelAppointment();
                break;
            case 5:
                backToMainMenu = true;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void displayEHRMenu() {
    int choice;
    bool backToMainMenu = false;

    while (!backToMainMenu) {
        printf("\n=== EHR Menu ===\n");
        printf("1. View EHR\n");
        printf("2. Back to Main Menu\n");
        printf("=================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientDisplayEHR();
                break;
            case 2:
                backToMainMenu = true;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
}

void displayPaymentMenu() {
    int choice;
    bool backToMainMenu = false;

    while (!backToMainMenu) {
        printf("\n=== Payment Menu ===\n");
        printf("1. View Billing History\n");
        //printf("2. View Billing Information\n");
        //printf("3. Manage Insurance Claims\n");
        printf("2. Back to Main Menu\n");
        printf("=====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientViewBillingHistory();
                break;
            /*
            case 2:
                patientViewBillingInformation(patientIndex);
                break;
            case 3:
                patientManageInsuranceClaims(patientIndex);
                break;
            */
            case 2:
                backToMainMenu = true;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
}


void patientAccess() {

    /*
    // Initialize patients
    for (int i = 0; i < MAX_PATIENTS_PS; i++) {
        patients[i].patientID = i + 1;
        strcpy(patients[i].personalID, "1234567890");
        strcpy(patients[i].name, "JohnDoe");
        strcpy(patients[i].password, "password");
        patients[i].isLoggedIn = false;
        patientInitializeEHR(&patients[i].ehr, i + 1);

        // Initialize claims
        for (int j = 0; j < MAX_CLAIMS_PS; j++) {
            patients[i].claims[j].claimID = 0; // Indicates empty slot
        }
    }
    */

    // Display options
    printf("PATIENT ACCESS\n-------------\n");
        
    char inPatientName[32];
    char inPatientPassword[32];
    printf("Enter patient name >> ");
    scanf("%s", inPatientName);
    printf("Enter patient password >> ");
    scanf("%s", inPatientPassword);

    // Login
    bool loginResult = false;
    loginResult = patientLogin(inPatientName, inPatientPassword);
    
    if(loginResult) {
        printf("Access granted\n");
        patientMainMenu(loggedInPatient.pkID);
    } 
    else {
        printf("Invalid username or password.\n");
        // return or try again
    }
}
