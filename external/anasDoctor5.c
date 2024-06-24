#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_APPOINTMENTS 100
#define MAX_PATIENTS 100
#define MAX_FILENAME_LENGTH 50

// Define structures
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct Appointment {
    char date[20];
    char time[20];
    char doctorName[50];
};

struct PatientRecord {
    int patientID;
    char name[50];
    int age;
    char gender[10];
    char medicalHistory[255];
};

// Global variables
struct User users[MAX_USERS];
int num_users = 0;

struct Appointment appointments[MAX_APPOINTMENTS];
int num_appointments = 0;

struct PatientRecord patients[MAX_PATIENTS];
int num_patients = 0;

// Function prototypes
void addUser();
int validateUser(const char* username, const char* password);
void addAppointment();
bool isAvailable(const char* date, const char* time);
void saveUsers(const char* filename);
void loadUsers(const char* filename);
void saveAppointments(const char* filename);
void loadAppointments(const char* filename);
void savePatients(const char* filename);
void loadPatients(const char* filename);
void displayMainMenu();
void displayUserMenu();
void secureEHRAccess();
void reportingAndAnalytics();
void viewAppointments();
void viewUsers();
void loginUser();
void addPatientRecord();
void viewPatientEHRByID();
void viewPatients();

int main() {
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, "users.txt");

    loadUsers(filename);
    loadAppointments("appointments.txt");
    loadPatients("patients.txt");

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

    saveUsers(filename);
    saveAppointments("appointments.txt");
    savePatients("patients.txt");

    return 0;
}

// Function to add a new user to the system
void addUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline character

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0';  // Remove newline character

    if (num_users < MAX_USERS) {
        strcpy(users[num_users].username, username);
        strcpy(users[num_users].password, password);
        num_users++;
        printf("User %s created successfully!\n", username);
    } else {
        printf("User database full. Cannot add new user.\n");
    }
}

// Function to validate user credentials
int validateUser(const char* username, const char* password) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Valid user
        }
    }
    return 0; // Invalid user
}

// Function to add a new appointment
void addAppointment() {
    char date[20];
    char time[20];
    char doctorName[50];

    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, 20, stdin);
    date[strcspn(date, "\n")] = '\0';  // Remove newline character

    printf("Enter time (HH:MM): ");
    fgets(time, 20, stdin);
    time[strcspn(time, "\n")] = '\0';  // Remove newline character

    printf("Enter doctor's name: ");
    fgets(doctorName, 50, stdin);
    doctorName[strcspn(doctorName, "\n")] = '\0';  // Remove newline character

    if (num_appointments < MAX_APPOINTMENTS) {
        strcpy(appointments[num_appointments].date, date);
        strcpy(appointments[num_appointments].time, time);
        strcpy(appointments[num_appointments].doctorName, doctorName);
        num_appointments++;
        printf("Appointment added successfully!\n");
    } else {
        printf("Appointment database full. Cannot add new appointment.\n");
    }
}

// Function to save users to a file
void saveUsers(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < num_users; i++) {
            fprintf(file, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(file);
    } else {
        printf("Error: Could not save users to file.\n");
    }
}

// Function to load users from a file
void loadUsers(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s", users[num_users].username, users[num_users].password) == 2) {
            num_users++;
        }
        fclose(file);
    } else {
        printf("Error: Could not load users from file.\n");
    }
}

// Function to save appointments to a file
void saveAppointments(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < num_appointments; i++) {
            fprintf(file, "%s %s %s\n", appointments[i].date, appointments[i].time, appointments[i].doctorName);
        }
        fclose(file);
    } else {
        printf("Error: Could not save appointments to file.\n");
    }
}

// Function to load appointments from a file
void loadAppointments(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s %s", appointments[num_appointments].date, appointments[num_appointments].time, appointments[num_appointments].doctorName) == 3) {
            num_appointments++;
        }
        fclose(file);
    } else {
        printf("Error: Could not load appointments from file.\n");
    }
}

// Function to save patients to a file
void savePatients(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < num_patients; i++) {
            fprintf(file, "%d %s %d %s %s\n", patients[i].patientID, patients[i].name, patients[i].age, patients[i].gender, patients[i].medicalHistory);
        }
        fclose(file);
    } else {
        printf("Error: Could not save patients to file.\n");
    }
}

// Function to load patients from a file
void loadPatients(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %d %s %s", &patients[num_patients].patientID, patients[num_patients].name, &patients[num_patients].age, patients[num_patients].gender, patients[num_patients].medicalHistory) == 5) {
            num_patients++;
        }
        fclose(file);
    } else {
        printf("Error: Could not load patients from file.\n");
    }
}

// Function to display main menu
void displayMainMenu() {
    printf("\n*** Main Menu ***\n");
    printf("1. Login\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");
}

