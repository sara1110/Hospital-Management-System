#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PATIENTS 10
#define MAX_APPOINTMENTS 10
#define MAX_CLAIMS 10

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
    int patientID;
    char prescriptions[150];
    char billingInformation[100];
    char medicalHistory[100];
    char allergies[100];
} EHR;

typedef struct {
    int billID;
    int patientID;
    char invoiceDate[10];
    char serviceProvider[20];
    float amountDue;
    float amountBreakdown;
} Billing;

typedef struct {
    int claimID;
    int patientID;
    char dateSubmitted[10];
    char status[20]; // e.g., "Pending", "Approved", "Rejected"
    float claimAmount;
} InsuranceClaim;

typedef struct {
    int patientID;
    char personalID[20];
    char name[100];
    char password[10];
    bool isLoggedIn;
    Appointment appointments[MAX_APPOINTMENTS];
    EHR ehr;
    Billing billing;
    InsuranceClaim claims[MAX_CLAIMS];
} Patient;

Patient patients[MAX_PATIENTS];

// Function prototypes
void login(const char* username, const char* password);
void logout(int patientIndex);
void verify(int patientIndex);
void view_appointments(int patientIndex);
void reschedule_appointment_history(int patientIndex);
void cancel_appointment(int patientIndex);
void book_appointment(int patientIndex);
void initialize_ehr(EHR *ehr, int patientID);
void add_medical_history(EHR *ehr, const char *history);
void add_prescriptions(EHR *ehr, const char *prescriptions);
void add_billing_information(EHR *ehr, const char *billing);
void add_allergies(EHR *ehr, const char *allergy);
void display_ehr(const EHR *ehr);
void view_billing_details(int patientIndex);
void view_billing_information(int patientIndex);
void manage_insurance_claims(int patientIndex);
void submit_insurance_claim(int patientIndex);
void check_insurance_claim_status(int patientIndex);
void update_insurance_claim(int patientIndex);

void login(const char* username, const char* password) {
    for (int i = 0; i < MAX_PATIENTS; i++) {
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

void verify(int patientIndex) {
    if (!patients[patientIndex].isLoggedIn) {
        printf("Please login first.\n");
        return;
    }
}

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

void view_billing_details(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    Billing *billing = &patients[patientIndex].billing;
    printf("Billing Details for patient %d:\n", billing->patientID);
    printf("Bill ID: %d\n", billing->billID);
    printf("Invoice Date: %s\n", billing->invoiceDate);
    printf("Service Provider: %s\n", billing->serviceProvider);
    printf("Amount Due: %.2f\n", billing->amountDue);
    printf("Amount Breakdown: %.2f\n", billing->amountBreakdown);
}

void view_billing_information(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Billing Information: \n%s\n", patients[patientIndex].ehr.billingInformation);
}

void manage_insurance_claims(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    int choice;
    printf("Insurance Claims Management:\n");
    printf("1. Submit Insurance Claim\n");
    printf("2. Check Insurance Claim Status\n");
    printf("3. Update Insurance Claim\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            submit_insurance_claim(patientIndex);
            break;
        case 2:
            check_insurance_claim_status(patientIndex);
            break;
        case 3:
            update_insurance_claim(patientIndex);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void submit_insurance_claim(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    int availableClaimSlot = -1;
    for (int i = 0; i < MAX_CLAIMS; i++) {
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

void check_insurance_claim_status(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Enter claim ID to check status: ");
    int claimID;
    scanf("%d", &claimID);

    for (int i = 0; i < MAX_CLAIMS; i++) {
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

void update_insurance_claim(int patientIndex) {
    verify(patientIndex);

    if (!patients[patientIndex].isLoggedIn) {
        return;
    }

    printf("Enter claim ID to update: ");
    int claimID;
    scanf("%d", &claimID);

    for (int i = 0; i < MAX_CLAIMS; i++) {
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

int main() {
    // Initialize patients
    for (int i = 0; i < MAX_PATIENTS; i++) {
        patients[i].patientID = i + 1;
        strcpy(patients[i].personalID, "1234567890");
        strcpy(patients[i].name, "John Doe");
        strcpy(patients[i].password, "password");
        patients[i].isLoggedIn = false;
        initialize_ehr(&patients[i].ehr, i + 1);

        // Initialize claims
        for (int j = 0; j < MAX_CLAIMS; j++) {
            patients[i].claims[j].claimID = 0; // Indicates empty slot
        }
    }

    // Example usage

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

    // Submit an insurance claim
    submit_insurance_claim(0);

    // Check insurance claim status
    check_insurance_claim_status(0);

    // Update insurance claim
    update_insurance_claim(0);

    // Manage insurance claims
    manage_insurance_claims(0);

    // Logout
    logout(0);

    return 0;
}
