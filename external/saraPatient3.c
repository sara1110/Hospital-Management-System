#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATIENTS 10
#define MAX_APPOINTMENTS 10

typedef struct {
    int appointmentID;
    char date[15];
    char time[6];
    char doctorName[40];
    bool isRescheduled;
    char originalDate[15];
    char originalTime[6];
    bool isCancelled;
} Appointment;

typedef struct {
    char prescriptions[150];
    Appointment appointmentHistory[MAX_APPOINTMENTS];
    char billingInformation[100];
    char medicalHistory[100];
} EHR;

typedef struct {
    char billingDetails[100];
    bool insuranceClaimStatus;
} Billing;

typedef struct {
    char username[20];
    char password[20];
    bool isLoggedIn;
    Appointment appointments[MAX_APPOINTMENTS];
    EHR ehr;
    Billing billing;
} Patient;

Patient patients[MAX_PATIENTS_PS];

// Login|Logout Function
void login(const char* username, const char* password) {
    
    for (int i = 0; i < MAX_PATIENTS_PS; i++) {
        if (strcmp(patients[i].username, username) == 0 && strcmp(patients[i].password, password) == 0) {
            patients[i].isLoggedIn = true;
            return;
        }
    }
    printf("Invalid username or password.\n");
}

void logout(int patientIndex) {
    patients[patientIndex].isLoggedIn = false;
}

// Verify feature
void verify(int patientIndex) {
    if (!patients[patientIndex].isLoggedIn) {
        printf("Please login first.\n");
        return;
    }
}

// View appointments function
void view_appointments(int patientIndex) {
    verify(patientIndex);

    printf("Appointments\n");

    int appointmentCount = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) {
        if (patients[patientIndex].appointments[i].appointmentID != 0) {
            appointmentCount++;
            printf("Appointment %d:\n", appointmentCount);
            printf("Doctor Name: %s\n", patients[patientIndex].appointments[i].doctorName);
            printf("Date: %s\n", patients[patientIndex].appointments[i].date);
            printf("Time: %s\n", patients[patientIndex].appointments[i].time);
            printf("-------------------------------\n");
        }
    }

    if (appointmentCount == 0) {
        printf("No appointments found.\n");
    }
}

// Reschedule appointment history function
void reschedule_appointment_history(int patientIndex) {
    verify(patientIndex);

    printf("Rescheduled Appointments\n");

    int rescheduledAppointmentCount = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) {
        if (patients[patientIndex].appointments[i].isRescheduled) {
            rescheduledAppointmentCount++;
            printf("Appointment %d:\n", rescheduledAppointmentCount);
            printf("Original Date: %s\n", patients[patientIndex].appointments[i].originalDate);
            printf("Original Time: %s\n", patients[patientIndex].appointments[i].originalTime);
            printf("Rescheduled Date: %s\n", patients[patientIndex].appointments[i].date);
            printf("Rescheduled Time: %s\n", patients[patientIndex].appointments[i].time);
            printf("--------------------\n");
        }
    }

    if (rescheduledAppointmentCount == 0) {
        printf("No rescheduled appointments found.\n");
    }
}

// Cancel appointment function
void cancel_appointment(int patientIndex) {
    verify(patientIndex);

    printf("Enter the appointment number to cancel: ");
    int appointmentNumber;
    scanf("%d", &appointmentNumber);

    if (appointmentNumber < 1 || appointmentNumber > MAX_APPOINTMENTS) {
        printf("Invalid appointment number.\n");
        return;
    }

    if (!patients[patientIndex].appointments[appointmentNumber-1].isCancelled) {
        patients[patientIndex].appointments[appointmentNumber-1].isCancelled = true;
        printf("Appointment cancelled successfully.\n");
    } else {
        printf("Appointment is already cancelled.\n");
    }
}

// Book appointment function
void book_appointment(int patientIndex) {
    verify(patientIndex);

    printf("Enter doctor name: ");
    char doctorName[40];
    scanf("%s", doctorName);

    printf("Enter date (DD/MM/YYYY): ");
    char date[15];
    scanf("%s", date);

    printf("Enter time (HH:MM): ");
    char time[6];
    scanf("%s", time);

    //int appointmentID = 1;
    //for (int i = 0; i < MAX_APPOINTMENTS; i++) {
       // if (patients

}

void menu1();

void genericMenu() {

    int choice;
    do {
        // DISPLAY
        printf("GENERIC MENU\n");
        printf("1. Go to menu 1\n");
        printf("2. Exit\n");

        // TAKE INPUT
        scanf("%d", &choice);

        // PROCESS INPUT AND STATE
        switch(choice) {
        case 1:
            //printf("Going to menu 1...\n");
            menu1();
            break;
        case 2:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid input, try again\n");
            break;
        }

    } while(choice != 2);
}

void menu1() {
    int choice;
    do {
        // DISPLAY
        printf("MENU 1\n");
        printf("1. Exit\n");

        // TAKE INPUT
        scanf("%d", &choice);

        // PROCESS INPUT AND STATE
        switch(choice) {
        case 1:
            break;
        default:
            printf("Invalid input, try again\n");
            break;
        }

    } while(choice != 1);
}

int main(int argc, char** argv) {

    genericMenu();

    return 0;
}