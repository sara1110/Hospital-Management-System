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
void displayMenu();

int main() {
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, "users.txt");

    loadUsers(filename);

    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                addAppointment();
                break;
            case 3:
                // Implement Secure EHR Access
                break;
            case 4:
                // Implement Reporting and Analytics
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

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
    // Similar to saveUsers function
}

// Function to load appointments from a file
void loadAppointments(const char* filename) {
    // Similar to loadUsers function
}

// Function to display menu
void displayMenu() {
    printf("\n*** Menu ***\n");
    printf("1. Add User\n");
    printf("2. Add Appointment\n");
    printf("3. Secure EHR Access\n");
    printf("4. Reporting and Analytics\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}
