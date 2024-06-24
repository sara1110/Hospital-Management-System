#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PATIENTS 10
#define MAX_APPOINTMENTS 10

typedef struct {
    int appointmentID;
    int patientID;
    int doctorID;
    char date[10];
    char startTime[10];
    char doctorName[40];
    char originalDate[10];
    char originalTime[10];
    bool isRescheduled;
    bool isCancelled;
} Appointment;

typedef struct {
    int ehrID;
    int patientID; // Changed type to int
    char prescriptions[150];
    char billingInformation[100];
    char medicalHistory[100];
    char allergies[100];
} EHR;

typedef struct {
    int billID;
    int patientID; // Changed type to int
    char invoiceDate[10];
    char serviceProvider[20];
    float amountDue;
    float amountBreakdown;
} Billing;

typedef struct {
    int patientID;
    char personalID[20];
    char name[100];
    char password[10];
    bool isLoggedIn;
    Appointment appointments[MAX_APPOINTMENTS];
    EHR ehr;
    Billing billing;
} Patient;

Patient patients[MAX_PATIENTS_PS];

// Login|Logout Function
void login(const char* username, const char* password) {
    for (int i = 0; i < MAX_PATIENTS_PS; i++) {
        if (strcmp(patients[i].name, username) == 0 && strcmp(patients[i].password, password) == 0) {
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

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Appointments\n");

    int appointmentCount = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) {
        if (patients[patientIndex].appointments[i].appointmentID != 0) {
            appointmentCount++;
            printf("Appointment %d:\n", appointmentCount);
            printf("Doctor Name: %s\n", patients[patientIndex].appointments[i].doctorName);
            printf("Date: %s\n", patients[patientIndex].appointments[i].date);
            printf("Time: %s\n", patients[patientIndex].appointments[i].startTime);
            printf("-------------------------------\n");
        }
    }

    if (appointmentCount == 0) {
        printf("No appointments found.\n");
    }
}

// Reschedule Appointment Function
void reschedule_appointment_history(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Rescheduled Appointments\n");

    int rescheduledAppointmentCount = 0;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) {
        if (patients[patientIndex].appointments[i].isRescheduled) {
            rescheduledAppointmentCount++;
            printf("Appointment %d:\n", rescheduledAppointmentCount);
            printf("Original Date: %s\n", patients[patientIndex].appointments[i].originalDate);
            printf("Original Time: %s\n", patients[patientIndex].appointments[i].originalTime);
            printf("Rescheduled Date: %s\n", patients[patientIndex].appointments[i].date);
            printf("Rescheduled Time: %s\n", patients[patientIndex].appointments[i].startTime);
            printf("--------------------\n");
        }
    }

    if (rescheduledAppointmentCount == 0) {
        printf("No rescheduled appointments found.\n");
    }
}

// Cancelling Appointment Function
void cancel_appointment(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Enter the appointment number to cancel: ");
    int appointmentNumber;
    scanf("%d", &appointmentNumber);

    if (appointmentNumber < 1 || appointmentNumber > MAX_APPOINTMENTS) {
        printf("Invalid appointment number.\n");
        return;
    }

    if (!patients[patientIndex].appointments[appointmentNumber - 1].isCancelled) {
        patients[patientIndex].appointments[appointmentNumber - 1].isCancelled = true;
        printf("Appointment cancelled successfully.\n");
    } else {
        printf("Appointment is already cancelled.\n");
    }
}

