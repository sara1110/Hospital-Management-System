#pragma once
#ifndef DOCTOR_H
#define DOCTOR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "include\cmdui_utils.h"
#include "include\patientdb.h"
#include "include\doctordb.h" 
#include "include\appointmentdb.h"
#include "include\ehrdb.h"
#include "analytics.h"

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_APPOINTMENTS 100
#define MAX_FILENAME_LENGTH 50

// Define structures (REPLACE BY THOSE IN DATADEFS.H)
struct UserDS {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct AppointmentDS {
    char date[20];
    char time[20];
    char doctorName[50];
};

/*
// Global variables
struct User users[MAX_USERS];
int num_users = 0;

struct Appointment appointments[MAX_APPOINTMENTS];
int num_appointments = 0;
*/

void doctorDisplayLoggedOutMenu();
void doctorDisplayLoggedInMenu();
void doctorLogin(SystemState *sysState);
void doctorSecureEHRAccess(SystemState *sysState);
void doctorReportingAndAnalytics(SystemState *sysState);
void doctorAddAppointment(SystemState *sysState);
void doctorViewAllAppointments(SystemState *sysState);
void doctorViewTodaysAppointments(SystemState *sysState);
void doctorShowMonthlyAppointments(SystemState *sysState);
void doctorShowYearlyAppointments(SystemState *sysState);
void doctorViewPatientEHRByID(SystemState *sysState);
void doctorModifyPatientEHR(SystemState *sysState);
    void doctorUpdateMedicalHistory();//struct PatientRecord* patient);
    void doctorUpdatePrescription();//struct PatientRecord* patient);
    void doctorUpdateAllergies();//struct PatientRecord* patient);
int doctorValidateUser(const char* inUsername, const char* inPassword, SystemState *sysState);
void doctorLoggedOutMenu(SystemState *sysState);

#endif // DOCTOR_H