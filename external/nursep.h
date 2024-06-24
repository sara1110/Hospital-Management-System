#ifndef NURSEP_H
#define NURSEP_H

//Define constants
#define MAX_USERS 100
#define MAX_APPOINTMENTS 100
#define MAX_INVENTORY 100
#define MAX_PATIENT 100

//Define Nurse un and pw structure
typedef struct {
	char username[50];
	char password[50];
	int isLoggedIn;
} Nurse;

//Define Appointments information structure
typedef struct {
	char PatientName[50];
	char DoctorName[50];
	char Date[50];
	char Time [50];
	char Description[200];
} Appointment;

//Define structure for Inventory
typedef struct {
	char ItemName[50];
	char Category[50];
	int quantity;
} InventoryItem;

//Define structure for patient data for the case of generating reports
typedef struct {
    char name[50];
    int age;
    char gender[10];
    char medicalHistory[200];
} Patient;


#endif