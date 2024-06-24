#include "include\staffNurse.h"

//Global varaibles
NurseSNS nurses[MAX_USERS];
int nurseCount = 0; //initialize nurse users count

AppointmentSNS appointments[MAX_APPOINTMENTS];
int appointmentCount = 0; //initialize appointment count

InventoryItemSNS inventory[MAX_INVENTORY];
int inventoryCount = 0; //initialize inventory count

PatientSNS patients[MAX_PATIENT];
int patientCount = 0; //initialize patient count

//Define login function with staff nurse username and password which will also check the user validity
void staffNurseLogin() {
    clearScreen();
	char username[50], password[50];
	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter password: ");
	scanf("%s", password);

	for (int i = 0; i < nurseCount; i++) {
		if (strcmp(nurses[i].username, username) == 0 && strcmp(nurses[i].password, password) == 0) {
			nurses[i].isLoggedIn = 1;
			printf("Login successful!\n");
			staffNurseMainMenu();
			return;
		}
	}
	printf("Invalid username or password. please try again.\n");
}


//Define logout function
void staffNurseLogout() {
	for (int i = 0; i < nurseCount; i++) {
		if (nurses[i].isLoggedIn) {
			nurses[i].isLoggedIn = 0;
			printf("Logout successful!\n");
			return;
		}
	}
}


//Define nurseMenu Function which will show the options that Staff Nurse can do
void staffNurseMainMenu() {
    clearScreen();
	int choice1;
	do {
		printf("\n-------------------------------------------------\n");
		printf("|                 Staff Nurse Menu                 |\n");
		printf(" --------------------------------------------------\n");
		printf("|1. Doctor Appointments & Availablity Management   |\n");
		printf("|2. Inventory Management                           |\n");
		printf("|3. Generating Reports & Analytics                 |\n");
		printf("|4. Logout                                         |\n");
		printf(" --------------------------------------------------\n\n");
		printf("Enter your choice: ");
		scanf("%d", &choice1);

		switch (choice1) {
			case 1:
				staffNurseAccessDoctorSchedules();  //function declare 1
				break;
			case 2:
				staffNurseManageInventory(); //declare 2
				break;
			case 3:
				staffNurseGenerateReports(); //declare 3
				break;
			case 4:
				staffNurseLogout(); //decalre logout function
				break;
			default:
				printf("Invalid choice. Please try again.\n");
		}
	} while (choice1 != 4);
}


