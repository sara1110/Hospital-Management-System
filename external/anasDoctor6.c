#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_APPOINTMENTS 100
#define MAX_PATIENTS 100

// Structure Definitions
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct Appointment {
    char date[20];
    char time[20];
    char doctorName[MAX_USERNAME_LENGTH];
    int patientID;
};

struct PatientRecord {
    int patientID;
    char name[50];
    int age;
    char gender[10];
    char medicalHistory[255];
    char prescription[255];
    char allergies[255];
};

// Global Variables
struct User users[MAX_USERS] = {
    {"doctor1", "password1"},
    {"doctor2", "password2"},
};
int num_users = 2;

struct Appointment appointments[MAX_APPOINTMENTS] = {
    {"2024-06-24", "09:00", "doctor1", 1},
    {"2024-06-24", "10:00", "doctor1", 2},
    {"2024-06-24", "11:00", "doctor2", 3},
    {"2024-06-25", "09:00", "doctor1", 4},
};

int num_appointments = 4;

struct PatientRecord patients[MAX_PATIENTS] = {
    {1, "John Doe", 30, "Male", "No medical history", "", ""},
    {2, "Jane Smith", 25, "Female", "Allergic to penicillin", "", ""},
    {3, "Mike Johnson", 40, "Male", "Diabetic", "", ""},
    {4, "Anna Brown", 35, "Female", "Asthma", "", ""},

};
int num_patients = 4;

// Function Prototypes
void displayMainMenu();
void displayUserMenu();
void loginUser();
void secureEHRAccess(const char* doctorName);
void reportingAndAnalytics(const char* doctorName);
void addAppointment();
void viewAppointments(const char* doctorName);
void viewTodaysAppointments(const char* doctorName);
void viewPatientEHRByID(const char* doctorName);
void modifyPatientEHR(const char* doctorName);
void updateMedicalHistory(struct PatientRecord* patient);
void updatePrescription(struct PatientRecord* patient);
void updateAllergies(struct PatientRecord* patient);
int validateUser(const char* username, const char* password);

// Main Function
int main() {
    int choice;
    do {
        displayMainMenu();
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer
        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 2);
    return 0;
}

// Display Functions
void displayMainMenu() {
    printf("\n*** Main Menu ***\n");
    printf("1. Login\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");
}

void displayUserMenu() {
    printf("\n*** User Menu ***\n");
    printf("1. Add Appointment\n");
    printf("2. Secure EHR Access\n");
    printf("3. Reporting and Analytics\n");
    printf("4. Logout\n");
    printf("Enter your choice: ");
}

// User Authentication
int validateUser(const char* username, const char* password) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Valid user
        }
    }
    return 0; // Invalid user
}

void loginUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline character

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0';  // Remove newline character

    if (validateUser(username, password)) {
        printf("Login successful!\n");
        int choice;
        do {
            displayUserMenu();
            scanf("%d", &choice);
            getchar();  // Clear the newline character from the buffer
            switch (choice) {
                case 1:
                    addAppointment();
                    break;
                case 2:
                    secureEHRAccess(username);
                    break;
                case 3:
                    reportingAndAnalytics(username);
                    break;
                case 4:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 4);
    } else {
        printf("Invalid username or password.\n");
    }
}

// Appointment Management
void addAppointment() {
    char date[20];
    char time[20];
    char doctorName[MAX_USERNAME_LENGTH];
    int patientID;

    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, 20, stdin);
    date[strcspn(date, "\n")] = '\0';  // Remove newline character

    printf("Enter time (HH:MM): ");
    fgets(time, 20, stdin);
    time[strcspn(time, "\n")] = '\0';  // Remove newline character

    printf("Enter doctor's name: ");
    fgets(doctorName, MAX_USERNAME_LENGTH, stdin);
    doctorName[strcspn(doctorName, "\n")] = '\0';  // Remove newline character

    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer

    if (num_appointments < MAX_APPOINTMENTS) {
        strcpy(appointments[num_appointments].date, date);
        strcpy(appointments[num_appointments].time, time);
        strcpy(appointments[num_appointments].doctorName, doctorName);
        appointments[num_appointments].patientID = patientID;
        num_appointments++;
        printf("Appointment added successfully!\n");

        // After adding, display appointments again for the logged-in doctor
        viewAppointments(doctorName);
    } else {
        printf("Appointment database full. Cannot add new appointment.\n");
    }
}

void viewAppointments(const char* doctorName) {
    printf("\n*** Appointments for Dr. %s ***\n", doctorName);
    for (int i = 0; i < num_appointments; i++) {
        if (strcmp(appointments[i].doctorName, doctorName) == 0) {
            printf("Date: %s, Time: %s, Patient ID: %d\n", appointments[i].date, appointments[i].time, appointments[i].patientID);
        }
    }
}

