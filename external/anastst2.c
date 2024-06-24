#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_APPOINTMENTS 100
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

// Global variables
struct User users[MAX_USERS];
int num_users = 0;

struct Appointment appointments[MAX_APPOINTMENTS];
int num_appointments = 0;

// Function prototypes
void addUser();
int validateUser(const char* username, const char* password);
void addAppointment();
bool isAvailable(const char* date, const char* time);
void saveUsers(const char* filename);
void loadUsers(const char* filename);
void saveAppointments(const char* filename);
void loadAppointments(const char* filename);
void displayMainMenu();
void displayUserMenu();
void secureEHRAccess();
void reportingAndAnalytics();
void viewAppointments();
void viewUsers();
void loginUser();
void registerUser();

int main() {
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, "users.txt");

    loadUsers(filename);

    int choice;
    do {
        displayMainMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    saveUsers(filename);

    return 0;
}

// Function to add a new user to the system
void addUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

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
    scanf("%s", date);
    printf("Enter time (HH:MM): ");
    scanf("%s", time);
    printf("Enter doctor's name: ");
    scanf("%s", doctorName);

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

// Function to check availability
bool isAvailable(const char* date, const char* time) {
    // Logic to check if the specified date and time slot is available
    // You can iterate through existing appointments and check for conflicts
    return true; // Placeholder return value
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

// Function to display main menu
void displayMainMenu() {
    printf("\n*** Main Menu ***\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

// Function to display user menu
void displayUserMenu() {
    printf("\n*** User Menu ***\n");
    printf("1. Add User\n");
    printf("2. Add Appointment\n");
    printf("3. Secure EHR Access\n");
    printf("4. Reporting and Analytics\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");
}

// Function to handle user login
void loginUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (validateUser(username, password)) {
        printf("Login successful!\n");
        int choice;
        do {
            displayUserMenu();
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    addUser();
                    break;
                case 2:
                    addAppointment();
                    break;
                case 3:
                    secureEHRAccess();
                    break;
                case 4:
                    reportingAndAnalytics();
                    break;
                case 5:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    } else {
        printf("Invalid credentials. Please try again.\n");
    }
}

// Function to handle user registration
void registerUser() {
    addUser();
}

// Function for Secure EHR Access
void secureEHRAccess() {
    int choice;
    do {
        printf("\n*** Secure EHR Access ***\n");
        printf("1. View Appointments\n");
        printf("2. View Users\n");
        printf("3. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                viewAppointments();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
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
        printf("3. Return to User Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Total number of users: %d\n", num_users);
                break;
            case 2:
                printf("Total number of appointments: %d\n", num_appointments);
                break;
            case 3:
                printf("Returning to user menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}