//Define Function for accessing appointments and availabilities of Doctor
void staffNurseAccessDoctorSchedules() {

    clearScreen();
    int choice2;

    do {
        printf("\n--------------------------------------------------\n");
        printf("|   Doctor Appointments & Availability Management   |\n");
        printf(" ---------------------------------------------------\n");
        printf("|1. View Doctor Schedules & Availabilities          |\n");
        printf("|2. Add new Appointments                            |\n");
        printf("|3. Remove Appointments                             |\n");
        printf("|4. Modify Appointments                             |\n");
        printf("|5. Exit                                            |\n");
        printf(" ---------------------------------------------------\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice2);

        switch (choice2) {
            case 1:
                printf("Doctor Schedules & Availabilities\n");
                for (int i = 0; i < appointmentCount; i++) {
                    printf("Patient Name: %s\nDoctor: %s\nDate: %s\nTime: %s\nDescription: %s\n", appointments[i].PatientName, appointments[i].DoctorName, appointments[i].Date, appointments[i].Time, appointments[i].Description);
                }
                break;
            case 2: {
                AppointmentSNS newAppointment;
                printf("Enter patient name: ");
                scanf("%s", newAppointment.PatientName);
                printf("Enter doctor name: ");
                scanf("%s", newAppointment.DoctorName);
                printf("Enter Date (MM-DD-YY): ");
                scanf("%s", newAppointment.Date);
                printf("Enter Time (HH:MM AM/PM): ");
                scanf("%s", newAppointment.Time);
                printf("Enter description of Appointment: ");
                scanf(" %[^\n]%*c", newAppointment.Description); // To read the whole line
                appointments[appointmentCount++] = newAppointment;
                printf("Appointment added successfully.\n");
                break;
            }
            case 3: {
                char patientName[50];
                printf("Enter patient name to remove appointment: ");
                scanf("%s", patientName);
                int found = 0;
                for (int i = 0; i < appointmentCount; i++) {
                    if (strcmp(appointments[i].PatientName, patientName) == 0) {
                        for (int j = i; j < appointmentCount - 1; j++) {
                            appointments[j] = appointments[j + 1];
                        }
                        appointmentCount--;
                        found = 1;
                        printf("Appointment removed successfully.\n");
                        break;
                    }
                }
                if (!found) {
                    printf("Appointment not found.\n");
                }
                break;
            }
            case 4: {
                char patientName[50];
                printf("Enter patient name to modify appointment: ");
                scanf("%s", patientName);
                int found = 0;
                for (int i = 0; i < appointmentCount; i++) {
                    if (strcmp(appointments[i].PatientName, patientName) == 0) {
                        int modifyChoice;
                        printf("\nWhat would you like to modify?\n");
                        printf("1. Doctor name\n");
                        printf("2. Appointment time\n");
                        printf("3. Appointment date\n");
                        printf("4. Appointment description\n");
                        printf("5. All fields\n\n");
                        printf("Enter your choice: ");
                        scanf("%d", &modifyChoice);

                        switch (modifyChoice) {
                            case 1:
                                printf("\nEnter new doctor name: ");
                                scanf("%s", appointments[i].DoctorName);
                                printf("Doctor name updated successfully.\n");
                                break;
                            case 2:
                                printf("Enter new appointment time: ");
                                scanf("%s", appointments[i].Time);
                                printf("Appointment time updated successfully.\n");
                                break;
                            case 3:
                                printf("Enter new appointment date: ");
                                scanf("%s", appointments[i].Date);
                                printf("Appointment date updated successfully.\n");
                                break;
                            case 4:
                                printf("Enter new appointment description: ");
                                scanf(" %[^\n]%*c", appointments[i].Description); // To read the whole line
                                printf("Appointment description updated successfully.\n");
                                break;
                            case 5:
                                printf("Enter new doctor name: ");
                                scanf("%s", appointments[i].DoctorName);
                                printf("Enter new appointment time: ");
                                scanf("%s", appointments[i].Time);
                                printf("Enter new appointment date: ");
                                scanf("%s", appointments[i].Date);
                                printf("Enter new appointment description: ");
                                scanf(" %[^\n]%*c", appointments[i].Description);
                                printf("All fields updated successfully.\n");
                                break;
                            default:
                                printf("Invalid choice. No changes made.\n");
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Appointment not found.\n");
                }
                break;
            }
            case 5:
                printf("Exiting Doctor Appointments & Availability Management...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (choice2 != 5) {
            int backChoice;
            printf("\nDo you want to go back to the previous menu?\n");
            printf("1. Yes\n");
            printf("2. No, Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &backChoice);

            if (backChoice == 2) {
                choice2 = 5; // Exit the loop
            }
        }

    } while (choice2 != 5);
}

void staffNurseManageInventory() {
	clearScreen();
    int choice3;
   do {
        printf("\n ---------------------------------------\n");
        printf("|         Inventory Management          |\n");
        printf(" ---------------------------------------\n");
        printf("|1. View Inventory                      |\n");
        printf("|2. Search Inventory                    |\n");
        printf("|3. View Low Stock Items                |\n");
        printf("|4. Add Inventory Item                  |\n");
        printf("|5. Remove Inventory Item               |\n");
        printf("|6. Modify Inventory Item               |\n");
        printf("|7. Exit                                |\n");
        printf(" ---------------------------------------\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice3);

        switch (choice3) {
            case 1:
                printf("Inventory List:\n");
                for (int i = 0; i < inventoryCount; i++) {
                    printf("Name: %s, Quantity: %d, Category: %s\n", inventory[i].ItemName, inventory[i].quantity, inventory[i].Category);
                }
                break;
            case 2: {
                char searchName[50];
                printf("Enter item name to search: ");
                scanf("%s", searchName);
                int found = 0;
                for (int i = 0; i < inventoryCount; i++) {
                    if (strcmp(inventory[i].ItemName, searchName) == 0) {
                        printf("Item Name: %s, Quantity: %d, Category: %s\n", inventory[i].ItemName, inventory[i].quantity, inventory[i].Category);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Item not found.\n");
                }
                break;
            }
            case 3:
                printf("Low Stock Items (Quantity <= 5):\n");
                int found = 0;
                for (int i = 0; i < inventoryCount; i++) {
                    if (inventory[i].quantity <= 5) {
                        printf("Name: %s, Quantity: %d, Category: %s\n", inventory[i].ItemName, inventory[i].quantity, inventory[i].Category);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No low stock items found.\n");
                }
                break;
            case 4: {
                InventoryItemSNS newItem;
                printf("Enter item name: ");
                scanf("%s", newItem.ItemName);
                printf("Enter quantity: ");
                scanf("%d", &newItem.quantity);
                printf("Enter category: ");
                scanf("%s", newItem.Category);
                inventory[inventoryCount++] = newItem;
                printf("Item added successfully.\n");
                break;
            }
            case 5: {
                char itemName[50];
                printf("Enter item name to remove: ");
                scanf("%s", itemName);
                int found = 0;
                for (int i = 0; i < inventoryCount; i++){
                    if (strcmp(inventory[i].ItemName, itemName) == 0) {
                        for (int j = i; j < inventoryCount - 1; j++) {
                            inventory [j] = inventory [j + 1];
                        }
                        inventoryCount--;
                        found = 1;
                        printf("Item removed successfully.\n");
                        break;
                    }
                }
                if (!found) {
                    printf("Item not found.\n");
                }
                break;
            }
            case 6: {
                char itemName[50];
                printf("Enter item name to modify: ");
                scanf("%s", itemName);
                int found = 0;
                for (int i = 0; i < inventoryCount; i++) {
                    if (strcmp(inventory[i].ItemName, itemName) == 0) {
                        int modifyChoice1;
                        printf("What would you like to modify?\n\n");
                        printf("1. Quantity\n");
                        printf("2. Category\n");
                        printf("3. Both\n\n");
                        printf("Enter your choice: ");
                        scanf("%d", &modifyChoice1);

                        switch (modifyChoice1) {
                            case 1:
                                printf("Enter new quantity: ");
                                scanf("%d", &inventory[i].quantity);
                                printf("Quantity updated successfully.\n");
                                break;
                            case 2:
                                printf("Enter new category: ");
                                scanf("%s", inventory[i].Category);
                                printf("Category updated successfully.\n");
                                break;
                            case 3:
                                printf("Enter new quantity: ");
                                scanf("%d", &inventory[i].quantity);
                                printf("Enter new category: ");
                                scanf("%s", inventory[i].Category);
                                printf("Both fields updated successfully.\n");
                                break;
                            default:
                                printf("Invalid choice. No changes made.\n");
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Item not found.\n");
                }
                break;
            }
            case 7:
                printf("Exiting Inventory Management...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (choice3 != 7) {
            int backChoice;
            printf("\nDo you want to go back to the previous menu?\n");
            printf("1. Yes\n");
            printf("2. No, Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &backChoice);

            if (backChoice == 2) {
                choice3 = 4; // Exit the loop
            }
        }

    } while (choice3 != 7);
}


//Function of generating reports with the patient data
void staffNurseGenerateReports() {
    clearScreen();
    int reportChoice;
    do {
        printf("\n -------------------------------------------\n");
        printf("|          Reporting and Analytics          |\n");
        printf(" -------------------------------------------\n");
        printf("|1. View All Patients                       |\n");
        printf("|2. View Patients by Doctor                 |\n");
        printf("|3. View Patient Appointments               |\n");
        printf("|4. Inventory Summary                       |\n");
        printf("|4. Exit                                    |\n");
        printf(" -------------------------------------------\n\n");
        printf("Enter your choice: ");
        scanf("%d", &reportChoice);

        switch (reportChoice) {
            case 1:
                printf("All Patients:\n");
                for (int i = 0; i < patientCount; i++) {
                    printf("Name: %s, Age: %d, Gender: %s, Medical History: %s\n", patients[i].name, patients[i].age, patients[i].gender, patients[i].medicalHistory);
                }
                break;
            case 2: {
                char doctorName[50];
                printf("Enter doctor name to view patients: ");
                scanf("%s", doctorName);
                int found = 0;
                for (int i = 0; i < appointmentCount; i++) {
                    if (strcmp(appointments[i].DoctorName, doctorName) == 0) {
                        for (int j = 0; j < patientCount; j++) {
                            if (strcmp(patients[j].name, appointments[i].PatientName) == 0) {
                                printf("Patient Name: %s, Age: %d, Gender: %s, Medical History: %s\n", patients[j].name, patients[j].age, patients[j].gender, patients[j].medicalHistory);
                                found = 1;
                            }
                        }
                    }
                }
                if (!found) {
                    printf("No patients found for Doctor %s.\n", doctorName);
                }
                break;
            }
            case 3: {
                char patientName[50];
                printf("Enter patient name to view appointments: ");
                scanf("%s", patientName);
                int found = 0;
                for (int i = 0; i < appointmentCount; i++) {
                    if (strcmp(appointments[i].PatientName, patientName) == 0) {
                        printf("Doctor: %s, Date: %s, Time: %s, Description: %s\n", appointments[i].DoctorName, appointments[i].Date, appointments[i].Time, appointments[i].Description);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No appointments found for Patient %s.\n", patientName);
                }
                break;
            }
            case 4:
                printf("Inventory Summary:\n");
                for (int i = 0; i < inventoryCount; i++) {
                    printf("Name: %s, Quantity: %d, Category: %s\n", inventory[i].ItemName, inventory[i].quantity, inventory[i].Category);
                }
                break;
            case 5:
                printf("Exiting Reporting and Analytics...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (reportChoice != 5) {
            int backChoice;
            printf("\nDo you want to go back to the previous menu?\n");
            printf("1. Yes\n");
            printf("2. No, Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &backChoice);

            if (backChoice == 2) {
                reportChoice = 5; // Exit the loop
            }
        }

    } while (reportChoice != 5);
}

//main function
void staffNurseLoggedOutMenu() {
    clearScreen();
    // Add sample nurse data for testing
    strcpy(nurses[0].username, "nurse1");
    strcpy(nurses[0].password, "password1");
    nurses[0].isLoggedIn = 0;
    nurseCount = 1;

    // Add sample patient data for testing
    strcpy(patients[0].name, "John Doe");
    patients[0].age = 30;
    strcpy(patients[0].gender, "Male");
    strcpy(patients[0].medicalHistory, "No allergies");
    patientCount = 1;

    // Add sample appointment data for testing
    strcpy(appointments[0].PatientName, "John Doe");
    strcpy(appointments[0].DoctorName, "Dr. Smith");
    strcpy(appointments[0].Date, "06-23-2024");
    strcpy(appointments[0].Time, "10:00 AM");
    strcpy(appointments[0].Description, "Regular check-up");
    appointmentCount = 1;

    // Add sample inventory data for testing
    strcpy(inventory[0].ItemName, "Bandages");
    strcpy(inventory[0].Category, "Medical Supplies");
    inventory[0].quantity = 50;
    inventoryCount = 1;

    int choice;
    do {
        printf("\n-------------------------------------------------\n");
        printf("|                 STAFF NURSE ACCESS             |\n");
        printf("|------------------------------------------------|\n");
        printf("|1. Login as Staff Nurse                         |\n");
        printf("|2. Exit                                         |\n");
        printf("|------------------------------------------------|\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                staffNurseLogin(); // Call the login function
                break;
            case 2:
                printf("Exiting the Hospital Management system...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 2);
}