void viewTodaysAppointments(const char* doctorName) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    printf("\n*** Today's Appointments for Dr. %s ***\n", doctorName);
    for (int i = 0; i < num_appointments; i++) {
        if (strcmp(appointments[i].doctorName, doctorName) == 0 && strcmp(appointments[i].date, today) == 0) {
            printf("Date: %s, Time: %s, Patient ID: %d\n", appointments[i].date, appointments[i].time, appointments[i].patientID);
        }
    }
}

// EHR Management
void secureEHRAccess(const char* doctorName) {
    int choice;
    do {
        printf("\n*** Secure EHR Access ***\n");
        printf("1. Search for Patient EHR (using patient ID)\n");
        printf("2. Modify Patient EHR\n");
        printf("3. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer
        switch (choice) {
            case 1:
                viewPatientEHRByID(doctorName);
                break;
            case 2:
                modifyPatientEHR(doctorName);
                break;
            case 3:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void viewPatientEHRByID(const char* doctorName) {
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer

    for (int i = 0; i < num_appointments; i++)
    {
        if (appointments[i].patientID == patientID && strcmp(appointments[i].doctorName, doctorName) == 0) {
            for (int j = 0; j < num_patients; j++) {
                if (patients[j].patientID == patientID) {
                    printf("\n*** Patient EHR ***\n");
                    printf("Patient ID: %d\n", patients[j].patientID);
                    printf("Name: %s\n", patients[j].name);
                    printf("Age: %d\n", patients[j].age);
                    printf("Gender: %s\n", patients[j].gender);
                    printf("Medical History: %s\n", patients[j].medicalHistory);
                    printf("Prescription: %s\n", patients[j].prescription);
                    printf("Allergies: %s\n", patients[j].allergies);
                    return;
                }
            }
        }
    }
    printf("Patient record not found or access denied.\n");
}

void modifyPatientEHR(const char* doctorName) {
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer

    for (int i = 0; i < num_appointments; i++) {
        if (appointments[i].patientID == patientID && strcmp(appointments[i].doctorName, doctorName) == 0) {
            for (int j = 0; j < num_patients; j++) {
                if (patients[j].patientID == patientID) {
                    int choice;
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
                            updateMedicalHistory(&patients[j]);
                            break;
                        case 2:
                            updatePrescription(&patients[j]);
                            break;
                        case 3:
                            updateAllergies(&patients[j]);
                            break;
                        case 4:
                            printf("Returning to secure EHR access...\n");
                            return;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
            }
        }
    }
    printf("Patient record not found or access denied.\n");
}

void updateMedicalHistory(struct PatientRecord* patient) {
    printf("Enter new medical history: ");
    fgets(patient->medicalHistory, 255, stdin);
    patient->medicalHistory[strcspn(patient->medicalHistory, "\n")] = '\0';  // Remove newline character
    printf("Medical history updated successfully.\n");
}

void updatePrescription(struct PatientRecord* patient) {
    printf("Enter new prescription: ");
    fgets(patient->prescription, 255, stdin);
    patient->prescription[strcspn(patient->prescription, "\n")] = '\0';  // Remove newline character
    printf("Prescription updated successfully.\n");
}

void updateAllergies(struct PatientRecord* patient) {
    printf("Enter new allergies: ");
    fgets(patient->allergies, 255, stdin);
    patient->allergies[strcspn(patient->allergies, "\n")] = '\0';  // Remove newline character
    printf("Allergies updated successfully.\n");
}

// Reporting and Analytics
void reportingAndAnalytics(const char* doctorName) {
    int choice;
    do {
        printf("\n*** Reporting and Analytics ***\n");
        printf("1. View Number of Appointments\n");
        printf("2. View Number of Patients\n");
        printf("3. View Today's Appointments\n");
        printf("4. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer
        switch (choice) {
            case 1:
                {
                    int count = 0;
                    for (int i = 0; i < num_appointments; i++) {
                        if (strcmp(appointments[i].doctorName, doctorName) == 0) {
                            count++;
                        }
                    }
                    printf("Total number of appointments: %d\n", count);
                }
                break;
            case 2:
                {
                    int patientIDs[MAX_PATIENTS];
                    int count = 0;
                    for (int i = 0; i < num_appointments; i++) {
                        if (strcmp(appointments[i].doctorName, doctorName) == 0) {
                            bool found = false;
                            for (int j = 0; j < count; j++) {
                                if (patientIDs[j] == appointments[i].patientID) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                patientIDs[count] = appointments[i].patientID;
                                count++;
                            }
                        }
                    }
                    printf("Total number of patients: %d\n", count);
                }
                break;
            case 3:
                viewTodaysAppointments(doctorName);
                break;
            case 4:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}
