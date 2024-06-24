#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "include\patientdb.h"
#include "include\appointmentdb.h"
#include "include\ehrdb.h"
#include "include\billdb.h"

/*
#define MAX_PATIENTS_PS 10
#define MAX_APPOINTMENTS_PS 10
#define MAX_CLAIMS_PS 10

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
} AppointmentPS;

typedef struct {
    int ehrID;
    int patientID;
    char prescriptions[150];
    char billingInformation[100];
    char medicalHistory[100];
    char allergies[100];
} EHRPS;

typedef struct {
    int billID;
    int patientID;
    char invoiceDate[10];
    char serviceProvider[20];
    float amountDue;
    float amountBreakdown;
} BillPS;

typedef struct {
    int claimID;
    int patientID;
    char dateSubmitted[10];
    char status[20]; // e.g., "Pending", "Approved", "Rejected"
    float claimAmount;
} InsuranceClaimPS;

typedef struct {
    int patientID;
    char personalID[20];
    char name[100];
    char password[10];
    bool isLoggedIn;
    AppointmentPS appointments[MAX_APPOINTMENTS_PS];
    EHRPS ehr;
    BillPS billing;
    InsuranceClaimPS claims[MAX_CLAIMS_PS];
} PatientPS;
*/

// entry point function
void patientAccess();

void patientMainMenu();

bool patientLogin(const char* username, const char* password);
//void patientLogout(int patientIndex);
//void patientVerify(int patientIndex);
void patientViewAppointments();
void patientRescheduleAppointment();
void patientCancelAppointment();
void patientBookAppointment();
// void patientInitializeEHR();
void patientDisplayEHR();
void patientViewBillingHistory();
/*
void patientViewBillingInformation(int patientIndex);
void patientManageInsuranceClaims(int patientIndex);
void patientSubmitInsuranceClaim(int patientIndex);
void patientCheckInsuranceClaimStatus(int patientIndex);
void patientUpdateInsuranceClaim(int patientIndex);
*/
void patientMainMenu();
void displayAppointmentsMenu();
void displayEHRMenu();
void displayPaymentMenu();
#endif // PATIENT_H