// Book Appointment Function
void book_appointment(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    // Check if there are any available appointments
    int availableAppointmentSlot = -1;
    for (int i = 0; i < MAX_APPOINTMENTS; i++) {
        if (patients[patientIndex].appointments[i].appointmentID == 0) {
            availableAppointmentSlot = i;
            break;
        }
    }

    if (availableAppointmentSlot == -1) {
        printf("No available appointments. Please reschedule or cancel an existing appointment.\n");
        return;
    }

    printf("Enter doctor name: ");
    char doctorName[40];
    scanf("%s", doctorName);

    printf("Enter date (DD/MM/YYYY): ");
    char date[15];
    scanf("%s", date);

    printf("Enter time (HH:MM): ");
    char time[6];
    scanf("%s", time);

    // Generate a new appointment ID
    int appointmentID = availableAppointmentSlot + 1;
    patients[patientIndex].appointments[availableAppointmentSlot].appointmentID = appointmentID;

    // Set other appointment details
    strcpy(patients[patientIndex].appointments[availableAppointmentSlot].date, date);
    strcpy(patients[patientIndex].appointments[availableAppointmentSlot].startTime, time);
    strcpy(patients[patientIndex].appointments[availableAppointmentSlot].doctorName, doctorName);
    patients[patientIndex].appointments[availableAppointmentSlot].isRescheduled = false;
    strcpy(patients[patientIndex].appointments[availableAppointmentSlot].originalDate, "");
    strcpy(patients[patientIndex].appointments[availableAppointmentSlot].originalTime, "");
    patients[patientIndex].appointments[availableAppointmentSlot].isCancelled = false;

    printf("Appointment booked successfully! Appointment ID: %d\n", appointmentID);
}

void initialize_ehr(EHR *ehr, int patientID) {
    ehr->ehrID = patientID;
    ehr->patientID = patientID; // Initialize patientID here
    strcpy(ehr->medicalHistory, "");
    strcpy(ehr->prescriptions, "");
    strcpy(ehr->billingInformation, "");
    strcpy(ehr->allergies, "");
}

void add_medical_history(EHR *ehr, const char *history) {
    strcat(ehr->medicalHistory, history);
    strcat(ehr->medicalHistory, "\n");
}

void add_prescriptions(EHR *ehr, const char *prescriptions) {
    strcat(ehr->prescriptions, prescriptions);
    strcat(ehr->prescriptions, "\n");
}

void add_billing_information(EHR *ehr, const char *billing) {
    strcat(ehr->billingInformation, billing);
    strcat(ehr->billingInformation, "\n");
}

void add_allergies(EHR *ehr, const char *allergy) {
    strcat(ehr->allergies, allergy);
    strcat(ehr->allergies, "\n");
}

void display_ehr(const EHR *ehr) {
    printf("\nEHR for patient %d\n", ehr->ehrID);
    printf("Medical History:\n%s\n", ehr->medicalHistory);
    printf("Prescriptions:\n%s\n", ehr->prescriptions);
    printf("Billing Information:\n%s\n", ehr->billingInformation);
    printf("Allergies:\n%s\n", ehr->allergies);
}

int main() {
    // Initialize patients
    for (int i = 0; i < MAX_PATIENTS_PS; i++) {
        patients[i].patientID = i + 1;
        strcpy(patients[i].personalID, "1234567890");
        strcpy(patients[i].name, "John Doe");
        strcpy(patients[i].password, "password");
        patients[i].isLoggedIn = false;
        initialize_ehr(&patients[i].ehr, i + 1);
    }

	const int EXITCHOICE = 5;
	int choice;
	int invalidInput = 0;

	do {
		switch(choice) {
		case 1: break;
		case EXITCHOICE: break;
		default:
			break;

		}


	while(choice != EXITCHOICE);

/*

    // Login
    login("John Doe", "password");

    // Book an appointment
    book_appointment(0);

    // View appointments
    view_appointments(0);

    // Reschedule an appointment
    reschedule_appointment_history(0);

    // Cancel an appointment
    cancel_appointment(0);

    // Add medical history
    add_medical_history(&patients[0].ehr, "Some medical history");

    // Add prescriptions
    add_prescriptions(&patients[0].ehr, "Some prescriptions");

    // Add billing information
    add_billing_information(&patients[0].ehr, "Some billing information");

    // Add allergies
    add_allergies(&patients[0].ehr, "Some allergies");

    // Display EHR
    display_ehr(&patients[0].ehr);

    // Logout
    logout(0);
*/

    return 0;
}