// Function to display user menu
void displayUserMenu() {
    printf("\n*** User Menu ***\n");
    printf("1. Add Appointment\n");
    printf("2. Secure EHR Access\n");
    printf("3. Reporting and Analytics\n");
    printf("4. Add Patient Record\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");
}

// Function to handle user login
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
                    secureEHRAccess();
                    break;
                case 3:
                    reportingAndAnalytics();
                    break;
                case 4:
                    addPatientRecord();
                    break;
                case 5:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    } else {
        printf("Invalid username or password.\n");
    }
}

// Function for secure EHR access
void secureEHRAccess() {
    int choice;
    do {
        printf("\n*** Secure EHR Access ***\n");
        printf("1. View Appointments\n");
        printf("2. View Users\n");
        printf("3. View Patient EHR by ID\n");
        printf("4. View Patients\n");
        printf("5. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer
        switch (choice) {
            case 1:
                viewAppointments();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                viewPatientEHRByID();
                break;
            case 4:
                viewPatients();
                break;
            case 5:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

// Function to view appointments
void viewAppointments() {
    if (num_appointments > 0) {
        printf("\n*** Appointments ***\n");
        for (int i = 0; i < num_appointments; i++) {
            printf("Date: %s, Time: %s, Doctor: %s\n", appointments[i].date, appointments[i].time, appointments[i].doctorName);
        }
    } else {
        printf("No appointments available.\n");
    }
}

// Function to view users
void viewUsers() {
    if (num_users > 0) {
        printf("\n*** Users ***\n");
        for (int i = 0; i < num_users; i++) {
            printf("Username: %s\n", users[i].username);
        }
    } else {
        printf("No users available.\n");
    }
}

// Function for Reporting and Analytics
void reportingAndAnalytics() {
    int choice;
    do {
        printf("\n*** Reporting and Analytics ***\n");
        printf("1. View Number of Users\n");
        printf("2. View Number of Appointments\n");
        printf("3. View Number of Patients\n");
        printf("4. View Detailed List of Patients\n");
        printf("5. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the buffer
        switch (choice) {
            case 1:
                printf("Total number of users: %d\n", num_users);
                break;
            case 2:
                printf("Total number of appointments: %d\n", num_appointments);
                break;
            case 3:
                printf("Total number of patients: %d\n", num_patients);
                break;
            case 4:
                viewPatients();
                break;
            case 5:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

// Function to add a new patient record
void addPatientRecord() {
    int patientID;
    char name[50];
    int age;
    char gender[10];
    char medicalHistory[255];

    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer
    printf("Enter name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove newline character

    printf("Enter age: ");
    scanf("%d", &age);
    getchar();  // Clear the newline character from the buffer
    printf("Enter gender: ");
    fgets(gender, 10, stdin);
    gender[strcspn(gender, "\n")] = '\0';  // Remove newline character

    printf("Enter medical history: ");
    fgets(medicalHistory, 255, stdin);
    medicalHistory[strcspn(medicalHistory, "\n")] = '\0';  // Remove newline character

    if (num_patients < MAX_PATIENTS) {
        patients[num_patients].patientID = patientID;
        strcpy(patients[num_patients].name, name);
        patients[num_patients].age = age;
        strcpy(patients[num_patients].gender, gender);
        strcpy(patients[num_patients].medicalHistory, medicalHistory);
        num_patients++;
        printf("Patient record added successfully!\n");
    } else {
        printf("Patient database full. Cannot add new patient record.\n");
    }
}

// Function to view a patient's EHR by ID
void viewPatientEHRByID() {
    int patientID;
    printf("Enter patient ID: ");
    scanf("%d", &patientID);
    getchar();  // Clear the newline character from the buffer

    for (int i = 0; i < num_patients; i++) {
        if (patients[i].patientID == patientID) {
            printf("\n*** Patient EHR ***\n");
            printf("Patient ID: %d\n", patients[i].patientID);
            printf("Name: %s\n", patients[i].name);
            printf("Age: %d\n", patients[i].age);
            printf("Gender: %s\n", patients[i].gender);
            printf("Medical History: %s\n", patients[i].medicalHistory);
            return;
        }
    }
    printf("Patient record not found.\n");
}

// Function to view all patients
void viewPatients() {
    if (num_patients > 0) {
        printf("\n*** Patients ***\n");
        for (int i = 0; i < num_patients; i++) {
            printf("Patient ID: %d, Name: %s, Age: %d, Gender: %s, Medical History: %s\n",
                   patients[i].patientID, patients[i].name, patients[i].age, patients[i].gender, patients[i].medicalHistory);
        }
    } else {
        printf("No patients available.\n");
    }
}
